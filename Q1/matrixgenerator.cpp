#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// matrix size(change if you want)
int M = 60;
int N = 80;

// Fill background with 0
void clearMatrix(int *a) {
    for(int i = 0; i < M; i++)
        for(int j = 0; j < N; j++)
            a[i*N + j] = 0;
}

// Write matrix to a file: input_tc1.in ... input_tc4.in
void writeToFile(int *a, int tc) {
    string fname = "input_tc" + to_string(tc) + ".in";
    ofstream fout(fname);

    fout << M << " " << N << "\n";

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            fout << a[i*N + j] << " ";
        }
        fout << "\n";
    }

    fout.close();
    cout << fname << " generated"<<endl;
}

// Generate 1 testcase in matrix a
void generateTestCase(int *a, int tc) {

    // Start with clean background
    clearMatrix(a);

    // ============================================================
    // TEST CASE 1: 1 Square + 1 Rectangle + 1 Circle (separated)
    // ============================================================
    if(tc == 1) {
        // Shape 1: FILLED SQUARE
        // top-left (5,5), size 12, intensity 90
        for(int i = 5; i < 5 + 12; i++)
            for(int j = 5; j < 5 + 12; j++)
                a[i*N + j] = 90;

        // Shape 2: FILLED RECTANGLE
        // rows 8..20, cols 30..60, intensity 140
        for(int i = 8; i <= 20; i++)
            for(int j = 30; j <= 60; j++)
                a[i*N + j] = 140;

        // Shape 3: FILLED CIRCLE
        // center (40, 60), radius 8, intensity 180
        int cx = 40, cy = 60, r = 8;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++) {
                int dx = i - cx;
                int dy = j - cy;
                if(dx*dx + dy*dy <= r*r)
                    a[i*N + j] = 180;
            }
    }

    // ============================================================
    // TEST CASE 2: Two Circles (different sizes + intensities)
    // ============================================================
    if(tc == 2) {
        // Shape 1: FILLED CIRCLE (bigger)
        // center (20, 20), radius 10, intensity 120
        int cx1 = 20, cy1 = 20, r1 = 10;

        // Shape 2: FILLED CIRCLE (smaller)
        // center (40, 55), radius 7, intensity 170
        int cx2 = 40, cy2 = 55, r2 = 7;

        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++) {
                int dx1 = i - cx1, dy1 = j - cy1;
                if(dx1*dx1 + dy1*dy1 <= r1*r1)
                    a[i*N + j] = 120;

                int dx2 = i - cx2, dy2 = j - cy2;
                if(dx2*dx2 + dy2*dy2 <= r2*r2)
                    a[i*N + j] = 170;
            }
    }

    // ============================================================
    // TEST CASE 3: Multiple Rectangles (different sizes)
    // ============================================================
    if(tc == 3) {
        // Shape 1: FILLED RECTANGLE (big)
        // rows 10..25, cols 10..45, intensity 100
        for(int i = 10; i <= 25; i++)
            for(int j = 10; j <= 45; j++)
                a[i*N + j] = 100;

        // Shape 2: FILLED RECTANGLE (smaller)
        // rows 35..50, cols 55..75, intensity 150
        for(int i = 35; i <= 50; i++)
            for(int j = 55; j <= 75; j++)
                a[i*N + j] = 150;
    }

    // ============================================================
    // TEST CASE 4: Rectangle and Circle FAR APART (no overlap / no touching)
    // ============================================================
    if(tc == 4) {

        // Shape 1: FILLED RECTANGLE (top-left)
        // rows 8..20, cols 10..35, intensity 80
        for(int i = 8; i <= 20; i++)
            for(int j = 10; j <= 35; j++)
                a[i*N + j] = 80;

        // Shape 2: FILLED CIRCLE (bottom-right)
        // center (40, 65), radius 8, intensity 160
        int cx = 40, cy = 65, r = 8;

        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++) {
                int dx = i - cx;
                int dy = j - cy;
                if(dx*dx + dy*dy <= r*r)
                    a[i*N + j] = 160;
            }
    }
}

int main() {

    // dynamic allocation 
    int *a = new int[M*N];

    for(int tc = 1; tc <= 4; tc++) {
        generateTestCase(a, tc);
        writeToFile(a, tc);
    }

    delete[] a;
    return 0;
}