/*
================================================================================
                    COMPONENT DETECTION IN IMAGES
================================================================================
This program detects connected components (objects/shapes) in a 2D grayscale image.
It uses a flood-fill algorithm (Depth-First Search using stack) to identify and
analyze regions of pixels with similar intensities.

Algorithm Overview:
1. Read a 2D grayscale image (matrix of pixel intensities 0-255)
2. Use DFS-based flood fill to identify connected components
3. For each component, extract properties: boundary box, area, fill ratio
4. Classify components as Rectangular, Circular, or Other based on their properties
5. Draw bounding boxes around detected components
6. Display visual representation of the processed image

Key Concepts:
- Grayscale Image: 2D matrix where each pixel has intensity 0-255 (0=black, 255=white)
- Connected Component: Group of adjacent pixels with similar intensities
- Flood Fill: Algorithm to mark all connected pixels starting from a seed pixel
- Threshold (T): Maximum intensity difference between adjacent pixels to group them
================================================================================
*/

#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;


/*
GLOBAL VARIABLES - Image Dimensions and Data Structures
========================================================
M, N: Dimensions of the 2D image (M rows, N columns)
matrix[i*N+j]: Stores grayscale pixel intensities (0-255)
               Linear array representation of 2D matrix for efficient memory access
visited[i*N+j]: Boolean array to track which pixels have been processed during DFS
                False = not visited (ready to be grouped)
                True = already visited (part of an existing component)
T: Threshold value - controls how similar neighboring pixels must be to group together
   Smaller T = stricter grouping (more components)
   Larger T = looser grouping (fewer, larger components)
*/

int M , N;                    // Dimensions of the input image
int* matrix;                  // Dynamic array storing pixel intensities
bool* visited;                // Dynamic array tracking visited pixels
int T = 10;                   // Threshold for pixel similarity (intensity difference) 


/*
FUNCTION: initMatrix()
======================
PURPOSE: Read the input image dimensions and pixel values from input stream
INPUT: User provides M (rows) and N (columns), followed by M*N pixel values
PROCESS:
  1. Reads image dimensions M and N
  n2. Allocates dynamic memory for the pixel matrix (M*N integers)
  3. Populates the matrix row by row with pixel intensity values
IMPORTANT: Uses row-major linear indexing: pixel[i][j] accessed as matrix[i*N+j]
TIME COMPLEXITY: O(M*N) - must read all M*N pixels
SPACE COMPLEXITY: O(M*N) - stores entire image in memory
*/
void initMatrix(){
    cin >> M >> N;                                    // Read image dimensions
    matrix = new int[M*N];                            // Allocate memory for pixel data
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            cin >> matrix[i*N+j];                     // Read each pixel intensity
        }
    }
}


/*
FUNCTION: initVisited()
=======================
PURPOSE: Initialize the visited tracking array for DFS traversal
LOGIC:
  - Pixels with intensity 0 (background) are marked as visited=true
    (we ignore background and don't process it as a component)
  - Pixels with intensity > 0 (objects) are marked as visited=false
    (ready to be discovered and grouped into components during DFS)
INVARIANT: visited[i*N+j] = false means the pixel is unvisited and can start a DFS
TIME COMPLEXITY: O(M*N)
DESIGN NOTE: By pre-marking background as visited, DFS focuses only on object pixels
*/
void initVisited(){
    visited = new bool[M*N];                          // Allocate visited tracking array
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            // Background pixels (intensity 0) marked as visited to skip them
            if (matrix[i*N+j] != 0)
                visited[i*N+j] = false;               // Object pixel - not yet visited
            else
                visited[i*N+j] = true;                // Background pixel - skip it
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
              Used to prevent accessing out-of-bounds array indices during DFS
*/
class Point{
    public:
        int rownum;       // Row coordinate of the pixel
        int colnum;       // Column coordinate of the pixel

        // Default constructor - initializes to (0,0)
        Point(){
            rownum = 0;
            colnum = 0;
        };

        // Parameterized constructor - creates a point at (r, c)
        Point(int r, int c){
            rownum = r;
            colnum = c;
        }

        // Check if this point exists within the image boundaries
        bool isValid(){
            if (rownum>=0 && rownum<M && colnum>=0 && colnum<N){
                return true;                          // Point is within bounds
            }
            else{
                return false;                         // Point is out of bounds
            }
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
        int top;              // Index of topmost element (-1 when empty)
        Point* arr;           // Dynamic array to store Point objects
        int maxLength;        // Maximum capacity of the stack

    public:
        /*
        Constructor: Initialize empty stack with capacity 'size'
        PARAMETER: size - maximum number of elements the stack can hold
        */
        Stack(int size){
            top = -1;                                // Stack starts empty
            maxLength = size;                        // Set maximum capacity
            arr = new Point[maxLength];              // Allocate array memory
        }

        /*
        pop(): Remove and return the topmost point
        RETURN: Point from top of stack, or (-1,-1) if stack is empty
        NOTE: Invalid point (-1,-1) serves as error indicator
        */
        Point pop(){
            if (top == -1){
                cout<<"stack is empty";
                return Point(-1,-1);                 // Return invalid point on underflow
            }
            else{
                return arr[top--];                   // Return and decrement top
            }
        }

        /*
        push(Point): Add a point to the top of stack
        PARAMETER: element - Point to be added
        NOTE: Prints error if stack is full (though won't happen in this project)
        */
        void push(Point element){
            if(top == maxLength-1){
                cout<<"the stack is full";
            }
            else{
                arr[++top] = element;                // Increment top, then add element
            }
        }

        /*
        isEmpty(): Check if stack is empty
        RETURN: true if empty (top == -1), false if contains elements
        */
        bool isEmpty(){
            if (top == -1){
                return true;                         // Stack is empty
            }
            else{
                return false;                        // Stack has elements
            }
        }

        /*
        Destructor: Free allocated memory when stack is destroyed
        */
        ~Stack(){
            delete[] arr;                            // Deallocate array memory
        }
};


/*
CLASS: Component
================
PURPOSE: Represents a detected connected component (object/shape) in the image
MEMBERS: Stores boundary box coordinates and area information
  - minrow, maxrow: Smallest and largest row indices of the component
  - mincol, maxcol: Smallest and largest column indices of the component
  - area: Total number of pixels in the component
KEY METRICS:
  - Boundary Box: Smallest rectangle containing the entire component
  - Height/Width: Dimensions of the bounding box
  - Fill Ratio: (actual pixels) / (bounding box area)
    Used for shape classification (rectangle ~0.95, circle ~0.78, irregular ~other)
CLASSIFICATION RULES:
  - Circular: Fill ratio 0.65-0.90 AND bounding box is nearly square
  - Rectangular: Fill ratio >= 0.95
  - Other: Everything else
*/
class Component{
    public:
        // Bounding box coordinates (axis-aligned rectangle containing component)
        int minrow;           // Minimum row number in the component
        int maxrow;           // Maximum row number in the component
        int mincol;           // Minimum column number in the component
        int maxcol;           // Maximum column number in the component
        int area;             // Total number of pixels in the component

        /*
        height(): Calculate height of bounding box
        FORMULA: maxrow - minrow + 1
        NOTE: +1 because indices are inclusive on both ends
        */
        int height(){
            return maxrow-minrow+1;
        }

        /*
        width(): Calculate width of bounding box
        FORMULA: maxcol - mincol + 1
        NOTE: +1 because indices are inclusive on both ends
        */
        int width(){
            return maxcol-mincol+1;
        }

        /*
        boxArea(): Calculate total area of bounding box
        FORMULA: height * width
        NOTE: This includes background pixels if component is hollow
        */
        int boxArea(){
            return height()*width();
        }

        /*
        fillRatio(): Calculate how much of bounding box is filled with component pixels
        FORMULA: area / boxArea
        INTERPRETATION:
          - Rectangle: ~0.95-1.0 (fills most of bounding box)
          - Circle: ~0.785 (π/4 = 78.5%)
          - Irregular: ~variable
        USAGE: Used to classify shape type
        */
        double fillRatio(){
            if (boxArea() == 0)
                return 0.0;                          // Avoid division by zero

            return (double) area/boxArea();          // Cast to double for precision
        }
};

/*
FUNCTION: isRectangular(Component c)
====================================
PURPOSE: Classify if a component is a rectangle
CRITERIA: Fill ratio >= 0.95
  - Rectangles fill ~95-100% of their bounding box (mostly solid)
  - Threshold of 0.95 allows for slight irregularities at edges
RETURN: true if component is rectangular, false otherwise
EXAMPLE: A perfect 10x10 square with all pixels filled would have fillRatio=1.0
*/
bool isRectangular(Component c){
    if (c.fillRatio() >= 0.95)
        return true;                                 // Dense fill = rectangular
    else
        return false;                                // Sparse fill = not rectangular
}

/*
FUNCTION: isCircular(Component c)
=================================
PURPOSE: Classify if a component is a circle
CRITERIA:
  1. Bounding box must be nearly square (aspect ratio ~1:1)
     - width and height differ by < 10% of the larger dimension
     - Formula: |width - height| <= 0.1 * max(width, height)
  2. Fill ratio must be within expected range (0.65 - 0.90)
     - π/4 ≈ 0.785 (theoretical for perfect circle)
     - Range allows for imperfect circles and discretization effects
LOGIC:
  - First check if bounding box is square (fails fast if not)
  - Only then check fill ratio (expensive operation)
RETURN: true if both criteria met, false otherwise
EXAMPLE: A circle with radius 10 has theoretical fill ≈ 78.5%
*/
bool isCircular(Component c){
    // Test 1: Check if bounding box is nearly square
    int height = c.height();
    int width = c.width();
    // Reject if aspect ratio is too extreme (not square-ish)
    if(abs(width-height) > 0.1*max(height, width)){
        return false;                                // Bounding box is rectangular, not square
    }

    // Test 2: Check if fill ratio matches a circle
    // For a circle: area = π*r², bounding box = 4*r²
    // Fill ratio = (π*r²)/(4*r²) = π/4 ≈ 0.785
    // Range 0.65-0.90 accounts for pixelation and edge effects
    if (c.fillRatio() > 0.65 && c.fillRatio() < 0.90)
        return true;                                 // Fill ratio matches circle
    else    
        return false;                                // Fill ratio doesn't match
}


/*
CLASS: Node
===========
PURPOSE: Represents a single node in a linked list of detected components
USE: Builds a singly-linked list chain of all identified components
MEMBERS:
  - component: The Component object (boundary box, area, etc.)
  - type: Classification character (c=circular, r=rectangular, o=other)
  - next: Pointer to the next Node in the linked list
DESIGN: Linked list allows easy insertion of new components as they're discovered
*/
class Node{
    public:
        Component component;      // The detected component object
        char type;                // Shape classification: c/r/o
        Node* next;               // Pointer to next node in linked list

        /*
        Constructor: Creates a new node containing a component
        PARAMETER: c - the Component to store in this node
        PROCESS: Classifies the component and sets next pointer to NULL
        */
        Node(Component c){
            component = c;          // Store component data

            // Classify the component type
            if(isCircular(c)){
                type = 'c';                          // Circular component
            }
            else if(isRectangular(c)){
                type = 'r';                          // Rectangular component
            }
            else{
                type = 'o';                          // Other/irregular component
            }

            next = NULL;                             // Initialize as end of list
        }
};

/*
FUNCTION: buildComponent(int start_row, int start_col)
======================================================
PURPOSE: Perform flood-fill (Depth-First Search) from a seed pixel to extract
         one complete connected component
PARAMETERS:
  - start_row, start_col: Coordinates of the seed pixel (unvisited object pixel)
ALGORITHM: DFS using iterative stack (not recursive to avoid stack overflow)
  1. Initialize component with seed pixel coordinates
  2. Push seed point onto stack and mark as visited
  3. While stack not empty:
     a. Pop a pixel from stack (current pixel in DFS)
     b. Update component bounding box to include this pixel
     c. Check all 4 neighbors (up, down, left, right)
     d. For each valid neighbor that hasn't been visited and has similar intensity:
        - Push it onto stack
        - Mark as visited (to avoid revisiting)
  4. Return completed component

NEIGHBOR CONNECTIVITY: Uses 4-connectivity (adjacent means sharing an edge)
  - Up:    (-1, 0)
  - Down:  (+1, 0)
  - Left:  (0, -1)
  - Right: (0, +1)

SIMILARITY CRITERIA: Neighbors included if intensity difference from seed < T
  - Allows slight variations within a component
  - T=10 means neighbors can differ by up to 10 intensity levels from seed

TIME COMPLEXITY: O(M*N) - visits each pixel at most once
SPACE COMPLEXITY: O(M*N) - stack can contain all unvisited pixels in worst case

RETURN: Complete Component object with area and bounding box
*/
Component buildComponent(int start_row, int start_col){
    Stack s(M*N);                                    // DFS working stack

    int seed_row = start_row;                       // Remember seed pixel
    int seed_col = start_col;                       // intensity for comparison

    Component c;                                    // Component being built

    // Initialize component boundaries with seed point
    c.area = 0;                                     // No pixels counted yet
    c.minrow = c.maxrow = start_row;                // Initial bounding box
    c.mincol = c.maxcol = start_col;                // is just the seed pixel

    // Start DFS from seed pixel
    s.push(Point(start_row, start_col));
    visited[start_row*N + start_col] = true;        // Mark seed as visited

    // Direction vectors for 4-connectivity (up, down, left, right)
    int x[4] = {-1, 1, 0, 0};                      // Row offsets
    int y[4] = {0, 0, -1, 1};                      // Column offsets

    // DFS Main Loop: Process all pixels in the component
    while(!s.isEmpty()){
        Point curr_pixel = s.pop();                 // Get next unprocessed pixel
        
        int row = curr_pixel.rownum;
        int col = curr_pixel.colnum;

        // Update component bounding box to include current pixel
        c.minrow = min(c.minrow, row);              // Expand box upward if needed
        c.maxrow = max(c.maxrow, row);              // Expand box downward if needed
        c.mincol = min(c.mincol, col);              // Expand box leftward if needed
        c.maxcol = max(c.maxcol, col);              // Expand box rightward if needed
        c.area++;                                   // Count this pixel as part of component

        // Explore all 4 neighbors of current pixel
        for(int i = 0; i < 4; i++){
            int new_row = row + x[i];
            int new_col = col + y[i];

            Point neighbour_pixel = Point(new_row, new_col);
            
            // Include neighbor if it meets all 3 criteria:
            // 1. Valid: Within image bounds
            // 2. Unvisited: Not already processed
            // 3. Similar: Intensity within threshold of seed pixel
            if(neighbour_pixel.isValid() &&                              // Criterion 1
               !visited[new_row*N + new_col] &&                          // Criterion 2
               abs(matrix[seed_row*N + seed_col] - matrix[new_row*N + new_col]) < T) { // Criterion 3
                s.push(neighbour_pixel);                                 // Add to DFS stack
                visited[new_row*N + new_col] = true;                    // Mark to avoid revisiting
            }
        }
    }
    
    return c;                                       // Return extracted component
}


/*
GLOBAL LINKED LIST POINTERS
===========================
Ans_H: Head pointer to linked list of detected components
Ans_T: Tail pointer for efficient insertion of new components
DESIGN: Maintaining both pointers allows O(1) insertion at end of list
*/
Node* Ans_H;                                        // Head of component list
Node* Ans_T;                                        // Tail of component list

/*
FUNCTION: addComponenentToAns(Node* &head, Node* &tail, Node* n)
==============================================================
PURPOSE: Add a newly discovered component to the linked list
PARAMETERS:
  - head, tail: References to head/tail pointers (passed by reference to modify)
  - n: New node containing a component
LOGIC:
  - If list is empty (head == NULL): Make n both head and tail
  - Otherwise: Link current tail to n, update tail to point to n
TIME COMPLEXITY: O(1) - constant time insertion
NOTE: Tail pointer avoids O(n) traversal to find end of list
*/
void addComponenentToAns(Node* &head, Node* &tail, Node* n){
    if (head == NULL){
        head = n;                                    // First node - becomes head
        tail = n;                                    // and tail
    }
    else{
        tail->next = n;                             // Link current tail to new node
        tail = tail->next;                          // Update tail to new node
    }
}

/*
FUNCTION: detection()
====================
PURPOSE: Main detection algorithm - scan entire image and identify all components
ALGORITHM:
  1. Scan image row by row, column by column
  2. When an unvisited object pixel found:
     - Call buildComponent() to extract entire connected component via DFS
     - If component size > 1% of image area: add to results (filter noise)
     - Continue scanning
  3. End result: Linked list of all significant components

NOISE FILTERING: Components smaller than 1% of total image area are ignored
  - Avoids cluttering output with tiny artifacts/noise
  - 1% threshold is tunable: decrease for more detail, increase for less sensitivity

TIME COMPLEXITY: O(M*N) - each pixel visited once during DFS
OUTPUT: Populates global linked list (Ans_H, Ans_T)
*/
void detection(){
    // Scan entire image systematically
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            // Found an unvisited object pixel (not background, not yet grouped)
            if(!visited[i*N + j]){
                // Extract complete component starting from this seed pixel
                Component component = buildComponent(i, j);

                // Filter out noise: keep only components >= 1% of image size
                // Rationale: Tiny scattered pixels are likely noise/artifacts
                // 0.01*(M*N) = 1% of total image pixels
                if (component.area > 0.01*(M*N)){
                    // Significant component found - add to results
                    Node* n = new Node(component);
                    addComponenentToAns(Ans_H, Ans_T, n);
                }
                // If component is too small: silently ignore (filtered as noise)
            }
        }
    }
}

/*
FUNCTION: printIdentifiedComponents(Node* head)
===============================================
PURPOSE: Display detailed information about all detected components
OUTPUT FORMAT: For each component, prints:
  - Component number (1, 2, 3, ...)
  - Fill Ratio: Ratio of actual pixels to bounding box area
  - Type: Character classification (c/r/o)
  - Bounding box coordinates: minrow, maxrow, mincol, maxcol
  - Dimensions: Height and width of bounding box
  - Box Area: Total area of bounding box

INTERPRETATION GUIDE:
  - Fill Ratio ~0.95+: Likely rectangular shape
  - Fill Ratio ~0.65-0.90: Likely circular shape
  - Fill Ratio other: Irregular/other shape
  - Type: 'c'=circular, 'r'=rectangular, 'o'=other
  - Coordinates useful for re-processing or visualization

NOTE: If no components found, prints "no region identified"
*/
void printIdentifiedComponents(Node* head){
    if (head == NULL){
        cout << "no region identified";               // No components found
    }
    else{
        Node* temp = head;
        int count = 1;
        cout << "The identified components are ->" << endl << endl;
        
        // Traverse linked list and print each component
        while(temp != NULL){
            cout << "Component " << count++ << endl;
            cout << "FillRatio -> " << temp->component.fillRatio() << endl;
            cout << "Type -> " << temp->type << endl;
            cout << "PROPERTIES OF THE BOUNDARY BOX" << endl;
            cout << "Min Row " << temp->component.minrow << endl;
            cout << "Max Row " << temp->component.maxrow << endl;
            cout << "Min Col " << temp->component.mincol << endl;
            cout << "Max Col " << temp->component.maxcol << endl;
            cout << "Height " << temp->component.height() << endl;
            cout << "Width " << temp->component.width() << endl;
            cout << "Box Area " << temp->component.boxArea() << endl;
            cout << endl;

            temp = temp->next;                        // Move to next component
        }
    }
}

/*
FUNCTION: buildBoundary(Node* head)
==================================
PURPOSE: Draw rectangular bounding boxes around all detected components
MECHANISM: Sets all boundary pixels to intensity 255 (white)
DRAWING PROCESS: For each component, draws 4 edges of bounding box:
  1. Left edge: pixels at column mincol, rows minrow to maxrow
  2. Right edge: pixels at column maxcol, rows minrow to maxrow
  3. Top edge: pixels at row minrow, columns mincol to maxcol
  4. Bottom edge: pixels at row maxrow, columns mincol to maxcol

VISUAL RESULT: White rectangular frames around each detected object
  - Useful for visualization and verification of detection accuracy
  - Can be displayed using printVisualImage() function

WHY 255?: Maximum grayscale intensity (white), highly visible contrast
TIME COMPLEXITY: O(total_boundary_perimeter) across all components
*/
void buildBoundary(Node* head){
    Node* temp = head;
    
    // Process each component in the linked list
    while(temp != NULL){
        // Draw left and right edges of bounding box
        for (int row = temp->component.minrow; row <= temp->component.maxrow; row++)
        {
            matrix[row*N + temp->component.mincol] = 255;  // Left edge
            matrix[row*N + temp->component.maxcol] = 255;  // Right edge
        }

        // Draw top and bottom edges of bounding box
        for (int col = temp->component.mincol; col <= temp->component.maxcol; col++)
        {
            matrix[temp->component.minrow*N + col] = 255;  // Top edge
            matrix[temp->component.maxrow*N + col] = 255;  // Bottom edge
        }
        
        temp = temp->next;                            // Process next component
    }
}

/*
FUNCTION: printVisualImage()
===========================
PURPOSE: Create ASCII art visualization of the processed image
METHOD: Convert pixel intensity to ASCII character for text-based display

CHARACTER MAPPING (intensity ranges):
  - 0-30 (black):    ' ' (space) - background
  - 31-60 (dark):    '`' (backtick) - very dark object pixels
  - 61-100 (medium): '-' (dash) - medium dark
  - 101-150 (light): '+' (plus) - medium light
  - 151-200 (bright):'@' (at) - bright
  - 201-255 (white): '$' (dollar) - white (boundaries)

USEFULNESS:
  - Quick visual verification of detection without external image viewer
  - Bounding boxes appear as '$' characters (white at 255)
  - Original components visible through varying ASCII characters
  - Works in any terminal environment

TIME COMPLEXITY: O(M*N) - must print all pixels
NOTE: Some character representation lost due to ASCII limitations,
      but still useful for verification purposes
*/
void printVisualImage(){
    cout << "Visual Representation" << endl << endl;

    // Iterate through entire image and print ASCII representation
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int val = matrix[i*N + j];                // Get pixel intensity
            char pixel;                               // Character to display

            // Map intensity to ASCII character (quantization)
            if (val <= 30)       
                pixel = ' ';                          // Background (black)
            else if (val <= 60)  
                pixel = '`';                          // Very dark
            else if (val <= 100) 
                pixel = '-';                          // Dark
            else if (val <= 150) 
                pixel = '+';                          // Medium
            else if (val <= 200) 
                pixel = '@';                          // Bright
            else                 
                pixel = '$';                          // White (boundaries/255)

            cout << pixel;                            // Print the character
        }
        cout << "\n";                                 // End of row
    }
    cout << "\n";                                     // Extra spacing
}

/*
FUNCTION: freeHeapMemory()
=========================
PURPOSE: Release all dynamically allocated memory before program termination
MEMORY CLEANUP:
  1. Delete matrix array (stores M*N pixel values)
  2. Delete visited array (stores M*N boolean flags)
  3. Traverse linked list and delete each Node

IMPORTANCE: Prevents memory leaks
  - Critical for long-running programs or repeated detection operations
  - Good practice to always free what you allocate

TIME COMPLEXITY: O(M*N + #components) - traverses arrays and linked list
*/
void freeHeapMemory(){
    delete[] matrix;                                  // Free pixel data
    delete[] visited;                                 // Free visited flags

    // Traverse linked list and delete each component node
    Node* temp = Ans_H;
    while(temp != NULL){
        Node* destroy = temp;                         // Current node to delete
        temp = temp->next;                            // Move to next node
        delete destroy;                               // Free current node
    }
}

/*
FUNCTION: main()
===============
PURPOSE: Main program entry point - orchestrates image processing workflow

EXECUTION SEQUENCE:
  1. initMatrix():
     - Read image dimensions (M, N)
     - Allocate and populate pixel matrix from input
     
  2. initVisited():
     - Mark all background pixels (0) as visited
     - Mark all object pixels (>0) as unvisited
     
  3. Initialize result pointers:
     - Ans_H = NULL (head of empty list)
     - Ans_T = NULL (tail of empty list)
     
  4. detection():
     - Scan entire image
     - For each unvisited pixel, run DFS to extract component
     - Filter noise (< 1% of image)
     - Add significant components to linked list
     
  5. printIdentifiedComponents(Ans_H):
     - Display detailed statistics about each detected component
     - Includes fill ratio, type, bounding box coordinates
     
  6. buildBoundary(Ans_H):
     - Draw white (255) rectangular frames around all components
     - Modifies the image matrix
     
  7. printVisualImage():
     - Display ASCII art visualization of processed image
     - Shows results with component boundaries highlighted
     
  8. freeHeapMemory():
     - Release all allocated memory
     - Avoid memory leaks
     
INPUT FORMAT:
  First line: M N (image dimensions)
  Next M lines: N space-separated integers (pixel intensities 0-255)

OUTPUT FORMAT:
  1. Detailed component analysis (coordinates, type, metrics)
  2. ASCII visualization with boundaries drawn

RETURN: 0 (successful execution)
*/
int main(){
    // Read input image and initialize data structures
    initMatrix();                                     // Read image from input
    initVisited();                                    // Initialize visited tracking

    // Initialize result linked list
    Ans_H = NULL;                                     // Empty head
    Ans_T = NULL;                                     // Empty tail

    // Main processing pipeline
    detection();                                      // Find all connected components
    printIdentifiedComponents(Ans_H);                 // Display component details
    buildBoundary(Ans_H);                             // Draw bounding boxes
    printVisualImage();                               // Show visual result

    // Cleanup
    freeHeapMemory();                                 // Free allocated memory

    return 0;                                         // Successful termination
}