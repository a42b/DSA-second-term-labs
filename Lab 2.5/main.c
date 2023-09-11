#include <stdio.h>
#include <windows.h>
#include <math.h>
#define N 10
// n1=2, n2=1, n3=0, n4=1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[]="Лабораторна робота 3";
int n = 10;
float k = 1.0-0.005-0.15;
float** T = NULL;
int** A = NULL;
int** symMatrix = NULL;
int** bfsTree = NULL;
int* bfsVisited = NULL;
int** dfsTree = NULL;
int* dfsVisited = NULL;
int flag1 = 0;
int vertexRadGraph = 28;
int dtx = 5;
int bfsNumber = 0;
int dfsNumber = 0;
int** NewMatrix;

char nn[10][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; //names of vertex
float nxD[10];   // x coordinates for directed graph
float nyD[10];   // y coordinates for directed graph

// bfs
typedef struct queue {
    int items[N];
    int front;
    int rear;
} queue;

queue* bfsQ;

queue* createQueue();
int isEmpty (queue* q);
void enqueue (queue* q, int value);
int dequeue(struct queue* q);
void printQueue(struct queue* q);
void textOutQueue(HDC hdc, queue* q, int n);

void bfs_next(int** matrix, queue* q, int* visited, int** tree, HDC hdc);
void startBfs(int** matrix, int start_v, queue** q, int** visited, int*** tree, HDC hdc);
//

//dfs
typedef struct {
    int capacity;
    int top;
    int* array;
} Stack;

Stack* dfsStack;

Stack* createStack(int capacity);
void printStack(Stack* s);
int peek(Stack* stack);
void push(Stack* stack, int value);
int pop(Stack* stack);
int isStackEmpty(Stack* stack);
void textOutStack(HDC hdc, Stack* s, int n);

void dfs_next(int** matrix, Stack* s, int* visited, int** tree, HDC hdc, int n);
void start_dfs(int** matrix, int start_v, Stack** s, int** visited, int*** tree, HDC hdc, int n);
//

//window size
int xWindowCorner = 50;
int yWindowCorner = 50;
int windowWidth = 1400;
int windowHeight = 700;

//allocates memory for matrix and assigns to random float(0-2);
float** randm(int rows, int cols); // also prints matrix in console

// multiplies a matrix T by the coef(k) and creates new matrix of integers 1 and 0 based on T matrix (allocates new space and frees the previous matrix)
int** mulmr(float coef, float** matrix, int rows, int cols); // also prints matrix in console

// changes matrix to symmetrical
int** undirectMatrix (int** matrix, int n);

// frees memory for float matrix(matrix after randm)
void freeMatrixFloat(float** matrix, int n);

// frees memory for integer matrix(matrix after mulmr)
void freeMatrixInt(int** matrix, int n);

//draws arrow head
void arrow(HDC hdc, float fi, int px,int py);

//calls arrow() with the calculated parameters
void drawArrow(HDC hdc, int xStart, int yStart, int xEnd, int yEnd, int vertexRad);

//calculates vertices coords for directed graph based on window size
void calcCoordsDirect(int xWidth, int yHeight, int n, float* nx, float* ny);

void drawDirectGraph(HDC hdc, float* xCoords, float* yCoords, int** matrix, int vertexRad, HPEN pen);

void draw_vertex(HDC hdc, int i);

void draw_line (HDC hdc, int i, int j);

void printTree(int** tree, int n);

int hasChildren(int** tree, int n, int i);

void drawTree(int** tree, int n, int start_v, int startX, int startY, HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    WNDCLASS w;
    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    if (!RegisterClass(&w))
        return 0;
    HWND hWnd;
    MSG lpMsg;


    hWnd = CreateWindow(ProgName,
                        "Лабораторна робота 3. Виконала Бондар Антоніна",
                        WS_OVERLAPPEDWINDOW,
                        xWindowCorner,
                        yWindowCorner,
                        windowWidth,
                        windowWidth / 2,
                        (HWND) NULL,
                        (HMENU) NULL,
                        (HINSTANCE) hInstance,
                        (HINSTANCE) NULL);
    ShowWindow(hWnd, nCmdShow);

    HWND button1 = CreateWindow("BUTTON",
                                "BFS start",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                700, 50, 200, 50,
                                hWnd, (HMENU)1, NULL, NULL);
    HWND button2 = CreateWindow("BUTTON",
                                "BFS next step",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                700, 150, 200, 50,
                                hWnd, (HMENU)2, NULL, NULL);
    HWND button3 = CreateWindow("BUTTON",
                                "DFS start",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                700, 250, 200, 50,
                                hWnd, (HMENU)3, NULL, NULL);
    HWND button4 = CreateWindow("BUTTON",
                                "DFS next step",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                700, 350, 200, 50,
                                hWnd, (HMENU)4, NULL, NULL);

    while (GetMessage(&lpMsg, hWnd, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (messg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1:
                    flag1 = 1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case 2:
                    flag1 = 2;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;

                case 3:
                    flag1 = 3;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;

                case 4:
                    flag1 = 4;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                default:
                    printf("other\n");
                    UpdateWindow(hWnd);
                    break;
            }
            break;
        }
        case WM_PAINT :
            hdc = BeginPaint(hWnd, &ps);
            if (T == NULL){
                T = randm(n, n);
            }
            if (A == NULL){
                A = mulmr(k, T, n, n);
            }
            if (symMatrix == NULL){
                symMatrix = undirectMatrix(mulmr(k, T, n, n), n);
            }

            calcCoordsDirect(windowWidth, windowHeight, n, nxD, nyD);

            HPEN BPen = CreatePen(PS_SOLID, 3, RGB(124, 67, 109));
            HPEN KPen = CreatePen(PS_SOLID, 1, RGB(116, 31, 78));
            HPEN GPen = CreatePen(PS_SOLID, 3, RGB(51, 255, 153));

            SelectObject(hdc, GPen);
            if (flag1 == 1){

                NewMatrix = (int**)malloc(n * sizeof(int*));
                for (int i = 0; i < n; i++) {
                    NewMatrix[i] = (int*)malloc(n * sizeof(int));
                    for (int j = 0; j < n; j++){
                        NewMatrix[i][j] = 0;
                    }
                }
                NewMatrix[0][1] = 1;
                NewMatrix[0][2] = 1;

                NewMatrix[1][0] = 1;
                NewMatrix[1][2] = 1;
                NewMatrix[1][3] = 1;
                NewMatrix[1][4] = 1;

                NewMatrix[2][0] = 1;
                NewMatrix[2][1] = 1;
                NewMatrix[2][4] = 1;
                NewMatrix[2][5] = 1;

                NewMatrix[3][1] = 1;
                NewMatrix[3][4] = 1;
                NewMatrix[3][7] = 1;

                NewMatrix[4][1] = 1;
                NewMatrix[4][2] = 1;
                NewMatrix[4][3] = 1;
                NewMatrix[4][7] = 1;

                NewMatrix[5][2] = 1;
                NewMatrix[5][6] = 1;
                NewMatrix[5][7] = 1;

                NewMatrix[6][5] = 1;
                NewMatrix[6][8] = 1;
                NewMatrix[6][9] = 1;

                NewMatrix[7][3] = 1;
                NewMatrix[7][4] = 1;
                NewMatrix[7][5] = 1;

                NewMatrix[8][6] = 1;
                NewMatrix[8][9] = 1;

                NewMatrix[9][6] = 1;
                NewMatrix[9][8] = 1;


                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
                startBfs(NewMatrix, 1, &bfsQ, &bfsVisited,  &bfsTree, hdc);
            }
            if (flag1 == 2){
                bfs_next(NewMatrix, bfsQ, bfsVisited, bfsTree, hdc);
                flag1 = 0;
            }
            if (flag1 == 3){

                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
                start_dfs(NewMatrix, 1,&dfsStack, &dfsVisited, &dfsTree, hdc, n);
            }
            if (flag1 == 4){
                dfs_next(NewMatrix,  dfsStack, dfsVisited, dfsTree, hdc, n);
                flag1 = 0;
            }

            drawDirectGraph(hdc, nxD, nyD, A, vertexRadGraph, KPen);

            SelectObject(hdc, BPen);
            // draw directed graph vertices
            for (int i = 0; i < n; i++) {
                Ellipse(hdc, nxD[i] - vertexRadGraph, nyD[i] - vertexRadGraph, nxD[i] + vertexRadGraph, nyD[i] + vertexRadGraph);
                TextOut(hdc, nxD[i] - dtx, nyD[i] - vertexRadGraph / 2, nn[i], 2);
            }
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            freeMatrixFloat(T, n);
            freeMatrixInt(A, n);
            freeMatrixInt(symMatrix, n);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }

}



float** randm(int rows, int cols) {
    printf("\nPrinting matrix after randm:\n");
    int i,j;
    float** matrix = (float**)malloc(sizeof(float*) * rows );
    srand(2101);
    for (i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(sizeof(float)* cols);
        for (j = 0; j < cols; j++) {
            matrix[i][j] = ((float)rand()/RAND_MAX)*(float)(2);
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
    return matrix;
}

int** mulmr(float coef, float** matrix, int rows, int cols){
    printf("\nPrinting matrix after mulmr (directed matrix):\n");
    int** matrixMul = (int**)malloc(sizeof(int*) * rows );
    for (int i = 0; i < rows; i++) {
        matrixMul[i] = (int*)malloc(sizeof(int)* cols);
        for (int j = 0; j < cols; j++) {
            matrixMul[i][j] = (int)floor(matrix[i][j] * coef);
            printf("%d\t", matrixMul[i][j]);
        }
        printf("\n");
    }
    return matrixMul;
};

int** undirectMatrix (int** matrix, int n){
    printf("\nPrinting symmetric matrix :\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i]){
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    return matrix;
}

void arrow(HDC hdc, float fi, int px,int py){
    float angleRad = fi * M_PI / 180.0;

    int arrowLength = floor((windowWidth + windowHeight)/120);  // Length of the arrowhead lines

    // Calculate the coordinates of the arrowhead points
    int lx = px - arrowLength * cos(angleRad + 0.3);
    int ly = py - arrowLength * sin(angleRad + 0.3);
    int rx = px - arrowLength * cos(angleRad - 0.3);
    int ry = py - arrowLength * sin(angleRad - 0.3);

    // Draw the arrowhead
    MoveToEx(hdc, lx, ly, NULL);
    LineTo(hdc, px, py);
    LineTo(hdc, rx, ry);
}

void drawArrow(HDC hdc, int xStart, int yStart, int xEnd, int yEnd, int vertexRad){
    float dx = xEnd - xStart;
    float dy = yEnd - yStart;
    float angleRad = atan2(dy, dx);
    float angleDeg = angleRad * 180.0 / M_PI;

    float pointX = xEnd - vertexRad * cos(angleRad);
    float pointY = yEnd - vertexRad * sin(angleRad);
    arrow(hdc, angleDeg, pointX, pointY);
}

void calcCoordsDirect(int xWidth, int yHeight, int n, float* nx, float* ny){
    float rad = (yHeight - 200)/2;
    float x0 = xWidth/4;
    float y0 = 50 + rad;
    float angle = 2 * M_PI /n;
    for (int i = 0; i<n; i++){
        float currentAngle = i * angle;
        nx[i] = x0 + rad * cos(currentAngle);
        ny[i] = y0 + rad * sin(currentAngle);
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


void drawDirectGraph(HDC hdc, float* xCoords, float* yCoords, int** matrix, int vertexRad, HPEN pen){
    SelectObject(hdc, pen);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                MoveToEx(hdc, xCoords[i], yCoords[i], NULL);
                if (i == j) {
                    if (i <= 3 || i >= 9) {  // checks if the vertex is on the right side of the circle to draw the ellipse on the outer side
                        Ellipse(hdc, xCoords[i], yCoords[i] , xCoords[i] + 2 * vertexRad, yCoords[i] + 2*vertexRad);
                        arrow(hdc, 255, xCoords[i], yCoords[i]+vertexRad);
                    } else { // left side
                        Ellipse(hdc, xCoords[i] - 2 * vertexRad, yCoords[i] - 2*vertexRad, xCoords[i], yCoords[i]);
                        arrow(hdc, 75, xCoords[i], yCoords[i]-vertexRad);
                    }
                } else {

                    if(matrix[j][i] == 1 && i <= j){ // checks if the line is two-sided to draw a curved line
                        int coefx = (windowWidth + windowHeight)/40; // calculates proportionally to window size, will be used to draw curved lines
                        int coefy = (windowWidth + windowHeight)/90;

                        int middleX = floor((xCoords[i]+xCoords[j])/2);
                        int middleY = floor((yCoords[i]+yCoords[j])/2);
                        // checks if the x coordinate of the line center is in the right or left half so the curve is inwards
                        if (middleX <= windowWidth/4) { // left  half
                            MoveToEx(hdc, xCoords[j], yCoords[j], NULL);
                            LineTo(hdc, middleX + coefx, middleY + coefy);
                            LineTo(hdc, xCoords[i], yCoords[i]);
                            int startX = middleX + coefx;
                            int startY = middleY + coefy;
                            drawArrow(hdc, startX, startY, xCoords[j], yCoords[j], vertexRad);
                        } else {  // right half
                            MoveToEx(hdc, xCoords[j], yCoords[j], NULL);
                            LineTo(hdc, middleX - coefx, middleY + coefy);
                            LineTo(hdc, xCoords[i], yCoords[i]);
                            int startX = middleX - coefx;
                            int startY = middleY + coefy;
                            drawArrow(hdc, startX, startY, xCoords[j], yCoords[j], vertexRad);
                        }
                    } else {
                        MoveToEx(hdc, xCoords[i], yCoords[i], NULL);
                        LineTo(hdc, xCoords[j], yCoords[j]);  // Draw the line segment
                        drawArrow(hdc, xCoords[i], yCoords[i], xCoords[j], yCoords[j], vertexRad);
                    }
                }
            }
        }
    }
}

queue* createQueue(){
    queue* q = malloc(sizeof(queue));
    q -> front = -1;
    q -> rear = -1;
    return q;
}

int isEmpty (queue* q){
    if (q->rear == -1){
        return 1;
    } else {
        return 0;
    }
}

void enqueue (queue* q, int value){
  if (q->rear == N-1){
      printf("\n Queue is full:(\n");
  }  else {
      if (q->front == -1){
          q->front = 0;
      }
      q->rear++;
      q->items[q->rear] = value;
  }
}

int dequeue(struct queue* q) {
    int item;
    if (isEmpty(q)) {
        printf("Queue is empty");
        item = -1;
    } else {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear) {
            printf("\nResetting queue \n");
            q->front = q->rear = -1;
        }
    }
    return item;
}

void printQueue(struct queue* q) {
    int i = q->front;

    if (isEmpty(q)) {
        printf("Queue is empty");
    } else {
        printf("\nQueue contains: ");
        for (i = q->front; i < q->rear + 1; i++) {
            printf("%d ", q->items[i]);
        }
    }
}

void startBfs(int** matrix, int start_v, queue** q, int** visited, int*** tree, HDC hdc) {
    bfsNumber = 1;
    *q = createQueue();
    *visited = (int*)malloc(n * sizeof(int));
    *tree = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        (*visited)[i] = 0;
        (*tree)[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            (*tree)[i][j] = 0;  // Initialize the tree matrix as empty
        }
    }

    (*visited)[start_v - 1] = bfsNumber;
    enqueue(*q, start_v);
    bfsNumber++;
    bfs_next(matrix, *q, *visited, *tree, hdc);
}


void bfs_next(int** matrix, queue* q, int* visited, int** tree, HDC hdc) {
    if (!isEmpty(q)) {

        int currentVertex = dequeue(q);

        draw_vertex(hdc, currentVertex-1);

        printf("\ncurrent: %d\n", currentVertex);

        char buffer[20];
        sprintf(buffer, "Current: %d", currentVertex);
        TextOut(hdc, 10, 10, buffer, strlen(buffer));


        for (int i = 0; i < n; i++) {
            if (matrix[currentVertex - 1][i] == 1 && visited[i] == 0) {
                draw_line(hdc, currentVertex-1, i);
                visited[i] = bfsNumber;
                bfsNumber++;
                tree[currentVertex - 1][i] = 1;
                enqueue(q, i + 1);
                printQueue(q);
                textOutQueue(hdc, q, n);

            }
        }
    } else {
        TextOut(hdc, 10, 60, "Queue is empty!", 15);
        printf("\nBFS Tree (Adjacency Matrix):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", tree[i][j]);
            }
            printf("\n");
        }
        printTree(tree, n);

        printf("\nAll visited: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", visited[i]);
        }
        printf("\nNumeration: \n");
        for (int i = 0; i < n; i++){
            printf("Vertex %d - number %d\n", i+1,visited[i]);
        }
        drawTree( tree, n, 0, 1100, 50, hdc);
        free(visited);
        free(q);
        freeMatrixInt(tree, n);
    }
}

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(capacity * sizeof(int));
    return stack;
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int value) {
    stack->array[++stack->top] = value;
}

int pop(Stack* stack) {
    if (isStackEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}

int peek(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Error: Stack is empty.\n");
        return -1; // or any appropriate error value
    }
    return stack->array[stack->top];
}

void printStack(Stack* s) {
    if (isStackEmpty(s)) {
        printf("Stack is empty\n");
        return;
    }

    printf("\nStack: ");
    for (int i = 0; i <= s->top; i++) {
        printf("%d ", s->array[i]);
    }
    printf("\n");
}
void start_dfs(int** matrix, int start_v, Stack** s, int** visited, int*** tree, HDC hdc, int n) {

    dfsNumber = 1;
    *s = createStack(n);
    *visited = (int*)malloc(n * sizeof(int));
    *tree = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        (*visited)[i] = 0;
        (*tree)[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            (*tree)[i][j] = 0;  // Initialize the tree matrix as empty
        }
    }

    (*visited)[start_v - 1] = dfsNumber;
    push(*s, start_v);
    dfsNumber++;
    dfs_next(matrix, *s, *visited, *tree, hdc, n);
}

void dfs_next(int** matrix, Stack* s, int* visited, int** tree, HDC hdc, int n) {
    if (!isStackEmpty(s)) {
        printStack(s);
        textOutStack(hdc, s, n);
        int currentVertex = peek(s);
        if (visited[currentVertex - 1] == 0) {
            visited[currentVertex - 1] = dfsNumber;
            dfsNumber++;
        }
        draw_vertex(hdc, currentVertex - 1);
        printf("Visited %d\n", currentVertex);

        char currentVertexStr[10] = "Current: ";
        char currentVertexS[5];
        sprintf(currentVertexS, "%d  ", currentVertex);
        strcat(currentVertexStr, currentVertexS);
        TextOut(hdc, 10, 10, currentVertexStr, strlen(currentVertexStr));

        int hasVertex = 0;
        int nextVertex = -1;
        for (int i = 0; i < n; i++) {
            if (matrix[currentVertex - 1][i] == 1 && visited[i] == 0) {
                draw_line(hdc, currentVertex-1, i);
                nextVertex = i+1;
                hasVertex = 1;
                push(s,nextVertex);
                tree[currentVertex - 1][nextVertex - 1] = 1;
                break;
            }
        }
        if (!hasVertex){
            pop(s);
        }

    } else {
        TextOut(hdc, 10, 60, "Stack is empty!", 15);
        printf("\nDFS Tree (Adjacency Matrix):\n");
        for (int i =0; i< n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d\t", tree[i][j]);
            }
            printf("\n");
        }
        printTree(tree, n);

        printf("\nAll visited: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", visited[i]);
        }
        printf("\nNumeration: \n");
        for (int i = 0; i < n; i++){
            printf("Vertex %d - number %d\n", i+1,visited[i]);
        }
        drawTree( tree, n, 0, 1100, 50, hdc);
        free(visited);
        free(s);
        freeMatrixInt(tree, n);
    }
}

void draw_vertex(HDC hdc, int i){
    Ellipse(hdc, nxD[i] - vertexRadGraph - 2, nyD[i] - vertexRadGraph - 2, nxD[i] + vertexRadGraph + 2, nyD[i] + vertexRadGraph + 2);
    TextOut(hdc, nxD[i] - dtx, nyD[i] - vertexRadGraph / 2, nn[i], 2);
}

void draw_line (HDC hdc, int i, int j){
    MoveToEx(hdc, nxD[i], nyD[i], NULL);
    LineTo(hdc, nxD[j], nyD[j]);  // Draw the line segment
    drawArrow(hdc, nxD[i], nyD[i], nxD[j], nyD[j], vertexRadGraph);
}


void printTree(int** tree, int n) {
    printf("Tree Representation:\n");
    for (int i = 0; i < n; i++) {
        printf("Node %d:", i+1);
        for (int j = 0; j < n; j++) {
            if ((tree)[i][j] == 1) {
                printf(" %d", j+1);
            }
        }
        printf("\n");
    }
}

void textOutStack(HDC hdc, Stack* s, int n) {
    if (isStackEmpty(s)) {
        printf("Stack is empty\n");
        return;
    }
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = rect.left + 200; // Adjust the width as needed
    rect.bottom = rect.top + 50; // Adjust the height as needed
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
    char stack[50] = "Stack: ";
    for (int i = 0; i <= s->top; i++) {
        char vertex[5];
        sprintf(vertex, " %d", s->array[i]);
        strcat(stack, vertex);
    }
    TextOut(hdc, 10, 30, stack, strlen(stack));
}

void textOutQueue(HDC hdc, queue* q, int n) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = rect.left + 200; // Adjust the width as needed
    rect.bottom = rect.top + 50; // Adjust the height as needed
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    char queue[50] = "Queue: ";
    int index = q->front;
    while (index != q->rear) {
        char element[5];
        sprintf(element, " %d", q->items[index]);
        strcat(queue, element);
        index = (index + 1) % N;
    }
    char element[5];
    sprintf(element, " %d", q->items[index]);
    strcat(queue, element);

    TextOut(hdc, 10, 30, queue, strlen(queue));
}

int hasChildren(int** tree, int parent, int n){
    int hasChild = 0;
    for (int i = 0; i < n; i++){
        if (tree[parent][i] == 1){
            hasChild = 1;
        }
    }
    return hasChild;
}

void drawTree(int** tree, int n, int start_v, int startX, int startY, HDC hdc){
    int parent = start_v;
    int parentX = startX;
    int parentY = startY;
    int vertexRad = 20;
    int parentWidth = 2 * vertexRad + 4; // Width of the parent node
    int childSpacing = 50; // Horizontal spacing between child nodes

    Ellipse(hdc, parentX - vertexRad - 2, parentY - vertexRad - 2, parentX + vertexRad + 2, parentY + vertexRad + 2);
    char vertexName[2];
    sprintf(vertexName, "%d", parent+1);
    TextOut(hdc, parentX , parentY - 10, vertexName, strlen(vertexName));

    printf("\n\nNode: %d; Coords: x = %d, y = %d\n", parent, parentX, parentY);

    int childCount = 0;
    for (int i = 0; i < n; i++) {
        if (tree[parent][i] == 1) {
            if (hasChildren(tree, i, n)) {
                int childX = parentX - parentWidth / 2 + childCount * childSpacing;
                int childY = parentY + 70;

                MoveToEx(hdc, parentX, parentY + vertexRad + 2, NULL);
                LineTo(hdc, childX, childY - vertexRad - 2);

                drawTree(tree, n, i, childX, childY, hdc);
            } else {
                int childX = parentX - parentWidth / 2 + childCount * childSpacing;
                int childY = parentY + 70;

                Ellipse(hdc, childX - vertexRad - 2, childY - vertexRad - 2, childX + vertexRad + 2, childY + vertexRad + 2);
                sprintf(vertexName, "%d", i+1);
                TextOut(hdc, childX , childY - 10, vertexName, strlen(vertexName));

                MoveToEx(hdc, parentX, parentY + vertexRad + 2, NULL);
                LineTo(hdc, childX, childY - vertexRad - 2);

                printf("\nNode: %d; Coords: x = %d, y = %d\n", i, childX, childY);
            }
            childCount++;
        }
    }
}









