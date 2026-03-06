#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;


// the 2D matrix representation of the grayscale image is of dimension MxN
int M , N;

// to access the [i][j] index of the matrix array we will be using matrix[i*N+j]
int* matrix;


// this is the visited matrix which tells whether a element has been visited or not
// if the value at index [i][j] is false => that index [i][j] has not been visited
bool* visited;

// here T is the threshold value , which is used for grouping the pixels as regions 
int T = 10; 


// this function is used to initialize the 2d matrix based on the given input 
void initMatrix(){
    cin >> M >> N;
    matrix = new int[M*N];
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cin >> matrix[i*N+j];
        }
    }
}


// in this function we only initialize those indices whose value is non zero with false and all the other with true
// we consider the indices with zero value as of the background and dont consider them in our detection
void initVisited(){
    visited = new bool[M*N];
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            if (matrix[i*N+j] != 0)
                visited[i*N+j] = false;
            else
                visited[i*N+j] = true;
        }
    }
}


// each point in the 2D matrix is represented using the class point 
class Point{
    public:
        // data members
        int rownum;
        int colnum;

        //default constructor
        Point(){
            rownum = 0;
            colnum = 0;
        };

        //parameterized constructor
        Point(int r, int c){
            rownum = r;
            colnum = c;
        }

        // functions
        bool isValid(){
            if (rownum>=0 && rownum<M && colnum>=0 && colnum<N){
                return true;
            }
            else{
                return false;
            }
        }
};

// implementation of stack ADT using array (in this we will be using the data structure Point)
class Stack{
    private:
        int top = -1;
        Point* arr ;
        int maxLength;

    public:
        // parameterized constructor 
        Stack(int size){
            maxLength = size;
            arr = new Point[maxLength];
        }

        Point pop(){
            if (top == -1){
                cout<<"stack is empty";
                return Point(-1,-1);
            }
            else{
                return arr[top--];
            }
        }

        void push(Point element){
            if(top == maxLength-1){
                cout<<"the stack is full";
            }
            else{
                arr[++top] = element;
            }
        }

        bool isEmpty(){
            if (top == -1){
                return true;
            }
            else{
                return false;
            }
        }

        // destructor 
        ~Stack(){
            delete[] arr;
        }

};


// this class contains a componenet that is present in the image eg a square/rectangle/circle
class Component{
    public:
        // these data members correspond to the boundary box of a detected component in a given image
        int minrow;
        int maxrow;
        int mincol;
        int maxcol;

        // this is the numbers of pixels present in the componenet 
        int area;

        int height(){
            return maxrow-minrow+1;
        }

        int width(){
            return maxcol-mincol+1;
        }

        int boxArea(){
            return height()*width();
        }

        double fillRatio(){
            if (boxArea() == 0)
                return 0.0;

            return (double) area/boxArea();
        }
};

// this function checks whether the component is rectangular
// if the fillRatio of the component is >= 0.95 then we will consider it to be a rectangular region
bool isRectangular(Component c){
    if (c.fillRatio() >= 0.95)
        return true;
    else
        return false;
}

bool isCircular(Component c){
    // the bounding region should be near about square 
    // if it is not then directly return false
    int height = c.height();
    int width = c.width();
    if(abs(width-height) > 0.1*max(height, width)){
        return false;
    }

    // expected fillRatio = 0.785 = ((pi*r*r)/(4*r*r))
    if (c.fillRatio() > 0.65 && c.fillRatio() < 0.90)
        return true;
    else    
        return false;
}


//class node to be used in the linked list to store all the componenets
class Node{
    public:
        // data members
        // stores the component
        Component component;
        // stores the type of the component c->circular r->rectangular o->other
        char type;
        Node* next;

        // parameterized constructor
        Node(Component c){
            component = c;

            if(isCircular(c)){
                type = 'c';
            }
            else if(isRectangular(c)){
                type = 'r';
            }
            else{
                type = 'o';
            }

            next = NULL;
        }

};

Component buildComponent(int start_row, int start_col){
    Stack s(M*N);

    int seed_row = start_row;
    int seed_col = start_col;

    Component c;

    c.area = 0;
    c.minrow = c.maxrow = start_row;
    c.mincol = c.maxcol = start_col;

    s.push(Point(start_row,start_col));
    visited[start_row*N+start_col] = true;

    // using these arrays x,y we can access the neighbouring Points of a given Point
    int x[4] = {-1,1,0,0};
    int y[4] = {0,0,-1,1};

    while(!s.isEmpty()){

        Point curr_pixel = s.pop();
        
        int row = curr_pixel.rownum;
        int col = curr_pixel.colnum;

        // updating the component 
        c.minrow = min(c.minrow,row);
        c.maxrow = max(c.maxrow,row);
        c.mincol = min(c.mincol,col);
        c.maxcol = max(c.maxcol,col);
        c.area ++;

        for(int i=0; i<4; i++){
            // exploring the neighnbouring pixels of the current pixel 
            int new_row = row + x[i];
            int new_col = col + y[i];

            Point neighbour_pixel = Point(new_row, new_col);
            // we will include it in the component if :
            // 1) it is a valid point (which is present in the matrix)
            // 2) it is not visited
            // 3) its matrix value is within the tolerance value

            if(neighbour_pixel.isValid() && !visited[new_row*N+new_col] && abs(matrix[seed_row*N+seed_col] - matrix[new_row*N+new_col]) < T ){
                s.push(neighbour_pixel);
                visited[new_row*N + new_col] = true;
            }

        }

    }
    return c;

}


// this is the header pointer to the linkedlist which stores all the identified components 
Node* Ans_H;
Node* Ans_T;

void addComponenentToAns(Node* &head, Node* &tail, Node* n){
    if (head == NULL){
        head = n;
        tail = n;
    }
    else{
        tail->next = n;
        tail = tail->next;
    }
}

void detection(){
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            if(!visited[i*N+j]){
                Component component = buildComponent(i,j);

                // area of the component is the number of pixels present inside the component
                // total area is the total number of pixels present in the image 
                // if the area of the component is less the 1% of the total area then it is probably noise , so ignore it 
                if (component.area > 0.01*(M*N)){
                    // store the details in the linked list 
                    Node* n = new Node(component);
                    addComponenentToAns(Ans_H,Ans_T,n);
                }
            }
        }
    }
}

void printIdentifiedComponents(Node* head){
    if (head == NULL)
        cout<<"no region identified";
    else{
        Node* temp = head;
        int count = 1;
        cout<<"The identifed components are -> "<<endl<<endl;
        while(temp != NULL){
            cout<<"Component "<<count++<<endl;
            cout<<"FillRatio -> "<<temp->component.fillRatio()<<endl;
            cout<<"Type -> "<<temp->type<<endl;
            cout<<"PROPERTIES OF THE BOUNDARY BOX"<<endl;
            cout<<"Min Row "<<temp->component.minrow<<endl;
            cout<<"Max Row "<<temp->component.maxrow<<endl;
            cout<<"Min COl "<<temp->component.mincol<<endl;
            cout<<"Max Col "<<temp->component.maxcol<<endl;
            cout<<"Height "<<temp->component.height()<<endl;
            cout<<"Width "<<temp->component.width()<<endl;
            cout<<"Box Area "<<temp->component.boxArea()<<endl;
            cout<<endl;

            temp = temp->next;
        }
    }
}

void buildBoundary(Node* head){
    Node* temp = head;
    //in this we are going to make the matrix value of all the pixels
    //present in the boundary box of a region to 255
    while(temp != NULL){
        for (int row = temp->component.minrow; row <= temp->component.maxrow; row++)
        {
            matrix[row*N+temp->component.mincol] = 255;
            matrix[row*N+temp->component.maxcol] = 255;
        }

        for (int col = temp->component.mincol; col <= temp->component.maxcol; col++)
        {
            matrix[temp->component.minrow*N+col] = 255;
            matrix[temp->component.maxrow*N+col] = 255;
        }
        temp = temp->next;
    }
}

void printVisualImage(){
    cout << "Visual Representation "<<endl<<endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int val = matrix[i*N+j];
            char pixel;

            if (val <= 30)       
                pixel = ' ';
            else if (val <= 60)  
                pixel = '`';
            else if (val <= 100) 
                pixel = '-';
            else if (val <= 150) 
                pixel = '+';
            else if (val <= 200) 
                pixel = '@';
            else                 
                pixel = '$';

            cout << pixel;
        }
        cout << "\n";
    }
    cout << "\n";
}

// freeing the heap memory
void freeHeapMemory(){
    delete[] matrix;
    delete[] visited;

    Node* temp = Ans_H;
    while(temp != NULL){
        Node* destroy = temp;
        temp = temp->next;
        delete destroy;
    }
}

int main(){
    
    initMatrix();
    initVisited();

    Ans_H = NULL;
    Ans_T = NULL;

    detection();
    printIdentifiedComponents(Ans_H);
    buildBoundary(Ans_H);
    printVisualImage();

    freeHeapMemory();

    return 0;
}