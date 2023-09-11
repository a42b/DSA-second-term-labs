#include <stdio.h>
#include <windows.h>
#include <math.h>
// n1=2, n2=1, n3=0, n4=1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[]="Лабораторна робота 3";
int n = 10;
//float k1 = 1.0 - 0*0.01 - 1*0.01 - 0.3;
float k2 = 1.0 - 0*0.005 - 1*0.005 - 0.27;

float k1 = 0.63;// (coef for testing purposes, shows off other possible cases)

float** T = NULL;
int** A1 = NULL;
int** symMatrix1 = NULL;
int** A2 = NULL;
int** symMatrix2 = NULL;

int** matrixSquare1 = NULL;
int** matrixSquare2 = NULL;
int** matrixCube1 = NULL;
int** matrixCube2 = NULL;

int** matrixReachability1 = NULL;
int** matrixReachability2 = NULL;
int** stronglyConnected1 = NULL;
int** stronglyConnected2 = NULL;

int** components1 = NULL;
int** components2 = NULL;
int** condensationGraph1 = NULL;
int** condensationGraph2 = NULL;

char nn[10][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; //names of vertex
float nxD[10];   // x coordinates for directed graph
float nyD[10];   // y coordinates for directed graph
float nxUn[10];   // x coordinates for undirected graph
float nyUn[10];   // y coordinates for undirected graph

float nxCon1[10];
float nyCon1[10];
float nxCon2[10];
float nyCon2[10];

int matrixFlag = 0;
int infoFlag = 0;
int isolatedVertex1 [10];
int hangingVertex1 [10];
int condensationGraphSize1 = 0;
int condensationGraphSize2 = 0;

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

//calculates vertices coords for undirected graph based on window size
void calcCoordsUndirect(int xWidth, int yHeight, int n, float* nx, float* ny);

void calcCoordsCond(int xWidth, int yHeight, int n, float* nx, float* ny);

void drawUndirectGraph(HDC hdc, float* nxUn, float* nyUn, int** matrix, int verticeRad, HPEN pen);

void drawDirectGraph(HDC hdc, float* xCoords, float* yCoords, int** matrix, int vertexRad, HPEN pen, int n);

//calculates and texts out powers of undirected graph vertices
void undirectVertexDeg ( HDC hdc, int** matrix, int n);

//calculates and texts out powers in and out of directed graph vertices
void directVertexDeg (HDC hdc, int** matrix, int n);

// texts out if graph has isolated or hanging vertexes
void isolatedVertex(HDC hdc, int** symMatrix, int n, int* isolatedArray, int* hangingArray);

//allocates memory for a new matrix result of multiplying
int** matrixMultiply (int** matrix1, int** matrix2, int n);

void pathTwo (HDC hdc, int** matrixSquare, int** matrix, int n);

void pathThree(HDC hdc, int** matrixCube, int** matrix, int n);

int** copyMatrix (int** matrix, int n);

int** identityMatrix(int n);

//adds second matrix to the first one, modifies the first one
void addMatrix (int** matrix1, int** matrix2, int n);

// returns matrix of stong connections
int** matrixStronglyConnected(int** reachMatrix, int n);

//depth first search used in finding strongly connected components
void dfs(int** reachMatrix, int* visited, int n, int v, int* component);

//returns matrix n*n of components
int** findStronglyConnectedComponents(int** reachMatrix, int n);

//returns matrix of a condensation graph
int** CondensationGraphMatrix(int** stronglyConnectedComponents,int** originalGraph, int componentCount, int originalGraphSize);

void printMatrix (int** matrix, int n);

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

    HWND button1 = CreateWindow("BUTTON",
                                "Перший граф",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                490, 10, 200, 30,
                                hWnd, (HMENU)1, NULL, NULL);
    HWND button2 = CreateWindow("BUTTON",
                                "Другий граф",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                720, 10, 200, 30,
                                hWnd, (HMENU)2, NULL, NULL);
    HWND button3 = CreateWindow("BUTTON",
                                "Степені графа",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                450, 50, 160, 30,
                                hWnd, (HMENU)3, NULL, NULL);
    HWND button4 = CreateWindow("BUTTON",
                                "Шляхи довжиною 2",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                610, 50, 160, 30,
                                hWnd, (HMENU)4, NULL, NULL);
    HWND button5 = CreateWindow("BUTTON",
                                "Шляхи довжиною 3",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                770, 50, 160, 30,
                                hWnd, (HMENU)5, NULL, NULL);
    HWND button6 = CreateWindow("BUTTON",
                                "Матриця досяжності",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                450, 80, 160, 30,
                                hWnd, (HMENU)6, NULL, NULL);
    HWND button7 = CreateWindow("BUTTON",
                                "Матриця cильної зв'язаності",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                610, 80, 160, 30,
                                hWnd, (HMENU)7, NULL, NULL);
    HWND button8 = CreateWindow("BUTTON",
                                "Компоненти",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                770, 80, 160, 30,
                                hWnd, (HMENU)8, NULL, NULL);
    HWND button9 = CreateWindow("BUTTON",
                                "Граф",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                610, 110, 160, 30,
                                hWnd, (HMENU)9, NULL, NULL);
    ShowWindow(hWnd, nCmdShow);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch(messg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1:
                    matrixFlag = 1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;
                case 2:

                    matrixFlag = 2;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;

                case 3:

                    infoFlag = 1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;

                case 4:
                    infoFlag = 2;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;
                case 5:
                    infoFlag = 3;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;
                case 6:
                    infoFlag = 4;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;

                case 7:
                    infoFlag = 5;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;

                case 8:
                    infoFlag = 6;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    break;
                case 9:
                    infoFlag = 7;
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
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
            if (T==NULL || A1==NULL || A2 == NULL || symMatrix1==NULL || symMatrix2 == NULL){
                printf("\nPrinting random matrix T: \n");
                T = randm(n, n);

                printf("\nPrinting matrix A1: \n");
                A1 = mulmr(k1, T, n, n);

                printf("\nPrinting symmetric matrix 1: \n");
                symMatrix1 = undirectMatrix(mulmr(k1, T, n, n), n);// first prints the same matrix as A1, then transforms it to symmetric matrix and prints

                printf("\nPrinting matrix A2: \n");
                A2 = mulmr(k2, T, n, n);

                printf("\nPrinting symmetric matrix 2: \n");
                symMatrix2 = undirectMatrix(mulmr(k2, T, n, n), n);// first prints the same matrix as A2, then transforms it to symmetric matrix and prints
            }

            calcCoordsDirect(windowWidth, windowHeight, n, nxD, nyD);
            calcCoordsUndirect(windowWidth, windowHeight, n, nxUn, nyUn);

            int vertexRad = floor(windowHeight / 25); // calculates radius proportionally to window size
            int dtx = floor(windowWidth / 240); //

            HPEN BPen = CreatePen(PS_SOLID, 3, RGB(124, 67, 109));
            HPEN KPen = CreatePen(PS_SOLID, 1, RGB(116, 31, 78));

            if (matrixFlag == 1) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                FillRect(hdc, &rect, (HBRUSH) (COLOR_WINDOW + 1));
                TextOut(hdc, 650, 150, "Перший граф", 22);
                drawUndirectGraph(hdc, nxUn, nyUn, symMatrix1, vertexRad, KPen);
                drawDirectGraph(hdc, nxD, nyD, A1, vertexRad, KPen, n);
            } else {
                if (matrixFlag == 2) {
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    FillRect(hdc, &rect, (HBRUSH) (COLOR_WINDOW + 1));
                    TextOut(hdc, 650, 150, "Другий граф", 22);
                    drawUndirectGraph(hdc, nxUn, nyUn, symMatrix2, vertexRad, KPen);
                    drawDirectGraph(hdc, nxD, nyD, A2, vertexRad, KPen, n);
                }
            }

            SelectObject(hdc, BPen);
//draw directed graph vertices
            for (int i = 0; i < n; i++) {
                Ellipse(hdc, nxD[i] - vertexRad, nyD[i] - vertexRad, nxD[i] + vertexRad, nyD[i] + vertexRad);
                TextOut(hdc, nxD[i] - dtx, nyD[i] - vertexRad / 2, nn[i], 2);
            }
//draw undirected vertices
            for (int i = 0; i < n; i++) {
                Ellipse(hdc, nxUn[i] - vertexRad, nyUn[i] - vertexRad, nxUn[i] + vertexRad, nyUn[i] + vertexRad);
                TextOut(hdc, nxUn[i] - dtx, nyUn[i] - vertexRad / 2, nn[i], 2);
            }

            if (infoFlag == 1) {
                if (matrixFlag == 1){
                    printf("\nСтепені вершин ненапрямленого графа 1: \n");
                    undirectVertexDeg(hdc, symMatrix1, n);
                    printf("\nПівстепені вершин напрямленого графа 1: \n");
                    directVertexDeg(hdc, A1, n);
                    isolatedVertex(hdc, symMatrix1, n, isolatedVertex1, hangingVertex1);
                }
                if (matrixFlag == 2){
                    printf("\nСтепені вершин ненапрямленого графа 2: \n");
                    undirectVertexDeg(hdc, symMatrix2, n);
                    printf("\nПівстепені вершин напрямленого графа 2: \n");
                    directVertexDeg(hdc, A2, n);
                    isolatedVertex(hdc, symMatrix2, n, isolatedVertex1, hangingVertex1);
                }
            }

            if (infoFlag == 2) {
                if (matrixFlag == 1){
                    if ( matrixSquare1 == NULL){
                        printf("\nPrinting square matrix 1\n");
                        matrixSquare1 = matrixMultiply(A1, A1, n);
                        printMatrix(matrixSquare1, n);
                    }
                    TextOut(hdc, 600, 300, "Full printed in console", 25);
                    printf("\nШляхи довжиною 2 граф 1:\n");
                    pathTwo(hdc, matrixSquare1, A1, n);
                }
                if (matrixFlag == 2){
                    if ( matrixSquare2 == NULL){
                        printf("\nPrinting square matrix 2\n");
                        matrixSquare2 = matrixMultiply(A2, A2, n);
                        printMatrix(matrixSquare2, n);
                    }
                    TextOut(hdc, 600, 300, "Full printed in console", 25);
                    printf("\nШляхи довжиною 2 граф 2:\n");
                    pathTwo(hdc, matrixSquare2, A2, n);
                }
            }

            if (infoFlag == 3) {
                if (matrixFlag == 1){
                    if (matrixCube1 == NULL){
                        if (matrixSquare1 == NULL){
                            printf("\nPrinting square matrix 1:\n");
                            matrixSquare1 = matrixMultiply(A1, A1, n);
                            printMatrix(matrixSquare1, n);
                        }
                        printf("\nPrinting matrix cube 1:\n");
                        matrixCube1 = matrixMultiply(matrixSquare1, A1, n);
                        printMatrix(matrixCube1, n);
                    }
                    TextOut(hdc, 600, 300, "Full printed in console", 25);
                    printf("\nШляхи довжиною 3 граф 1:\n");
                    pathThree(hdc, matrixCube1, A1, n);
                }
                if (matrixFlag == 2){
                    if (matrixCube2 == NULL){
                        if (matrixSquare2 == NULL){
                            printf("\nPrinting square matrix 2:\n");
                            matrixSquare2 = matrixMultiply(A2, A2, n);
                            printMatrix(matrixSquare2, n);
                        }
                        printf("\nPrinting matrix cube 2:\n");
                        matrixCube2 = matrixMultiply(matrixSquare2, A2, n);
                        printMatrix(matrixCube2, n);
                    }
                    TextOut(hdc, 600, 300, "Full printed in console", 25);
                    printf("\nШляхи довжиною 3 граф 2:\n");
                    pathThree(hdc, matrixCube2, A2, n);
                }
            }

            if (infoFlag == 4) {
                if (matrixFlag == 1){
                    if (matrixReachability1 == NULL){
                        matrixReachability1 = identityMatrix(n);
                        int** next = copyMatrix(A1, n);
                        for (int i = 0; i < n-1; i++){
                            addMatrix(matrixReachability1, next, n);
                            next = matrixMultiply(next, A1, n);
                        }
                    }
                    printf("\nМатриця досяжності 1: \n");
                    for (int i = 0; i < n; i++){
                        for (int j = 0; j < n; j++){
                            matrixReachability1[i][j] = (matrixReachability1[i][j] != 0);
                        }
                    }
                    TextOut(hdc, 600, 300, "Printed in console", 22);
                    printMatrix(matrixReachability1, n);
                }
                if (matrixFlag == 2){
                    if (matrixReachability2 == NULL){
                        matrixReachability2 = identityMatrix(n);
                        int** next = copyMatrix(A2, n);
                        for (int i = 0; i < n-1; i++){
                            addMatrix(matrixReachability2, next, n);
                            next = matrixMultiply(next, A2, n);
                        }
                    }
                    printf("\nМатриця досяжності 2: \n");
                    for (int i = 0; i < n; i++){
                        for (int j = 0; j < n; j++){
                            matrixReachability2[i][j] = (matrixReachability2[i][j] != 0);
                        }
                    }
                    TextOut(hdc, 600, 300, "Printed in console", 22);
                    printMatrix(matrixReachability2, n);
                }
            }
            if (infoFlag == 5) {
                if (matrixFlag == 1){
                    if (matrixReachability1 != NULL){
                        if (stronglyConnected1 == NULL){
                            stronglyConnected1 = matrixStronglyConnected(matrixReachability1, n);
                        }
                        TextOut(hdc, 600, 300, "Printed in console", 22);
                        printf("\nМатриця сильної зв'язаності 1:\n");
                        printMatrix(stronglyConnected1, n);
                    } else {
                        TextOut(hdc, 600, 300, "Need to find reachability matrix first", 40);
                    }

                }
                if (matrixFlag == 2) {
                    if (matrixReachability2 != NULL) {
                        if (stronglyConnected2 == NULL) {
                            stronglyConnected2 = matrixStronglyConnected(matrixReachability2, n);
                        }
                        TextOut(hdc, 600, 300, "Printed in console", 22);
                        printf("\nМатриця сильної зв'язаності 2:\n");
                        printMatrix(stronglyConnected2, n);
                    } else {
                        TextOut(hdc, 600, 300, "Need to find reachability matrix first", 40);
                    }
                }
            }

            if (infoFlag == 6) {
                if (matrixFlag == 1) {
                    if (stronglyConnected1 != NULL) {
                        if (components1 == NULL){
                            components1 = findStronglyConnectedComponents(stronglyConnected1, n);
                            condensationGraphSize1 = 0;
                            TextOut(hdc, 600, 300, "Компоненти сильної зв'язаності 1", 60);
                            for (int i = 0; i < n; i++){
                                char message[100] = "Component ";
                                char component[4];

                                int hasVertices = 0;
                                sprintf(component, " %d: ", i+1);
                                strcat(message, component);
                                for (int j = 0; j < n; j++) {
                                    int vertex = components1[i][j];
                                    if (vertex == 1){
                                        char elementString[4];
                                        sprintf(elementString, " %d,", j+1);
                                        strcat(message, elementString);
                                        hasVertices = 1;
                                    }

                                }
                                if (hasVertices){
                                    condensationGraphSize1++;
                                    TextOut(hdc, 650, 400 + i*20, message, strlen(message));
                                }
                            }

                        }
                        printf("\nКомпоненти сильної зв'язності 1: \n");
                        printMatrix(components1, n);
                    } else {
                        TextOut(hdc, 600, 300, "Need to find strong connection matrix first", 45);
                    }
                }
                if (matrixFlag == 2) {
                    if (stronglyConnected2 != NULL) {
                        if (components2 == NULL){
                            components2 = findStronglyConnectedComponents(stronglyConnected2, n);
                            condensationGraphSize2 = 0;
                            TextOut(hdc, 600, 300, "Компоненти сильної зв'язаності 2", 60);
                            for (int i = 0; i < n; i++){
                                char message[100] = "Component ";
                                char component[4];

                                int hasVertices = 0;
                                sprintf(component, " %d: ", i+1);
                                strcat(message, component);
                                for (int j = 0; j < n; j++) {
                                    int vertex = components2[i][j];
                                    if (vertex == 1){
                                        char elementString[4];
                                        sprintf(elementString, " %d,", j+1);
                                        strcat(message, elementString);
                                        hasVertices = 1;
                                    }
                                }
                                if (hasVertices){
                                    condensationGraphSize2++;
                                    TextOut(hdc, 650, 400 + i*20, message, strlen(message));
                                }
                            }
                        }
                        printf("Компоненти сильної зв'язності 2: \n");
                        printMatrix(components2, n);
                    } else {
                        TextOut(hdc, 600, 300, "Need to find strong connection matrix first", 45);
                    }
                }
            }

            if (infoFlag == 7) {

                if (matrixFlag == 1){
                    if (components1 != NULL){
                        if (condensationGraph1 == NULL){
                            condensationGraph1 = CondensationGraphMatrix(components1, A1, condensationGraphSize1, n);
                        }
                        printf("Матриця графа конденсації 1: \n");
                        printMatrix(condensationGraph1, condensationGraphSize1);

                        calcCoordsCond(windowWidth, windowHeight, condensationGraphSize1, nxCon1, nyCon1);
                        drawDirectGraph(hdc, nxCon1, nyCon1, condensationGraph1, 20, KPen, condensationGraphSize1);

                        TextOut(hdc, 600, 250, "Граф конденсації 1", 32);
                        TextOut(hdc, 600, 300, "Matrix printed in console", 25);
                        for (int i = 0; i < condensationGraphSize1; i++) {
                            Ellipse(hdc, nxCon1[i] - vertexRad, nyCon1[i] - vertexRad, nxCon1[i] + vertexRad, nyCon1[i] + vertexRad);
                            TextOut(hdc, nxCon1[i] - dtx, nyCon1[i] - vertexRad / 2, nn[i], 2);
                        }

                    }else {
                        TextOut(hdc, 600, 300, "Need to find components first", 30);
                    }
                }
                if (matrixFlag == 2){
                    if (components2 != NULL){
                        if (condensationGraph2 == NULL){
                            condensationGraph2 = CondensationGraphMatrix(components2, A2, condensationGraphSize2, n);

                        }
                        printf("Матриця графа конденсації  2: \n");
                        printMatrix(condensationGraph2, condensationGraphSize2);

                        calcCoordsCond(windowWidth, windowHeight, condensationGraphSize2, nxCon2, nyCon2);
                        drawDirectGraph(hdc, nxCon2, nyCon2, condensationGraph2, 20, KPen, condensationGraphSize2);


                        TextOut(hdc, 600, 250, "Граф конденсації 2", 35);
                        TextOut(hdc, 600, 300, "Matrix printed in console", 25);
                        for (int i = 0; i < condensationGraphSize2; i++) {
                            Ellipse(hdc, nxCon2[i] - vertexRad, nyCon2[i] - vertexRad, nxCon2[i] + vertexRad, nyCon2[i] + vertexRad);
                            TextOut(hdc, nxCon2[i] - dtx, nyCon2[i] - vertexRad / 2, nn[i], 2);
                        }

                    }else {
                        TextOut(hdc, 600, 300, "Need to find components first", 30);
                    }
                }
            }
            EndPaint(hWnd, &ps);
            break;


        case WM_DESTROY:
            freeMatrixFloat(T, n);
            freeMatrixInt(A1, n);
            freeMatrixInt(symMatrix1, n);

            freeMatrixInt(A2, n);
            freeMatrixInt(symMatrix2, n);

            freeMatrixInt(matrixSquare1, n);
            freeMatrixInt(matrixSquare2, n);
            freeMatrixInt(matrixCube1, n);
            freeMatrixInt(matrixCube2, n);

            freeMatrixInt(matrixReachability1, n);
            freeMatrixInt(matrixReachability2, n);
            freeMatrixInt(stronglyConnected1, n);
            freeMatrixInt(stronglyConnected2, n);

            freeMatrixInt(components1, n);
            freeMatrixInt(components2, n);
            freeMatrixInt(condensationGraph1, condensationGraphSize1);
            freeMatrixInt(condensationGraph2, condensationGraphSize2);

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
    float rad = (yHeight - 350)/2;
    float x0 = xWidth/5;
    float y0 = 50 + rad;
    float angle = 2 * M_PI /n;
    for (int i = 0; i<n; i++){
        float currentAngle = i * angle;
        nx[i] = x0 + rad * cos(currentAngle);
        ny[i] = y0 + rad * sin(currentAngle);
    }
}

void calcCoordsCond(int xWidth, int yHeight, int n, float* nx, float* ny){
    float rad = (yHeight - 500)/2;
    float x0 = xWidth/2;
    float y0 = 400 + rad;
    float angle = 2 * M_PI /n;
    for (int i = 0; i<n; i++){
        float currentAngle = i * angle;
        nx[i] = x0 + rad * cos(currentAngle);
        ny[i] = y0 + rad * sin(currentAngle);
    }
}

void calcCoordsUndirect(int xWidth, int yHeight, int n, float* nx, float* ny){
    float rad = (yHeight - 350)/2;
    float x0 =  (xWidth/5)*4;
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


void drawUndirectGraph(HDC hdc, float* nxUn, float* nyUn, int** matrix, int verticeRad, HPEN pen){
    SelectObject(hdc, pen);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
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

void drawDirectGraph(HDC hdc, float* xCoords, float* yCoords, int** matrix, int vertexRad, HPEN pen, int n){
    SelectObject(hdc, pen);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                MoveToEx(hdc, xCoords[i], yCoords[i], NULL);
                if (i == j) {
                    if (i <= 2 || i >= 8) {  // checks if the vertex is on the right side of the circle to draw the ellipse on the outer side
                        Ellipse(hdc, xCoords[i], yCoords[i] , xCoords[i] + 2 * vertexRad, yCoords[i] + 2*vertexRad);
                        arrow(hdc, 255, xCoords[i], yCoords[i]+vertexRad);
                    } else { // left side
                        Ellipse(hdc, xCoords[i] - 2 * vertexRad, yCoords[i] - 2*vertexRad, xCoords[i], yCoords[i]);
                        arrow(hdc, 75, xCoords[i], yCoords[i]-vertexRad);
                    }
                } else {

                    if(matrix[j][i] == 1 && i <= j){ // checks if the line is two-sided to draw a curved line
                        int coefx = (windowWidth + windowHeight)/65; // calculates proportionally to window size, will be used to draw curved lines
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

void undirectVertexDeg ( HDC hdc, int** matrix, int n){
    TextOut(hdc, 750, 400, "Степіні вершин ненапрямленого графа:", 68);
    int r = 0;
    int isRegular = 1;
    for (int i = 0; i < n; i++){
        int deg = 0;
        for (int j = 0; j < n; j++){
            if (matrix[i][j] == 1){
                deg++;
            }
        }
        if (i > 0 && deg != r) {
            isRegular = 0;
        }
        r = deg;
        printf("Deg %d: %d\n", i+1, deg);
        char message[50];
        sprintf(message, "Vertex %d deg: %d\n", i+1, deg);

        TextOut(hdc, 850, 420+20*i, message, strlen(message));
    }
    if (isRegular == 1){
        TextOut(hdc, 600, 200, "Граф однорідний", 30);
        char message[50];
        sprintf(message, "\nСтепінь однорідності: %d\n", r);

        TextOut(hdc, 600, 220, message, strlen(message));
    } else {
        TextOut(hdc, 600, 200, "Граф неоднорідний", 40);
    }
}

void directVertexDeg (HDC hdc, int** matrix, int n){
    TextOut(hdc, 470, 400, "Півстепені вершин напрямленого графа:", 70);
    for (int i = 0; i < n; i++){
        int degIn = 0;
        int degOut = 0;
        for (int j = 0; j < n; j++){
            if (matrix[i][j] == 1){
                degOut++;
            }
            if (matrix[j][i] == 1){
                degIn++;
            }
        }
        printf("Deg out %d: %d\n", i+1, degOut);
        char message[50];
        sprintf(message, "Vertex %d in: %d, out: %d\n", i+1, degIn, degOut);

        TextOut(hdc, 520, 420+20*i, message, strlen(message));
    }
}

void isolatedVertex(HDC hdc, int** matrix, int n, int* isolatedArray, int* hangingArray){
    int hasIsolated = 0;
    int hasHanging = 0;
    for (int i = 0; i < n; i++){
        int deg = 0;
        for (int j = 0; j< n; j++){
            if ( matrix[i][j] == 1){
                deg++;
            }
        }
        if (deg == 0){
            isolatedArray[i] = 1;
            hasIsolated = 1;
        }
        if (deg == 1){
            hangingArray[i] = 1;
            hasHanging = 1;
        }
    }
    if (hasIsolated == 0){
        TextOut(hdc, 600, 250, "Граф не має ізольованих вершин", 57);
    } else {
        char message[100] = "Ізольовані вершини: ";
        for (int k = 0;  k < n; k++){
            if (isolatedArray[k] == 1){
                char elementString[4];
                sprintf(elementString, " %d,", k+1);
                strcat(message, elementString);
            }
            TextOut(hdc, 600, 270, message, strlen(message));
        }
    }
    if (hasHanging == 0){
        TextOut(hdc, 600, 300, "Граф не має висячих вершин", 49);
    } else {
        char message[100] = "Висячі вершини: ";
        for (int k = 0;  k < n; k++){
            if (hangingArray[k] == 1){
                char elementString[4];
                sprintf(elementString, " %d,", k+1);
                strcat(message, elementString);
            }
            TextOut(hdc, 600, 300, message, strlen(message));
        }
    }
}

int** matrixMultiply (int** matrix1, int** matrix2, int n){
    int** matrix = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++){
        matrix[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++){
            matrix[i][j] = 0;
            for (int k = 0; k < n; k++){
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return matrix;
}

void pathTwo (HDC hdc, int** matrixSquare, int** matrix, int n){
    TextOut(hdc, 650, 250, "Шляхи довжиною 2:", 30);
    int yPos = 420;
    int xPos = 20;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (matrixSquare[i][j] >= 1){
                for (int k = 0; k < n; k++){
                    if (matrix[i][k] == 1 && matrix[k][j] == 1){
                        printf("%d->%d->%d \t", i+1, k+1, j+1);
                        char message[20];
                        sprintf(message, "%d  ->  %d  ->  %d", i+1, k+1, j+1);

                        TextOut(hdc, xPos, yPos, message, strlen(message));
                        yPos += 20;
                        if (yPos >= windowHeight-60) {
                            yPos = 420;
                            xPos += 120;
                        }
                    }
                }
            }
        }
        printf("\n\n\n");
    }
}

void pathThree(HDC hdc, int** matrixCube, int** matrix, int n) {
    TextOut(hdc, 650, 250, "Шляхи довжиною 3:", 30);
    int yPos = 420;
    int xPos = 20;
    int pathHeight = 20;
    int pathWidth = 140;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrixCube[i][j] >= 1) {
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < n; l++) {
                        if (matrix[i][k] == 1 && matrix[k][l] == 1 && matrix[l][j] == 1) {
                            printf("%d->%d->%d->%d \t", i + 1, k + 1, l + 1, j + 1);
                            char message[20];
                            sprintf(message, "%d  ->  %d  ->  %d -> %d", i + 1, k + 1, l + 1, j + 1);

                            TextOut(hdc, xPos, yPos, message, strlen(message));
                            yPos += 20;
                            if (yPos >= windowHeight - 60) {
                                yPos = 420;
                                xPos += 140;
                            }
                        }
                    }
                }
            }
        }
        printf("\n\n\n");
    }
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

int** identityMatrix(int n){
    int** identMatrix = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        identMatrix[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            if (i == j){
                identMatrix[i][j] = 1;
            } else {
                identMatrix[i][j] = 0;
            }
        }
    }
    return identMatrix;
}


void addMatrix (int** matrix1, int** matrix2, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            matrix1[i][j] += matrix2[i][j];
        }
    }
}

int** matrixStronglyConnected(int** reachMatrix, int n){
    int** reachMatrixT = (int**)malloc(sizeof(int*) * n );
    for (int i = 0; i < n; i++) {
        reachMatrixT[i] = (int*)malloc(sizeof(int)* n);
        for (int j = 0; j < n; j++) {
            reachMatrixT[i][j] = reachMatrix[j][i];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            reachMatrixT[i][j] *= reachMatrix[i][j];
        }
    }
    return reachMatrixT;
}


void dfs(int** reachMatrix, int* visited, int n, int v, int* component) {
    visited[v] = 1;
    component[v] = 1;

    for (int i = 0; i < n; i++) {
        if (reachMatrix[v][i] && !visited[i]) {
            dfs(reachMatrix, visited, n, i, component);
        }
    }
}

int** findStronglyConnectedComponents(int** reachMatrix, int n) {
    int* visited = (int*)malloc(sizeof(int) * n);
    int** components = (int**)malloc(sizeof(int*) * n);
    int count = 0;

    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        components[i] = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) {
            components[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(reachMatrix, visited, n, i, components[count]);
            count++;
        }
    }

    free(visited);
    return components;
}


int** CondensationGraphMatrix(int** stronglyConnectedComponents, int** originalGraph, int componentCount, int originalGraphSize) {
    printf("\nPrinting condensation matrix:\n");
    printf("Condensation matrix size: %d\n", componentCount);

    int** condensationMatrix = (int**)malloc(sizeof(int*) * componentCount);
    for (int i = 0; i < componentCount; i++) {
        condensationMatrix[i] = (int*)malloc(sizeof(int) * componentCount);
        memset(condensationMatrix[i], 0, sizeof(int) * componentCount);
    }

    for (int i = 0; i < componentCount; i++) {
        for (int j = 0; j < componentCount; j++) {
            if (i != j) {
                for (int k = 0; k < originalGraphSize; k++) {
                    if (stronglyConnectedComponents[i][k] == 1) {
                        for (int l = 0; l < originalGraphSize; l++) {
                            if (stronglyConnectedComponents[j][l] == 1 && originalGraph[k][l] == 1) {
                                condensationMatrix[i][j] = 1;
                                break;
                            }
                        }
                        if (condensationMatrix[i][j] == 1) {
                            break;
                        }
                    }
                }
            }
        }
    }
    return condensationMatrix;
}

void printMatrix (int** matrix, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}