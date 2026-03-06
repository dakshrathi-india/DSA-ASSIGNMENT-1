#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int M,N;

void clearMatrix(int *a){
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)
            a[i*N+j] = 0;
}

void writeToFile(int *a,int tc){
    string fname = "input_tc" + to_string(tc) + ".in";
    ofstream fout(fname);

    fout<<M<<" "<<N<<endl;

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++)
            fout<<a[i*N+j]<<" ";
        fout<<endl;
    }

    fout.close();
    cout<<fname<<" generated"<<endl;
}

void generateTestCase(int tc){

    int *a = nullptr;

    // ============================================================
    // TEST CASE 1
    // square + rectangle + circle
    // matrix 20 x 25
    // ============================================================

    if(tc==1){

        M = 20;
        N = 25;

        a = new int[M*N];
        clearMatrix(a);

        // square
        for(int i=2;i<=5;i++)
            for(int j=2;j<=5;j++)
                a[i*N+j] = 1;

        // rectangle
        for(int i=6;i<=10;i++)
            for(int j=10;j<=16;j++)
                a[i*N+j] = 1;

        // circle (radius 3)
        int cx = 15;
        int cy = 19;
        int r  = 3;

        for(int i=0;i<M;i++)
            for(int j=0;j<N;j++){
                int dx=i-cx;
                int dy=j-cy;
                if(dx*dx+dy*dy<=r*r)
                    a[i*N+j]=1;
            }
    }

    // ============================================================
    // TEST CASE 2
    // two circles
    // matrix 25 x 20
    // ============================================================

    if(tc==2){

        M = 25;
        N = 20;

        a = new int[M*N];
        clearMatrix(a);

        int cx1=7,  cy1=6,  r1=3;
        int cx2=18, cy2=14, r2=2;

        for(int i=0;i<M;i++)
            for(int j=0;j<N;j++){

                int dx1=i-cx1, dy1=j-cy1;
                if(dx1*dx1+dy1*dy1<=r1*r1)
                    a[i*N+j]=1;

                int dx2=i-cx2, dy2=j-cy2;
                if(dx2*dx2+dy2*dy2<=r2*r2)
                    a[i*N+j]=1;
            }
    }

    // ============================================================
    // TEST CASE 3
    // multiple rectangles
    // matrix 18 x 30
    // ============================================================

    if(tc==3){

        M = 18;
        N = 30;

        a = new int[M*N];
        clearMatrix(a);

        // rectangle 1
        for(int i=2;i<=6;i++)
            for(int j=2;j<=8;j++)
                a[i*N+j]=1;

        // rectangle 2
        for(int i=9;i<=13;i++)
            for(int j=12;j<=18;j++)
                a[i*N+j]=1;

        // rectangle 3
        for(int i=4;i<=8;i++)
            for(int j=23;j<=26;j++)
                a[i*N+j]=1;
    }

    // ============================================================
    // TEST CASE 4
    // hollow square + rectangle + isolated pixels
    // matrix 30 x 25
    // ============================================================

    if(tc==4){

        M = 30;
        N = 25;

        a = new int[M*N];
        clearMatrix(a);

        // hollow square
        for(int j=3;j<=9;j++){
            a[3*N+j]=1;
            a[9*N+j]=1;
        }

        for(int i=3;i<=9;i++){
            a[i*N+3]=1;
            a[i*N+9]=1;
        }

        // filled rectangle
        for(int i=15;i<=20;i++)
            for(int j=12;j<=17;j++)
                a[i*N+j]=1;

        // isolated pixels
        a[25*N+4]  = 1;
        a[27*N+8]  = 1;
        a[26*N+20] = 1;
    }

    writeToFile(a,tc);
    delete[] a;
}

int main(){

    for(int tc=1; tc<=4; tc++)
        generateTestCase(tc);

    return 0;
}