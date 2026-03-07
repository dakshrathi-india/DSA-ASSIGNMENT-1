#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// reduced matrix size
int M = 30;
int N = 40;

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
    cout << fname << " generated" << endl;
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
        // top-left (3,3), size 6, intensity 90
        for(int i = 3; i < 3 + 6; i++)
            for(int j = 3; j < 3 + 6; j++)
                a[i*N + j] = 90;

        // Shape 2: FILLED RECTANGLE
        // rows 5..11, cols 16..28, intensity 140
        for(int i = 5; i <= 11; i++)
            for(int j = 16; j <= 28; j++)
                a[i*N + j] = 140;

        // Shape 3: FILLED CIRCLE
        // center (21, 30), radius 5, intensity 180
        int cx = 21, cy = 30, r = 5;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++) {
                int dx = i - cx;
                int dy = j - cy;
                if(dx*dx + dy*dy <= r*r)
                    a[i*N + j] = 180;
            }
    }

    // ============================================================
    // TEST CASE 2: Two Circles (same sizes + intensities)
    // ============================================================
    if(tc == 2) {
        // Shape 1: FILLED CIRCLE (bigger)
        // center (10, 10), radius 5, intensity 120
        int cx1 = 10, cy1 = 10, r1 = 5;

        // Shape 2: FILLED CIRCLE (smaller)
        // center (20, 28), radius 5, intensity 170
        int cx2 = 20, cy2 = 28, r2 = 5;

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
        // rows 5..13, cols 5..20, intensity 100
        for(int i = 5; i <= 13; i++)
            for(int j = 5; j <= 20; j++)
                a[i*N + j] = 100;

        // Shape 2: FILLED RECTANGLE (smaller)
        // rows 18..25, cols 26..36, intensity 150
        for(int i = 18; i <= 25; i++)
            for(int j = 26; j <= 36; j++)
                a[i*N + j] = 150;
    }

    // ============================================================
    // TEST CASE 4: Rectangle and Circle FAR APART
    // ============================================================
    if(tc == 4) {

        // Shape 1: FILLED RECTANGLE (top-left)
        // rows 4..12, cols 5..16, intensity 80
        for(int i = 4; i <= 12; i++)
            for(int j = 5; j <= 16; j++)
                a[i*N + j] = 80;

        // Shape 2: FILLED CIRCLE (bottom-right)
        // center (22, 31), radius 4, intensity 160
        int cx = 22, cy = 31, r = 4;

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