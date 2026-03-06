/*
================================================================================
                  BINARY IMAGE OBJECT DETECTION & INVERSION
================================================================================
This program analyzes a binary image (pixels: 0 or 1) to:
1. Identify connected components (objects)
2. Extract boundary pixels for each object
3. Represent the image as a sparse matrix using linked lists
4. Invert the binary image (swap 0s and 1s)
5. Generate sparse representation of the inverted image

Algorithm Overview:
1. Read a binary image (0=background, 1=foreground)
2. Create sparse matrix representation (store only 1s with coordinates)
3. Use DFS flood-fill to identify all connected objects
4. For each object: count pixels and extract boundary pixels
5. Invert the image: swap all 0s and 1s
6. Generate new sparse matrix for inverted image

Key Concepts:
- Binary Image: 2D matrix where each pixel is either 0 (black) or 1 (white)
- Connected Component: Group of adjacent pixels all having value 1
- Boundary Pixel: Foreground pixel adjacent to at least one background pixel or image edge
- Sparse Matrix: Efficient representation storing only non-zero elements (1s)
- Flood-Fill: Algorithm to mark all connected pixels starting from a seed
================================================================================
*/

#include <iostream>
using namespace std;


/*
GLOBAL VARIABLES - Image Data and Structures
=============================================
M, N: Dimensions of the binary image (M rows, N columns)
matrix[i*N+j]: Stores binary pixel values (0 or 1)
               Linear array representation of 2D matrix
visited[i*N+j]: Boolean array tracking which pixels have been processed during DFS
                 false = not yet visited/processed
                 true = already part of an identified object
*/

int M,N;                                      // Image dimensions
int* matrix;                                  // Dynamic array storing pixel values (0 or 1)
bool* visited;                                // Dynamic array tracking visited pixels


/*
FUNCTION: initMatrix()
======================
PURPOSE: Read the binary image dimensions and pixel values from input stream
INPUT: User provides M (rows) and N (columns), followed by M*N binary values (0 or 1)
PROCESS:
  1. Reads dimensions M and N
  2. Allocates dynamic memory for the pixel matrix (M*N integers)
  3. Populates the matrix row by row with binary pixel values
IMPORTANT: Uses row-major linear indexing: pixel[i][j] accessed as matrix[i*N+j]
TIME COMPLEXITY: O(M*N) - must read all M*N pixels
SPACE COMPLEXITY: O(M*N) - stores entire image in memory
*/
void initMatrix(){
    cin>>M>>N;                                // Read image dimensions
    matrix = new int[M*N];                    // Allocate memory for pixel data

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cin>>matrix[i*N+j];               // Read each binary pixel (0 or 1)
        }
    }
}

/*
FUNCTION: initVisited()
=======================
PURPOSE: Initialize the visited tracking array for DFS traversal
LOGIC:
  - All pixels initially marked as false (not visited)
  - As DFS explores pixels, visited[i*N+j] is set to true
  - Prevents revisiting same pixel multiple times
INVARIANT: visited[i*N+j] = false means pixel is unvisited and can be processed
TIME COMPLEXITY: O(M*N)
DESIGN NOTE: Essential for DFS to avoid cycles and redundant processing
*/
void initVisited(){
    visited = new bool[M*N];                  // Allocate visited tracking array

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            visited[i*N+j] = false;           // Initially all pixels unvisited
        }
    }
}

/*
CLASS: Point
============
PURPOSE: Represents a single pixel coordinate in the 2D image
USE CASE: Used as elements in the DFS stack during flood-fill algorithm
MEMBERS:
  - rownum: Row index (0 to M-1)
  - colnum: Column index (0 to N-1)
METHODS:
  - isValid(): Checks if the point is within image bounds
              Returns true if (0 <= rownum < M) AND (0 <= colnum < N)
              Used to prevent accessing out-of-bounds array indices
*/
class Point{
    public:
        int rownum;                           // Row coordinate of the pixel
        int colnum;                           // Column coordinate of the pixel
        
        // Default constructor - initializes to (-1, -1) indicating invalid point
        Point(){
            rownum = -1;
            colnum = -1;
        }

        // Parameterized constructor - creates a point at (r, c)
        Point(int r, int c){
            rownum = r;
            colnum = c;
        }

        // Check if this point exists within the image boundaries
        bool isValid(){
            if (rownum<M && rownum>=0 && colnum<N && colnum>=0)
                return true;                  // Point is within bounds
            else
                return false;                 // Point is out of bounds
        }
};
/*
CLASS: Stack (Array-based implementation)
==========================================
PURPOSE: Implements a LIFO (Last-In-First-Out) data structure to support DFS
USE IN PROJECT: Core data structure for flood-fill algorithm (depth-first search)
INTERNAL STRUCTURE:
  - arr[]: Dynamic array of Point objects
  - top: Index pointing to the topmost element (-1 when empty)
  - maxLength: Maximum capacity of the stack
WORKFLOW:
  1. push(Point): Add a point to top of stack (for exploring new neighbors)
  2. pop(): Remove and return the topmost point (current pixel in DFS)
  3. isEmpty(): Check if stack is empty (DFS termination condition)
TIME COMPLEXITY: All operations O(1) - constant time
NOTE: Stack size = M*N guarantees enough space for all image pixels
*/
class Stack{
    private:
        // Data members
        int top;                              // Index of topmost element (-1 when empty)
        Point* arr;                           // Dynamic array to store Point objects
        int maxLength;                        // Maximum capacity of the stack
        
    public:
        /*
        Constructor: Initialize empty stack with capacity 'size'
        PARAMETER: size - maximum number of elements the stack can hold
        */
        Stack(int size){
            top = -1;                         // Stack starts empty
            maxLength = size;                 // Set maximum capacity
            arr = new Point[maxLength];       // Allocate array memory
        }

        // Destructor: Free allocated memory when stack is destroyed
        ~Stack(){
            delete[] arr;                     // Deallocate array memory
        }

        // Functions
        /*
        push(Point): Add a point to the top of stack
        PARAMETER: p - Point to be added
        NOTE: Prints error if stack is full (won't happen with M*N size)
        */
        void push(Point p){
            if (top == maxLength-1)
                cout<<"the stack is full";
            else
                arr[++top] = p;               // Increment top, then add element
        }

        /*
        pop(): Remove and return the topmost point
        RETURN: Point from top of stack, or (-1,-1) if stack is empty
        NOTE: Invalid point (-1,-1) serves as error indicator
        */
        Point pop(){
            if(top == -1)
                return Point(-1,-1);          // Return invalid point on underflow
            else
                return arr[top--];            // Return and decrement top
        }

        /*
        isEmpty(): Check if stack is empty
        RETURN: true if empty (top == -1), false if contains elements
        */
        bool isEmpty(){
            if (top == -1)
                return true;                  // Stack is empty
            else
                return false;                 // Stack has elements
        }

};
/*
CLASS: Object
=============
PURPOSE: Represents a detected connected component (object) in the binary image
MEMBERS:
  - area: Total number of foreground pixels (value 1) in the object
  - boundary_pixel_count: Number of edges/boundary pixels
  - boundary_pixel[]: Array storing all boundary pixel coordinates
        SIZE: Maximum 4*(M+N) because perimeter can't exceed this
        Boundary pixel = foreground pixel adjacent to background or image edge
KEY PROPERTY:
  - A boundary pixel is a foreground pixel (value 1) that:
    a) Has a neighbor outside the image bounds, OR
    b) Has at least one neighbor with value 0 (background)
USAGE: Identifies object shape by analyzing boundary pixels
*/
class Object{
    public:
        // Total number of foreground pixels (value 1) for each detected object
        int area;

        // Number of edge pixels of an object
        int boundary_pixel_count;

        // Array containing all the Points present in the boundary of the Object
        // Size set to 4*(M+N) as maximum perimeter cannot exceed this
        Point* boundary_pixel;

        // Default constructor
        Object(){
            area = 0;                         // No pixels initially
            boundary_pixel_count = 0;         // No boundary pixels initially
            boundary_pixel = new Point[4*(M+N)]; // Allocate boundary array
        }

        /*
        Copy constructor: Deep copy of Object
        Creates independent copy of another Object including boundary pixels array
        */
        Object(const Object &o){
            area = o.area;
            boundary_pixel_count = o.boundary_pixel_count;
            boundary_pixel = new Point[4*(M+N)];

            // Copy all boundary pixels from source object
            for(int i=0; i<boundary_pixel_count; i++){
                boundary_pixel[i] = o.boundary_pixel[i];
            }
            
        }

        // Destructor: Free allocated memory
        ~Object(){
            delete[] boundary_pixel;          // Deallocate boundary array
        }

};


/*
CLASS: Node_Sparse
==================
PURPOSE: Represents a single node in a linked list for sparse matrix representation
USE: Builds a linked list containing only non-zero elements (1s) with coordinates
MEMBERS:
  - rownum: Row index of the foreground pixel
  - colnum: Column index of the foreground pixel
  - value: Always 1 (since sparse matrix only stores 1s)
  - next: Pointer to next node in linked list
DESIGN: Efficient storage - only stores M*N ones instead of full image
EXAMPLE: Image with 4800 pixels but only 100 ones stores only 100 nodes
*/
class Node_Sparse{
    public:
        // Data members
        int rownum;                           // Row coordinate of non-zero element
        int colnum;                           // Column coordinate of non-zero element
        int value;                            // Value (always 1 for foreground)
        Node_Sparse* next;                    // Pointer to next node

        // Parameterised constructor
        Node_Sparse(int r, int c, int v){
            rownum = r;
            colnum = c;
            value = v;                        // Usually 1
            next = NULL;                      // Initially no next node
        }
};
/*
CLASS: Node_Ans
===============
PURPOSE: Represents a node in linked list storing detected objects
USE: Builds a linked list of all identified connected components
MEMBERS:
  - object: The detected Object containing area and boundary pixels
  - next: Pointer to next detected object
DESIGN: Linked list allows dynamic storage of unknown number of objects
        Each node maintains complete Object information
*/
class Node_Ans{
    public:
        // Data members
        Object object;                        // The detected object
        Node_Ans* next;                       // Pointer to next object node

        // Parameterised constructor
        Node_Ans(const Object &o) : object(o), next(NULL){}
};


/*
GLOBAL LINKED LIST POINTERS
===========================
Ans_H, Ans_T: Head and tail pointers for linked list of detected objects
              Maintains all identified connected components
Sparse_H, Sparse_T: Head and tail pointers for linked list of sparse matrix
                    Stores efficient representation of 1s in the image
*/
Node_Ans* Ans_H;                             // Head of detected objects list
Node_Ans* Ans_T;                             // Tail of detected objects list
Node_Sparse* Sparse_H;                       // Head of sparse matrix list
Node_Sparse* Sparse_T;                       // Tail of sparse matrix list


/*
FUNCTION: identifyObject(int start_row, int start_col)
======================================================
PURPOSE: Perform flood-fill (Depth-First Search) from a seed pixel to extract
         one complete connected component (object)
PARAMETERS:
  - start_row, start_col: Coordinates of the seed pixel (unvisited foreground pixel)
ALGORITHM: DFS using iterative stack (not recursive to avoid stack overflow)
  1. Initialize empty Object
  2. Push seed point onto stack and mark as visited
  3. While stack not empty:
     a. Pop a pixel from stack (current pixel in DFS)
     b. Increment object area (count pixels)
     c. Check all 4 neighbors (up, down, left, right)
     d. Mark current pixel as boundary if:
        - Any neighbor is out of bounds, OR
        - Any neighbor has value 0 (background)
     e. For each valid neighbor with value 1 that hasn't been visited:
        - Push it onto stack
        - Mark as visited
  4. Return completed Object with area and boundary pixels

NEIGHBOR CONNECTIVITY: Uses 4-connectivity (adjacent means sharing an edge)
  - Up:    (-1, 0)
  - Down:  (+1, 0)
  - Left:  (0, -1)
  - Right: (0, +1)

BOUNDARY DEFINITION: Foreground pixel (1) adjacent to:
  - Image boundary (out of bounds), OR
  - Background pixel (0)

TIME COMPLEXITY: O(M*N) - visits each pixel at most once
SPACE COMPLEXITY: O(M*N) - stack can contain all foreground pixels

RETURN: Complete Object with area and boundary pixels array
*/
Object identifyObject(int start_row, int start_col){
    Object o;                                 // Object being built
    Stack s(M*N);                             // DFS working stack

    s.push(Point(start_row, start_col));      // Start DFS from seed pixel
    visited[start_row*N+start_col] = true;    // Mark seed as visited

    // Direction vectors for 4-connectivity (up, down, left, right)
    int x[4] = {-1,1,0,0};                   // Row offsets
    int y[4] = {0,0,-1,1};                   // Column offsets

    // DFS Main Loop: Process all pixels in the object
    while(!s.isEmpty()){
        Point curr_pixel = s.pop();           // Get next unprocessed pixel
        o.area++;                             // Count this pixel in the object
        bool boundary = false;                // Flag: is current pixel a boundary?

        // Explore all 4 neighbors of current pixel
        for(int i=0; i<4; i++){
            // Calculate neighbor coordinates
            int new_row = curr_pixel.rownum + x[i];
            int new_col = curr_pixel.colnum + y[i];
            Point neighbour_pixel = Point(new_row, new_col);

            /*
            Current pixel is a BOUNDARY pixel if:
            1) One of its neighbors is INVALID (out of bounds), OR
            2) One of its neighbors has value 0 (background)
            
            Include valid foreground neighbors (value 1) that haven't been visited
            */
            if(neighbour_pixel.isValid() && matrix[new_row*N+new_col] == 1){
                // Valid neighbor with foreground value (1)
                if(!visited[new_row*N+new_col]){
                    s.push(neighbour_pixel);                // Add to DFS stack
                    visited[new_row*N+new_col] = true;     // Mark as visited
                }
            }
            else{
                // Invalid neighbor or background pixel - current pixel is boundary
                boundary = true;
            }
        }
        
        // If this pixel is on boundary, store it
        if(boundary){
            o.boundary_pixel[o.boundary_pixel_count++] = curr_pixel;
        }
    }
    return o;                                 // Return extracted object
}

/*
FUNCTION: insertLL_Sparse(Node_Sparse* &head, Node_Sparse* &tail, int rownum, int colnum)
=======================================================================================
PURPOSE: Add a new node to the sparse matrix linked list
PARAMETERS:
  - head, tail: References to head/tail pointers (passed by reference to modify)
  - rownum, colnum: Coordinates of the foreground pixel (1) to store
LOGIC:
  - Create new Node_Sparse with value 1
  - If list is empty: make it both head and tail
  - Otherwise: link current tail to new node, update tail
TIME COMPLEXITY: O(1) - constant time insertion
*/
void insertLL_Sparse(Node_Sparse* &head, Node_Sparse* &tail, int rownum, int colnum){
    Node_Sparse* node = new Node_Sparse(rownum, colnum, 1); // Create new node
    if(head == NULL){
        head = node;                          // First node - becomes head
        tail = head;                          // and tail
    }
    else{
        tail->next = node;                    // Link current tail to new node
        tail = tail->next;                    // Update tail to new node
    }
}

/*
FUNCTION: sparseLL()
====================
PURPOSE: Generate linked list representation of sparse matrix from the binary image
ALGORITHM:
  1. Scan entire image row by row, column by column
  2. For each pixel with value 1: add its coordinates to sparse list
  3. Result: linked list containing all foreground pixels
EFFICIENCY:
  - Instead of storing M*N values (mostly 0s), store only count of 1s
  - For sparse images: significant memory savings
TIME COMPLEXITY: O(M*N) - must scan entire image
OUTPUT: Populates global Sparse_H and Sparse_T pointers
*/
void sparseLL(){
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            if(matrix[i*N+j] == 1){
                insertLL_Sparse(Sparse_H, Sparse_T, i, j); // Add to sparse list
            }
        }
    }
}

/*
FUNCTION: printSparseLL(Node_Sparse* head)
==========================================
PURPOSE: Display all elements in the sparse matrix linked list
OUTPUT FORMAT: (row,col,value) separated by spaces
EXAMPLE: (0,2,1) (1,5,1) (3,4,1) ...
TIME COMPLEXITY: O(count of 1s in image)
*/
void printSparseLL(Node_Sparse* head){
    while(head != NULL){
        cout<<"("<<head->rownum<<","<<head->colnum<<","<<head->value<<")"<<" ";
        head = head->next;
    }
    cout<<endl;
}

/*
FUNCTION: insertLL_Ans(Node_Ans* &head, Node_Ans* &tail, const Object &o)
========================================================================
PURPOSE: Add a newly detected object to the linked list of results
PARAMETERS:
  - head, tail: References to head/tail pointers (passed by reference)
  - o: The Object to store in the new node
LOGIC:
  - Create new Node_Ans containing the Object (deep copy via copy constructor)
  - If list is empty: make it both head and tail
  - Otherwise: link current tail to new node, update tail
TIME COMPLEXITY: O(boundary_pixel_count) - for object copy
*/
void insertLL_Ans(Node_Ans* &head, Node_Ans* &tail,const Object &o){
    Node_Ans* node = new Node_Ans(o);         // Create new node (invokes copy constructor)
    if(head == NULL){
        head = node;                          // First node - becomes head
        tail = head;                          // and tail
    }
    else{
        tail->next = node;                    // Link current tail to new node
        tail = tail->next;                    // Update tail to new node
    }
}

/*
FUNCTION: detection(Node_Sparse* head)
======================================
PURPOSE: Main detection algorithm - traverse sparse list and identify all objects
PARAMETERS:
  - head: Head pointer to sparse matrix linked list
ALGORITHM:
  1. Traverse sparse linked list (only contains coordinates of 1s)
  2. For each unvisited node in the list:
     - Call identifyObject() to extract complete connected component via DFS
     - Store the Object in result linked list
  3. End result: Linked list of all objects identified
NOTE: Traversing sparse list is efficient because we only check actual 1 pixels
TIME COMPLEXITY: O(M*N) - each foreground pixel visited once during DFS
OUTPUT: Populates global Ans_H, Ans_T pointers with detected objects
*/
void detection(Node_Sparse* head){
    Node_Sparse* temp = head;                 // Traversal pointer
    while(temp != NULL){
        // Check if this pixel hasn't been visited yet
        if(!visited[temp->rownum*N + temp->colnum]){
            // Extract entire object starting from this seed pixel
            Object o = identifyObject(temp->rownum, temp->colnum);
            // Store the extracted object in results
            insertLL_Ans(Ans_H, Ans_T, o);
        }
        temp = temp->next;                    // Move to next node in sparse list
    }
}

/*
FUNCTION: printAnsLL(Node_Ans* head)
====================================
PURPOSE: Display detailed information about all detected objects
OUTPUT FORMAT: For each object:
  - Object number (1, 2, 3, ...)
  - Area: Number of foreground pixels in the object
  - Boundary pixel count: Number of pixels on the edge
  - Boundary pixels: List of all edge pixel coordinates
      Useful for: shape analysis, perimeter calculation, object reconstruction
TIME COMPLEXITY: O(total_boundary_pixels) across all objects
*/
void printAnsLL(Node_Ans* head){
    int count = 1;
    cout<<"FOLLOWING ARE THE IDENTIFIED OBJECTS"<<endl<<endl;
    while(head != NULL){
        cout<<"OBJECT "<<count++<<endl;
        // Area: total foreground pixels in this object
        cout<<"area(number of foreground pixels present in the identified object) -> "<<head->object.area<<endl;
        // Boundary pixel count
        cout<<"number of boundary pixels -> "<<head->object.boundary_pixel_count<<endl;
        cout<<"The following are the boundary pixels ->"<<endl;

        // Print all boundary pixel coordinates
        for(int i=0; i<head->object.boundary_pixel_count; i++){
            Point p = head->object.boundary_pixel[i];
            cout<<"("<<p.rownum<<","<<p.colnum<<")"<<" ";
        }

        head = head->next;
        cout<<endl<<endl;
    }
}

/*
FUNCTION: invertImage(Node_Sparse* head)
========================================
PURPOSE: Invert the binary image (swap 0s and 1s)
ALGORITHM:
  1. Initialize entire matrix to 1s (all foreground)
  2. Traverse sparse linked list
  3. For each coordinate that was 1 in original image: set to 0
  RESULT: Complete inversion - background becomes foreground and vice versa
PROCESS:
  - During inversion, foreground pixels (originally 1) become background (0)
  - Background pixels (originally 0) become foreground (1)
TIME COMPLEXITY: O(M*N + count_of_ones)
PURPOSE: This operation is useful for analyzing complement of objects
*/
void invertImage(Node_Sparse* head){
    // Step 1: Initialize all elements of the matrix to 1 (foreground)
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            matrix[i*N+j] = 1;                // Set all to foreground
        }
    }

    // Step 2: Set original foreground pixels (1s) to 0 (background)
    // This effectively inverts the image
    while(head != NULL){
        int row = head->rownum;
        int col = head->colnum;
        matrix[row*N+col] = 0;                // Invert this pixel
        head = head->next;                    // Move to next node
    }
}

/*
FUNCTION: printMatrix()
======================
PURPOSE: Display the binary image matrix (2D grid of 0s and 1s)
OUTPUT FORMAT: M rows, N columns, each row on separate line
VISUALIZATION:
  - 0: Background pixel (displayed as "0")
  - 1: Foreground pixel (displayed as "1")
USE CASE: Verification of image inversion and other transformations
TIME COMPLEXITY: O(M*N)
*/
void printMatrix(){
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cout<<matrix[i*N+j]<<" ";        // Print each pixel with space
        }
        cout<<endl;                           // End of row
    }
}

/*
FUNCTION: delLLSparse(Node_Sparse* &head, Node_Sparse* &tail)
=============================================================
PURPOSE: Free allocated memory from sparse matrix linked list
PROCESS:
  1. Traverse entire linked list from head
  2. For each node: delete and move to next
  3. Set tail to NULL when complete
IMPORTANCE: Prevents memory leaks
TIME COMPLEXITY: O(count_of_ones)
*/
void delLLSparse(Node_Sparse* &head, Node_Sparse* &tail){
    while(head != NULL){
        Node_Sparse* temp = head;             // Current node to delete
        head = head->next;                    // Move to next node
        delete temp;                          // Free current node's memory
    }
    tail = head;                              // Tail also becomes NULL
}

/*
FUNCTION: delLLAns(Node_Ans* &head, Node_Ans* &tail)
====================================================
PURPOSE: Free allocated memory from detected objects linked list
PROCESS:
  1. Traverse entire linked list from head
  2. For each Node_Ans: delete (which triggers Object destructor)
     - This recursively deletes the boundary_pixel array in each Object
  3. Set tail to NULL when complete
IMPORTANCE: Critical memory cleanup - recursive deletion through destructors
TIME COMPLEXITY: O(count_of_objects)
*/
void delLLAns(Node_Ans* &head, Node_Ans* &tail){
    while(head != NULL){
        Node_Ans* temp = head;                // Current node to delete
        head = head->next;                    // Move to next node
        delete temp;                          // Free node (triggers ~Object destructor)
    }
    tail = head;                              // Tail also becomes NULL
}

/*
FUNCTION: main()
===============
PURPOSE: Main program entry point - orchestrates binary image processing workflow

EXECUTION SEQUENCE:
  1. initMatrix():
     - Read image dimensions (M, N)
     - Allocate and populate binary pixel matrix from input
     
  2. initVisited():
     - Initialize all pixels as unvisited (false)
     
  3. Create sparse representation:
     - Initialize Sparse_H and Sparse_T to NULL
     - Call sparseLL() to build linked list of all 1 pixels (foreground)
     - Print sparse representation of original image
     
  4. Detect objects:
     - Initialize Ans_H and Ans_T to NULL
     - Call detection() to identify all connected components
     - Extract area and boundary pixels for each object
     - Print detailed analysis of detected objects
     
  5. Invert image:
     - Call invertImage() to swap 0s and 1s
     - Print the inverted matrix as 2D grid
     
  6. Create sparse representation of inverted image:
     - Delete old sparse linked list
     - Call sparseLL() again on inverted matrix
     - Print new sparse representation
     
  7. Memory cleanup:
     - Delete all dynamically allocated arrays and linked list nodes
     - Prevents memory leaks
     
INPUT FORMAT:
  First line: M N (image dimensions)
  Next M lines: N space-separated integers (0 or 1)

OUTPUT FORMAT:
  1. Original sparse matrix representation
  2. Detected objects (area, boundary pixels)
  3. Inverted image matrix (2D grid)
  4. Inverted image sparse matrix representation

RETURN: 0 (successful execution)
*/
int main(){
    
    // Step 1: Read and initialize
    initMatrix();                             // Read binary image
    initVisited();                            // Initialize visited tracking

    // Step 2: Create sparse representation of original image
    Sparse_H = Sparse_T = NULL;               // Empty sparse list initially
    sparseLL();                               // Build sparse linked list
    
    cout<<"SHOWING THE ORIGINAL SPARSE LL"<<endl; // Output label
    printSparseLL(Sparse_H);                  // Display sparse representation

    // Step 3: Detect objects
    Ans_H = Ans_T = NULL;                     // Empty results list initially
    detection(Sparse_H);                      // Find all connected components
    
    cout<<endl;
    printAnsLL(Ans_H);                        // Display detected objects

    // Step 4: Invert image
    invertImage(Sparse_H);                    // Swap 0s and 1s
    
    cout<<endl;
    cout<<"PRINTING THE FINAL RECONSTRUCTED IMAGE MATRIX"<<endl; // Output label
    printMatrix();                            // Display inverted matrix

    // Step 5: Create sparse representation of inverted image
    delLLSparse(Sparse_H, Sparse_T);          // Delete old sparse list
    
    sparseLL();                               // Build sparse list for inverted image
    
    cout<<endl;
    cout<<"SHOWING THE NEW SPARSE LL(AFTER OPERATIONS)"<<endl; // Output label
    printSparseLL(Sparse_H);                  // Display new sparse representation

    // Step 6: Memory cleanup
    delete[] matrix;                          // Free pixel data
    delete[] visited;                         // Free visited flags
    delLLSparse(Sparse_H, Sparse_T);          // Free sparse linked list
    delLLAns(Ans_H, Ans_T);                   // Free detected objects list

    return 0;                                 // Successful termination
}