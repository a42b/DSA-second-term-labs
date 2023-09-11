#include <stdio.h>
#include <windows.h>
#include <math.h>
// n1=2, n2=1, n3=0, n4=1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[]="Лабораторна робота 3";
int N = 10;
float k1 = 1.0 - 0*0.01 - 1*0.005 - 0.05;

//list
typedef struct Node{
    int num;
    int* edges;
    struct Node* nextVertex;
} Node;

Node* graph;
Node* spTree;
int* visited;

Node* startGraph (int n, int vertexNum);
Node* addNode(Node* graph_p, int n, int vertexNum);
void addEdge(Node* graph_p, int from, int to, int weight);
void print_list(Node* graph_p, int n);
int del_list(Node* graph_p);
Node* createGraphFromMatrix(int** matrix, int n);
int allVisited(int* array, int n);
Node* prim (Node* graph, Node* current_p, int n, int* visit, Node* tree, HDC hdc);

float** T = NULL;
int** A = NULL;
int** symMatrix = NULL;
int** Wt = NULL;
int** B = NULL;
int** C = NULL;
int** D = NULL;
int** Tr = NULL;
int** W = NULL;

char nn[10][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; //names of vertex
float nxD[10];
float nyD[10];
float nxUn[10];
float nyUn[10];

int xWindowCorner = 50;
int yWindowCorner = 50;
int windowWidth = 1400;
int windowHeight = 700;

int vertexRad = 28;
int dtx = 5;


//allocates memory for matrix and assigns to random float(0-2);
float** randm(int rows, int cols); // also prints matrix in console

// multiplies a matrix T by the coef(k) and creates new matrix of integers 1 and 0 based on T matrix
int** mulmr(float coef, float** matrix, int rows, int cols);

int** makeC(int** matrixB, int n);
int** makeD(int** matrixB, int n);
int** makeTr(int n);

int** roundm(float** matrix, int k, int** matrixA, int n);

// changes matrix to symmetrical
int** symmetricMatrix (int** matrix, int n);

int** copyMatrix (int** matrix, int n);

void printMatrix (int** matrix, int n);

void finishWt(int** matrixWt, int** matrixC, int** matrixD, int** matrixTr, int n);

// frees memory for float matrix
void freeMatrixFloat(float** matrix, int n);

// frees memory for integer matrix
void freeMatrixInt(int** matrix, int n);

//calculates vertices coords for graph based on window size
void calcCoords(int xWidth, int yHeight, int n, float* nx, float* ny);

void calcCoordsTree(int xWidth, int yHeight, int n, float* nx, float* ny);

void drawGraph(HDC hdc, float* nxUn, float* nyUn, Node* graph, int verticeRad, HPEN pen, int n);

void drawMatrixGraph(HDC hdc, float* nxUn, float* nyUn, int** matrix, int verticeRad, HPEN pen, int n);

void halt();

void drawLine(HDC hdc, float* nxUn, float* nyUn , int from, int to, int weight);

void calculateCoordinatesWithDistance(int x1, int y1, int x2, int y2, int distance, int* newX, int* newY);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS w;
    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    if(!RegisterClass(&w))
        return 0;
    HWND hWnd;
    MSG lpMsg;


    hWnd = CreateWindow(ProgName,
                        "Лабораторна робота 3. Виконала Бондар Антоніна",
                        WS_OVERLAPPEDWINDOW,
                        xWindowCorner,
                        yWindowCorner,
                        windowWidth,
                        windowWidth/2,
                        (HWND)NULL,
                        (HMENU)NULL,
                        (HINSTANCE)hInstance,
                        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (messg) {
        case WM_PAINT :
            hdc = BeginPaint(hWnd, &ps);
            if (T == NULL ) {
                printf("\nPrinting random matrix T: \n");
                T = randm(N, N);

                printf("\nPrinting matrix A: \n");
                A = mulmr(k1, T, N, N);

                printf("\nPrinting matrix Wt: \n");
                Wt = roundm(T, 100, A, N);

                B = copyMatrix(A, N);
                printf("\nPrinting matrix B: \n");
                printMatrix(B, N);

                C = makeC(B, N);
                printf("\nPrinting matrix C: \n");
                printMatrix(C, N);

                D = makeD(B, N);
                printf("\nPrinting matrix D: \n");
                printMatrix(D, N);

                Tr = makeTr(N);
                printf("\nPrinting matrix Tr: \n");
                printMatrix(Tr, N);

                finishWt(Wt, C, D, Tr, N);
                printf("\nPrinting matrix final Wt: \n");
                printMatrix(Wt, N);

                W = symmetricMatrix(Wt, N);
                printf("\nPrinting matrix final W symmetrical: \n");
                printMatrix(W, N);

                graph = createGraphFromMatrix(W, N);

            }

            HPEN BPen = CreatePen(PS_SOLID, 3, RGB(124, 67, 109));
            HPEN KPen = CreatePen(PS_SOLID, 1, RGB(116, 31, 78));

            calcCoordsTree(windowWidth, windowHeight, N, nxUn, nyUn);
            calcCoords(windowWidth, windowHeight, N, nxD, nyD);
            //drawMatrixGraph(hdc, nxD, nyD, W, vertexRad, KPen, N);

            //drawGraph(hdc, nxUn, nyUn, SmtTree, vertexRad, KPen, N );
            drawGraph(hdc, nxD, nyD, graph, vertexRad, KPen, N );


            SelectObject(hdc, BPen);
            //draw directed graph vertices
            for (int i = 0; i < N; i++) {
                Ellipse(hdc, nxD[i] - vertexRad, nyD[i] - vertexRad, nxD[i] + vertexRad, nyD[i] + vertexRad);
                TextOut(hdc, nxD[i] - dtx, nyD[i] - vertexRad / 2, nn[i], 2);
            }
            spTree = prim(graph, graph, N, visited, spTree, hdc);
            for (int i = 0; i < N; i++) {
                Ellipse(hdc, nxUn[i] - vertexRad, nyUn[i] - vertexRad, nxUn[i] + vertexRad, nyUn[i] + vertexRad);
                TextOut(hdc, nxUn[i] - dtx, nyUn[i] - vertexRad / 2, nn[i], 2);
            }

            DeleteObject(KPen);
            DeleteObject(BPen);



            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            freeMatrixFloat(T, N);
            freeMatrixInt(A, N);
            freeMatrixInt(symMatrix, N);
            freeMatrixInt(W,N);
            freeMatrixInt(B, N);
            freeMatrixInt(C,N);
            freeMatrixInt(D,N);
            freeMatrixInt(Wt,N);
            freeMatrixInt(Tr,N);
            freeMatrixInt(B, N);
            del_list(spTree);
            del_list(graph);


            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }

}

float** randm(int rows, int cols) {
    printf("Printing matrix after randm:\n");
    int i,j;
    float** matrix = (float**)malloc(sizeof(float*) * rows );
    srand(2101);
    for (i = 0; i < rows; i++) {
        matrix[i] = (float*)malloc(sizeof(float)* cols);
        for (j = 0; j < cols; j++) {
            matrix[i][j] = ((float)rand()/RAND_MAX)*(float)(2);
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
    return matrix;
}

int** mulmr(float coef, float** matrix, int rows, int cols){
    printf("Printing matrix after mulmr (directed matrix):\n");
    int** matrixMul = (int**)malloc(sizeof(int*) * rows );
    for (int i = 0; i < rows; i++) {
        matrixMul[i] = (int*)malloc(sizeof(int)* cols);
        for (int j = 0; j < cols; j++) {
            matrixMul[i][j] = (int)floor(matrix[i][j] * coef);
            printf("%d\t", matrixMul[i][j]);
        }
        printf("\n");
    }
    // freeMatrixFloat(matrix,n);
    return matrixMul;
}

int** roundm(float** matrix, int k, int** matrixA, int n){
    printf("Printing matrix after mulmr (directed matrix):\n");
    int** matrixW = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        matrixW[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            matrixW[i][j] = (int)round((matrix[i][j] * k) * (matrixA[i][j]));
            printf("%d\t", matrixW[i][j]);
        }
        printf("\n");
    }
    // freeMatrixFloat(matrix,n);
    return matrixW;
}

int** symmetricMatrix (int** matrix, int n) {
    printf("\nPrinting symmetric matrix :\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i] && matrix[j][i] == 0) {

                matrix[j][i] = matrix[i][j];
            }
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    return matrix;
}

int** copyMatrix (int** matrix, int n){
    int** copy = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        copy[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

void printMatrix (int** matrix, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int** makeC(int** matrixB, int n){
    int** matrixC = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        matrixC[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            if (matrixB[i][j] != matrixB[j][i]){
                matrixC[i][j] = 1;
            } else {
                matrixC[i][j] = 0;
            }
        }
    }
    return matrixC;
}

int** makeD(int** matrixB, int n){
    int** matrixD = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        matrixD[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            if (matrixB[i][j] == matrixB[j][i] && matrixB[i][j] == 1){
                matrixD[i][j] = 1;
            } else {
                matrixD[i][j] = 0;
            }
        }
    }
    return matrixD;
}


int** makeTr(int n){
    int** matrixTr = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        matrixTr[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            if (i < j){
                matrixTr[i][j] = 1;
            } else {
                matrixTr[i][j] = 0;
            }
        }
    }
    return matrixTr;
}

void finishWt(int** matrixWt, int** matrixC, int** matrixD, int** matrixTr, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixWt[i][j] *= (matrixC[i][j]+(matrixD[i][j] * matrixTr[i][j]));
        }
    }
}

void calcCoords(int xWidth, int yHeight, int n, float* nx, float* ny){
    float rad = (yHeight - 250)/2;
    float x0 = xWidth/4;
    float y0 = yHeight/2;
    float angle = 2 * M_PI /n;
    for (int i = 0; i<n; i++){
        float currentAngle = i * angle;
        nx[i] = x0 + rad * cos(currentAngle);
        ny[i] = y0 + rad * sin(currentAngle);
    }
}

void drawMatrixGraph(HDC hdc, float* nxUn, float* nyUn, int** matrix, int verticeRad, HPEN pen, int n){
    SelectObject(hdc, pen);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                MoveToEx(hdc, nxUn[i], nyUn[i], NULL);
                if (i == j) {
                    if (i <= 2 || i >= 8) {
                        Ellipse(hdc, nxUn[i], nyUn[i] - verticeRad, nxUn[i] + 2 * verticeRad, nyUn[i] + verticeRad);
                    } else {
                        Ellipse(hdc, nxUn[i] - 2 * verticeRad, nyUn[i] - verticeRad, nxUn[i], nyUn[i] + verticeRad);
                    }
                } else {
                    LineTo(hdc, nxUn[j], nyUn[j]);
                }
            }
        }
    }
}

void freeMatrixInt(int** matrix, int n){
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void freeMatrixFloat(float** matrix, int n){
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}


Node* startGraph (int n, int vertexNum){
    Node* node_p = malloc(sizeof(Node));
    int* edgs = malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++){
        edgs[i] = 0;
    }
    *node_p = (Node){
            .num = vertexNum,
            .edges = edgs,
            .nextVertex = NULL
    };
    printf("\nStarting graph\n");
    printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",node_p, node_p->num, node_p->nextVertex);
    printf("Edges of this vertex:\t");
    for (int i = 0; i < n; i++){
        printf("%d,\t", node_p->edges[i]);
    }
    printf("\n");
    return node_p;
}

Node* addNode(Node* graph_p, int n, int vertexNum){
    Node* node_p = malloc(sizeof(Node));
    int* edgs = malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++){
        edgs[i] = 0;
    }
    *node_p = (Node){
            .num = vertexNum,
            .edges = edgs,
            .nextVertex = graph_p
    };
    printf("\nAdding a vertex\n");
    printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",node_p, node_p->num, node_p->nextVertex);
    printf("Edges of this vertex:\t");
    for (int i = 0; i < n; i++){
        printf("%d,\t", node_p->edges[i]);
    }
    printf("\n");
    return node_p;
}

void addEdge(Node* graph_p, int from, int to, int weight){
    Node* this_p = graph_p;
    while(this_p != NULL){
        int vertex = this_p->num;
        if (vertex == from){
            this_p->edges[to] = weight;
            return;
        }
        this_p = this_p ->nextVertex;
    }
}

void print_list(Node* graph_p, int n){
    printf("\nPrinting list\n");
    printf("graph_p = %p\n", graph_p);
    while (graph_p != NULL) {
        printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",graph_p, graph_p->num, graph_p->nextVertex);
        printf("Edges of this vertex:\t");
        for (int i = 0; i < n; i++){
            printf("%d,\t", graph_p->edges[i]);
        }
        printf("\n");
        graph_p = graph_p->nextVertex;
    }
    printf("list ended :(\n");

}

int del_list(Node* graph_p){
    printf("deleting\n");
    Node* next_p = graph_p -> nextVertex;
    if(graph_p != NULL){
        free(graph_p);
        printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",graph_p, graph_p->num, graph_p->nextVertex);
        del_list(next_p);
    }
    return 0;
}

Node* createGraphFromMatrix(int** matrix, int n){
    Node* graph_p = startGraph(n, 0);
    for (int i = 0; i < n-1; i++){
        graph_p = addNode(graph_p, n,i+1);
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (matrix[i][j] != 0){
                addEdge(graph_p, i, j, matrix[i][j]);
            }
        }
    }
    return graph_p;
}

Node* findVertex (Node* graph, int vertex){
    Node* curr_p = graph;
    printf("finding vertex %d\n", vertex);
    while(curr_p != NULL){
        printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",curr_p, curr_p->num, curr_p->nextVertex);
        if (curr_p->num == vertex){
            return curr_p;
        }
        curr_p = curr_p->nextVertex;
    }
    return NULL;
}

Node* prim (Node* graph, Node* current_p, int n, int* visit, Node* tree, HDC hdc){
    printf("\nPrim\n");
    Node* curr_p = current_p;
    int vertexNum = curr_p->num;
    printf("This node p = %p;\tthis vertex number = %d;\tthis next vertex p = %p\n",curr_p, vertexNum, curr_p->nextVertex);

    if (tree == NULL){
        printf("Prim start\n");
        tree = startGraph(n, vertexNum);
        visit = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) {
            visit[i] = 0;
        }
    }

    visit[vertexNum] = 1;
    int newVertexNum = -1;
    int minWeight = INT_MAX;

    for (int i = 0; i < n; i++){
        if (curr_p->edges[i] != 0 && visit[i] == 0 && curr_p->edges[i] < minWeight){
            minWeight = curr_p->edges[i];
            newVertexNum  = i;
        }
    }
    printf("Min weight = %d\tNew vertex: %d\n", minWeight, newVertexNum);

    if (newVertexNum != -1){
        tree = addNode(tree, n, newVertexNum);
        addEdge(tree, vertexNum, newVertexNum, minWeight);
        addEdge(tree, newVertexNum, vertexNum, minWeight);

        drawLine(hdc, nxUn, nyUn, vertexNum, newVertexNum, minWeight);
        halt();

        printf("Vertex num = %d\tnew vertex num = %d\n", vertexNum, newVertexNum);
        //print_list(tree, n);
        curr_p = findVertex(graph, newVertexNum);
        printf("Curr p = %p\n", curr_p);
        prim(graph, curr_p, n, visit, tree, hdc);
    } else {
        printf("\nCase 1!!\n");
        if (!allVisited(visit, n)){
            printf("\nCase 2!!\n");
            newVertexNum = findVertex(tree, curr_p->num)->nextVertex->num;
            printf("Vertex num = %d\tNew vertex num = %d\n", vertexNum, newVertexNum);

            curr_p = findVertex(graph, newVertexNum);
            printf("Curr p = %p\n", curr_p);
            prim(graph, curr_p, n, visit, tree, hdc);
        } else {
            printf("\nCase 3!!\n");
            free(visit);
            printf("Tree pointer = %p\n", tree);
            return tree;
        }
    }
}

int allVisited(int* array, int n){
    int allvisit = 1;
    for (int i = 0; i < n; i++){
        if (array[i] == 0){
            allvisit = 0;
        }
    }
    return allvisit;
}
void drawGraph(HDC hdc, float* nxUn, float* nyUn, Node* graph, int vertexRad, HPEN pen, int n){
    SelectObject(hdc, pen);
    int graphCenterX = (nxUn[0]+nxUn[5])/2;
    int graphCenterY = (nyUn[0]+nyUn[5])/2;
    Node* curr_p = graph;
    while(curr_p != NULL){
        for (int i = 0; i < n; i++){
            if (curr_p->edges[i] != 0 && i > curr_p->num){
                MoveToEx(hdc, nxUn[curr_p->num], nyUn[curr_p->num], NULL);
                if (curr_p->num == i) {
                    if (i <= 2 || i >= 8) {
                        Ellipse(hdc, nxUn[curr_p->num], nyUn[curr_p->num] - vertexRad, nxUn[curr_p->num] + 2 * vertexRad, nyUn[curr_p->num] + vertexRad);
                    } else {
                        Ellipse(hdc, nxUn[curr_p->num] - 2 * vertexRad, nyUn[curr_p->num] - vertexRad, nxUn[curr_p->num], nyUn[curr_p->num] + vertexRad);
                    }
                } else {
                    int centerX = ((nxUn[curr_p->num] + nxUn[i]) / 2);
                    int centerY = (nyUn[curr_p->num] + nyUn[i]) / 2;

                    char weightStr[5];
                    sprintf(weightStr, "%d", curr_p->edges[i]);

                    //if lines intercept in the middle
                    if (centerX == graphCenterX && centerY == graphCenterY) {
                        calculateCoordinatesWithDistance(centerX, centerY, nxUn[i], nyUn[i], 40,
                                                         &centerX, &centerY);
                    }
                    LineTo(hdc, nxUn[i], nyUn[i]);
                    Ellipse(hdc, centerX - 16, centerY - 16, centerX + 16, centerY + 16);
                    TextOut(hdc, centerX - 10, centerY - 10, weightStr, 3);

                }
            }
        }
        curr_p = curr_p->nextVertex;
    }
}

void drawLine(HDC hdc, float* nxUn, float* nyUn , int from, int to, int weight){
    MoveToEx(hdc, nxUn[from], nyUn[from], NULL);
    int centerX = ((nxUn[from] + nxUn[to]) / 2);
    int centerY = (nyUn[from] + nyUn[to]) / 2;
    char weightStr[5];

    sprintf(weightStr, "%d",weight);

    LineTo(hdc, nxUn[to], nyUn[to]);
    Ellipse(hdc, centerX - 18, centerY - 18, centerX + 18, centerY + 18);
    TextOut(hdc, centerX-10 , centerY-10 , weightStr, 3);

    for (int i = 0; i < N; i++) {
        Ellipse(hdc, nxUn[i] - vertexRad, nyUn[i] - vertexRad, nxUn[i] + vertexRad, nyUn[i] + vertexRad);
        TextOut(hdc, nxUn[i] - dtx, nyUn[i] - vertexRad / 2, nn[i], 2);
    }
}

void calcCoordsTree(int xWidth, int yHeight, int n, float* nx, float* ny){
    float rad = (yHeight - 250)/2;
    float x0 =  (xWidth/4)*3;
    float y0 = yHeight/2;
    float angle = 2 * M_PI /n;
    for (int i = 0; i<n; i++){
        float currentAngle = i * angle;
        nx[i] = x0 + rad * cos(currentAngle);
        ny[i] = y0 + rad * sin(currentAngle);
    }
}

void halt() {
    MSG message;
    MessageBox(NULL, "Press any key to continue...", "Halt", MB_OK);
    GetMessage(&message, NULL, 0, 0);
}


void calculateCoordinatesWithDistance(int x1, int y1, int x2, int y2, int distance, int* newX, int* newY) {
    double length = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    double ratio = distance / length;

    *newX = x1 + (int)((x2 - x1) * ratio);
    *newY = y1 + (int)((y2 - y1) * ratio);
    printf("x= %d\t y = %d\n", *newX, *newY);
}