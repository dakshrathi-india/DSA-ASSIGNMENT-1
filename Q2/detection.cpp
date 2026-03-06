#include <iostream>
using namespace std;


// global variables 
// the binary image is of size M*N
// as we will be doing dynamic allocation of matrix and visited so we will access the [i][j] index by using matrix[i*N+j]
int M,N;
int* matrix;
bool* visited;


// it is used to initialize the Matrix which is the 2D representation of the binary image
void initMatrix(){
    cin>>M>>N;
    matrix = new int[M*N];

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cin>>matrix[i*N+j];
        }
    }
}

// it is used to initialize the Visited matrix which stores true for all the visited nodes and false for all the non visited nodes
void initVisited(){
    visited = new bool[M*N];

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            visited[i*N+j] = false;
        }
    }
}

// every pixel of the image is represented as a object of class Point 
class Point{
    public:
        int rownum;
        int colnum;
        
        //default constructor
        Point(){
            rownum = -1;
            colnum = -1;
        }

        //parameterized constructor
        Point(int r, int c){
            rownum = r;
            colnum = c;
        }

        //function 
        bool isValid(){
            if (rownum<M && rownum>=0 && colnum<N && colnum>=0)
                return true;
            else
                return false;
        }
};

// implementation of stack ADT using array 
class Stack{
    private:
        //data members
        int top = -1;
        Point* arr;
        int maxLength;
        
    public:
        //parametrixed constructor
        Stack(int size){
            maxLength = size;
            arr = new Point[maxLength];
        }

        // destructor
        ~Stack(){
            delete[] arr;
        }

        // functions
        void push(Point p){
            if (top == maxLength-1)
                cout<<"the stack is full";
            else
                arr[++top] = p;
        }

        Point pop(){
            if(top == -1)
                return Point(-1,-1);
            else
                return arr[top--];
        }

        bool isEmpty(){
            if (top == -1)
                return true;
            else
                return false;
        }

};

// each identified object belongs to the class componenet
class Object{
    public:
        //total number of foreground pixels for each detected object
        int area;

        //number of edge pixels of a object
        int boundary_pixel_count;

        //this array contains all the Points present in the boundary of the Object
        //the size of the array is set to be 4*(M+N) because no object can have a boundary length of more than 4*(M+N)
        Point* boundary_pixel;

        //default constructor
        Object(){
            area = 0;
            boundary_pixel_count = 0;
            boundary_pixel = new Point[4*(M+N)];
        }

        //copy constructor
        Object(const Object &o){
            area = o.area;
            boundary_pixel_count = o.boundary_pixel_count;
            boundary_pixel = new Point[4*(M+N)];

            for(int i=0; i<boundary_pixel_count; i++){
                boundary_pixel[i] = o.boundary_pixel[i];
            }
            
        }

        //destructor
        ~Object(){
            delete[] boundary_pixel;
        }

};


// every element of the linked list which is used to store is sparse matrix is of class Node_Sparse
class Node_Sparse{
    public:
        // data members
        int rownum;
        int colnum;
        int value;
        Node_Sparse* next;

        //parameterised constructor 
        Node_Sparse(int r, int c, int v){
            rownum = r;
            colnum = c;
            value = v;
            next = NULL;
        }
};

// every element of the linked list which is used to store all the components identified in the binary image is of type Node_Ans
class Node_Ans{
    public:
        //data members
        Object object;
        Node_Ans* next;

        //parameterised constructor 
        Node_Ans(const Object &o) : object(o), next(NULL){}
};


//a linked list with the head pointer as Ans_H is made to store all the identified componenets inside the binary image , Ans_T is the tail pointer
Node_Ans* Ans_H;
Node_Ans* Ans_T;
//a linked list with the head pointer as Sparse_H is made to store the sparse matrix , Sparse_T is the tail pointer
Node_Sparse* Sparse_H;
Node_Sparse* Sparse_T;


//this function is used to identify a component from the matrix
Object identifyObject(int start_row, int start_col){
    Object o;
    Stack s(M*N);

    s.push(Point(start_row, start_col));
    visited[start_row*N+start_col] = true;

    int x[4] = {-1,1,0,0};
    int y[4] = {0,0,-1,1};


    while(!s.isEmpty()){
        Point curr_pixel = s.pop();
        o.area ++;
        bool boundary = false;

        for(int i=0; i<4; i++){
            //using the new_row and the new_col we can access all the neighbours of the curr Point 
            //and check whether they belong to the Object or not
            int new_row = curr_pixel.rownum + x[i];
            int new_col = curr_pixel.colnum + y[i];
            Point neighbour_pixel = Point(new_row,new_col);

            //the curr_pixel Point will be considered as a boundary pixel if :
            //a)one of the neighbouring point of the curr point is not valid (means either its rownum or its colnum is out of bounds (index is not valid/not satisfy N*M)) 
            //OR
            //b)the matrix value of one of the neighbouring value is 0

            if(neighbour_pixel.isValid() && matrix[new_row*N+new_col] == 1){
                if(!visited[new_row*N+new_col]){
                    s.push(neighbour_pixel);
                    visited[new_row*N+new_col] = true;
                }
            }
            else
                boundary = true;
        }
        
        if(boundary){
            o.boundary_pixel[o.boundary_pixel_count++] = curr_pixel;
        }
    }
    return o;
}

//this function is used to add a node to a linked list at the end
void insertLL_Sparse(Node_Sparse* &head, Node_Sparse* &tail, int rownum, int colnum){
    Node_Sparse* node = new Node_Sparse(rownum, colnum, 1);
    if(head == NULL){
        head = node;
        tail = head;
    }
    else{
        tail->next = node;
        tail = tail->next;
    }
}

//this function generates a linked list from the given sparse matrix
void sparseLL(){
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            if(matrix[i*N+j] == 1){
                insertLL_Sparse(Sparse_H, Sparse_T, i, j);
            }
        }
    }
}

void printSparseLL(Node_Sparse* head){
    while(head != NULL){
        cout<<"("<<head->rownum<<","<<head->colnum<<","<<head->value<<")"<<" ";
        head = head->next;
    }
    cout<<endl;
}

//this function is used to add a node to a linked list at the end
void insertLL_Ans(Node_Ans* &head, Node_Ans* &tail,const Object &o){
    Node_Ans* node = new Node_Ans(o);
    if(head == NULL){
        head = node;
        tail = head;
    }
    else{
        tail->next = node;
        tail = tail->next;
    }
}

// this function will call the identifyObject() function for each value of the sparse linked list
void detection(Node_Sparse* head){
    Node_Sparse* temp = head;
    while(temp != NULL){
        if(!visited[temp->rownum*N + temp->colnum]){
            Object o = identifyObject(temp->rownum, temp->colnum);
            insertLL_Ans(Ans_H, Ans_T, o);
        }
        temp = temp->next;
    }
}

void printAnsLL(Node_Ans* head){
    int count = 1;
    cout<<"FOLLOWING ARE THE IDENTIFIED OBJECTS"<<endl<<endl;
    while(head != NULL){
        cout<<"OBJECT "<<count++<<endl;
        cout<<"area(number of foreground pixels present in the identified object) -> "<<head->object.area<<endl;
        cout<<"number of boundary pixels -> "<<head->object.boundary_pixel_count<<endl;
        cout<<"The following are the boundary pixels ->"<<endl;

        for(int i=0; i<head->object.boundary_pixel_count; i++){
            Point p = head->object.boundary_pixel[i];
            cout<<"("<<p.rownum<<","<<p.colnum<<")"<<" ";
        }

        head = head->next;
        cout<<endl<<endl;
    }
}

//this function is used to invert the matric
void invertImage(Node_Sparse* head){
    //intializing all the elements of the matrix to 1
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            matrix[i*N+j] = 1;
        }
    }

    //now we will be changing all those elements to 0 which were 1 in the normal image
    //to identify such elements we will be using our linked list
    while(head != NULL){
        int row = head->rownum;
        int col = head->colnum;
        matrix[row*N+col] = 0;
        head = head->next;
    }
}

//this function is used to print the matrix
void printMatrix(){
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cout<<matrix[i*N+j]<<" ";
        }
        cout<<endl;
    }
}

//this function is used to free the dynamic allocation of a linked list for representing the sparse matrix
void delLLSparse(Node_Sparse* &head, Node_Sparse* &tail){
    while(head != NULL){
        Node_Sparse* temp = head;
        head = head->next;
        delete temp;
    }
    tail = head;
}

//this function is used to free the dynamic allocation of a linked list for representing the sparse matrix
void delLLAns(Node_Ans* &head, Node_Ans* &tail){
    while(head != NULL){
        Node_Ans* temp = head;
        head = head->next;
        delete temp;
    }
    tail = head;
}

int main(){

    //initializing the arrays
    initMatrix();
    initVisited();

    Sparse_H = Sparse_T = NULL;
    //making the linked list (to represent the sparse matrix) of the normal binary image 
    sparseLL();

    //printing the sparse matrix representation of the normal binary image
    cout<<"SHOWING THE ORIGINAL SPARSE LL"<<endl;
    printSparseLL(Sparse_H);

    Ans_H = Ans_T = NULL;
    //identifying the componenets in the normal binary image (we will call the identifyComponenet() function while traversing the linked list (made for the representation of the sparse matrix))
    detection(Sparse_H);

    //printing the details of all the identifiedComponents
    cout<<endl;
    printAnsLL(Ans_H);

    //inverting the binary image 
    invertImage(Sparse_H);

    //printing the matrix of the inverted image 
    cout<<endl;
    cout<<"PRINTING THE FINAL RECONSTRUCTED IMAGE MATRIX"<<endl;
    printMatrix();

    // deleting the previous sparse ll (because now we need to store the inverted sparse matrix)
    delLLSparse(Sparse_H, Sparse_T);

    //making the linked list (to represent the sparse matrix) of the inverted binary image 
    sparseLL();

    //printing the sparse matrix representation of the normal binary image
    cout<<endl;
    cout<<"SHOWING THE NEW SPARSE LL(AFTER OPERATIONS)"<<endl;
    printSparseLL(Sparse_H);

    //deleting the dynamic allocation 
    delete[] matrix;
    delete[] visited;
    delLLSparse(Sparse_H, Sparse_T);
    delLLAns(Ans_H,Ans_T);

    return 0;
}