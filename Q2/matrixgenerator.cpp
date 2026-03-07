#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int M, N;

void clearMatrix(int *a){
    for(int i=0; i<M; i++)
        for(int j=0; j<N; j++)
            a[i*N + j] = 0;
}

void writeToFile(int *a, int tc){
    string fname = "input_tc" + to_string(tc) + ".in";
    ofstream fout(fname);

    fout << M << " " << N << "\n";

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++)
            fout << a[i*N + j] << " ";
        fout << "\n";
    }

    fout.close();
    cout << fname << " generated" << endl;
}

void generateTestCase(int tc){
    int *a = nullptr;

    // ============================================================
    // TEST CASE 1
    // small square + small rectangle + tiny circle
    // matrix 12 x 14
    // ============================================================
    if(tc == 1){
        M = 12;
        N = 14;
        a = new int[M*N];
        clearMatrix(a);

        // square 3x3
        for(int i=1; i<=3; i++)
            for(int j=1; j<=3; j++)
                a[i*N + j] = 1;

        // rectangle 2x4
        for(int i=4; i<=5; i++)
            for(int j=7; j<=10; j++)
                a[i*N + j] = 1;

        // tiny circle-like blob
        int cx = 8, cy = 11, r = 1;
        for(int i=0; i<M; i++)
            for(int j=0; j<N; j++){
                int dx = i - cx;
                int dy = j - cy;
                if(dx*dx + dy*dy <= r*r)
                    a[i*N + j] = 1;
            }
    }

    // ============================================================
    // TEST CASE 2
    // two tiny circles
    // matrix 10 x 12
    // ============================================================
    if(tc == 2){
        M = 10;
        N = 12;
        a = new int[M*N];
        clearMatrix(a);

        int cx1 = 2, cy1 = 3, r1 = 1;
        int cx2 = 7, cy2 = 8, r2 = 1;

        for(int i=0; i<M; i++)
            for(int j=0; j<N; j++){
                int dx1 = i - cx1, dy1 = j - cy1;
                if(dx1*dx1 + dy1*dy1 <= r1*r1)
                    a[i*N + j] = 1;

                int dx2 = i - cx2, dy2 = j - cy2;
                if(dx2*dx2 + dy2*dy2 <= r2*r2)
                    a[i*N + j] = 1;
            }
    }

    // ============================================================
    // TEST CASE 3
    // two small rectangles
    // matrix 10 x 15
    // ============================================================
    if(tc == 3){
        M = 10;
        N = 15;
        a = new int[M*N];
        clearMatrix(a);

        // rectangle 1
        for(int i=1; i<=3; i++)
            for(int j=1; j<=4; j++)
                a[i*N + j] = 1;

        // rectangle 2
        for(int i=5; i<=7; i++)
            for(int j=9; j<=12; j++)
                a[i*N + j] = 1;
    }

    // ============================================================
    // TEST CASE 4
    // hollow square + one tiny rectangle + one isolated pixel
    // matrix 12 x 12
    // ============================================================
    if(tc == 4){
        M = 12;
        N = 12;
        a = new int[M*N];
        clearMatrix(a);

        // hollow square
        for(int j=1; j<=4; j++){
            a[1*N + j] = 1;
            a[4*N + j] = 1;
        }
        for(int i=1; i<=4; i++){
            a[i*N + 1] = 1;
            a[i*N + 4] = 1;
        }

        // small rectangle
        for(int i=7; i<=8; i++)
            for(int j=7; j<=9; j++)
                a[i*N + j] = 1;

        // isolated pixel
        a[10*N + 10] = 1;
    }

    writeToFile(a, tc);
    delete[] a;
}

int main(){
    for(int tc=1; tc<=4; tc++)
        generateTestCase(tc);

    return 0;
}