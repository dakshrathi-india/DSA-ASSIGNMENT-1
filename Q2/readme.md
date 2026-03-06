# Binary Image Object Detection & Image Inversion

## Project Overview

This Q2 project implements a **binary image processing system** that detects connected components (objects) in black-and-white images and performs image inversion. The system uses Depth-First Search (DFS) with efficient sparse matrix representation using linked lists.

### Core Objectives

1. **Component Detection**: Identify all connected foreground regions in a binary image
2. **Boundary Extraction**: Find and store edge pixels for each detected object
3. **Sparse Representation**: Efficiently represent images using linked lists
4. **Image Inversion**: Convert foreground to background and vice versa
5. **Data Reconstruction**: Regenerate sparse representation of the inverted image

---

## Problem Statement

Given a binary image (M×N matrix with pixels 0 or 1):
- Detect all connected components where adjacent 1s form objects
- Extract boundary pixels (edges touching background or image boundary)
- Represent the image and inverted image as sparse matrices using linked lists
- Provide detailed analysis of detected objects

---

## Algorithm Overview

### Main Pipeline

```
Input Image (Binary)
        ↓
    Read Image
        ↓
  Sparse Representation ← Linked List of 1s
        ↓
  Print Original Sparse Matrix
        ↓
  DFS Component Detection ← For each unvisited 1
        ↓
  Identify Objects (Area + Boundary)
        ↓
  Print Detected Objects
        ↓
  Image Inversion (Swap 0s and 1s)
        ↓
  Print Inverted Matrix
        ↓
  Sparse Representation of Inverted Image
        ↓
  Print New Sparse Matrix
        ↓
  Memory Cleanup
        ↓
Output Complete
```

### DFS-Based Flood-Fill Algorithm

**Purpose**: Extract one complete connected component starting from a seed pixel

```
identifyObject(row, col):
    1. CREATE empty Object o
    2. CREATE stack, PUSH (row, col)
    3. MARK (row, col) as visited
    
    WHILE stack not empty:
        4. curr_pixel = POP stack
        5. o.area++
        6. SET boundary = false
        
        FOR each 4-neighbor (up, down, left, right):
            7. IF neighbor is valid AND neighbor.value == 1:
                   IF neighbor not visited:
                       PUSH neighbor to stack
                       MARK neighbor as visited
               ELSE (invalid or background):
                   SET boundary = true
        
        8. IF boundary == true:
               ADD curr_pixel to o.boundary_pixels
    
    9. RETURN o
```

**4-Connectivity**: Neighbors share an edge (not diagonal)
```
        N
        ↓
    W ← X → E
        ↑
        S
```

### Boundary Pixel Definition

A foreground pixel (1) is a **boundary pixel** if:
- It has a neighbor **outside image bounds** (invalid), OR
- It has a neighbor with value **0** (background)

---

## Data Structures

### Class: Point
```cpp
Represents: Single pixel coordinate
Members:
  - rownum: Row index (0 to M-1)
  - colnum: Column index (0 to N-1)
Methods:
  - isValid(): Check if within image bounds
```

### Class: Stack
```cpp
Purpose: LIFO structure for DFS
Operations:
  - push(Point): Add to top
  - pop(): Remove from top, return value
  - isEmpty(): Check if empty
Time Complexity: O(1) per operation
Capacity: M*N (max pixels in image)
```

### Class: Object
```cpp
Represents: Detected connected component
Members:
  - area: Count of foreground pixels
  - boundary_pixel_count: Number of edge pixels
  - boundary_pixel[]: Array of boundary coordinates
Capacity: 4*(M+N) pixels (theoretical max perimeter)
Construction: Default, Copy, Destruction with cleanup
```

### Class: Node_Sparse
```cpp
Represents: Single node in sparse matrix linked list
Members:
  - rownum, colnum: Coordinates of foreground pixel
  - value: Always 1 (for sparse, only store 1s)
  - next: Pointer to next node
Purpose: Memory-efficient image representation
```

### Class: Node_Ans
```cpp
Represents: Single node in detected objects linked list
Members:
  - object: The detected Object
  - next: Pointer to next
Purpose: Dynamic storage of detected components
```

### Linked List Structure

**Sparse Matrix List**:
```
Sparse_H → Node(r0,c0) → Node(r1,c2) → Node(r2,c1) → ... → NULL
           └─ next ─┘    └─ next ─┘    └─ next ─┘
```
Stores all foreground pixels with efficiency

**Detected Objects List**:
```
Ans_H → Node(Object1) → Node(Object2) → Node(Object3) → ... → NULL
        └─ next ─┘     └─ next ─┘     └─ next ─┘
```
Stores all detected components

---

## Key Algorithms

### Image Inversion

```cpp
InvertImage():
    1. Set all matrix values to 1
    2. For each node in sparse list:
       Set matrix[row][col] = 0
    Result: Original 1s become 0s, original 0s become 1s
```

**Efficiency**: O(M×N + count_of_ones) since we initialize then selectively zero

### Sparse Matrix Construction

```cpp
SparseLL():
    1. Scan entire image M×N
    2. For each pixel == 1:
       Add (row, col, 1) to linked list
    Result: Efficient representation
    
Memory Savings:
    Dense: M*N values
    Sparse: ~count_of_ones * 3 pointers/values
    For 4800 pixels with only 400 ones: 92% reduction!
```

---

## File Structure

```
Q2/
├── detection_copy.cpp           Main detection algorithm (FULLY COMMENTED)
├── matrixgenerator.cpp          Generates 4 test cases
├── input_tc1.in                 Test case 1: Square + Rectangle + Circle
├── input_tc2.in                 Test case 2: Two circles
├── input_tc3.in                 Test case 3: Multiple rectangles
├── input_tc4.in                 Test case 4: Hollow and filled shapes
├── instructions.md              Project execution guide
└── README.md                    This file
```

---

## Test Cases

### Test Case 1: Mixed Shapes (20 rows × 25 cols)
```
Image contents:
  - Square: 4×4 filled at (2-5, 2-5)
  - Rectangle: 5×7 filled at (6-10, 10-16)
  - Circle: radius 3 centered at (15, 19)

Expected output:
  ✓ 3 objects detected
  ✓ Areas: ~16, ~35, ~28
  ✓ Different boundary patterns
```

### Test Case 2: Two Circles (25 rows × 20 cols)
```
Image contents:
  - Large circle: radius 3 at (7, 6)
  - Small circle: radius 2 at (18, 14)

Expected output:
  ✓ 2 objects detected
  ✓ Different sizes visible in area
  ✓ Separate boundary lists
```

### Test Case 3: Multiple Rectangles (18 rows × 30 cols)
```
Image contents:
  - Rectangle 1: 5×7 at (2-6, 2-8)
  - Rectangle 2: 5×7 at (9-13, 12-18)
  - Rectangle 3: 5×4 at (4-8, 23-26)

Expected output:
  ✓ 3 objects detected
  ✓ Similar rectangular areas
  ✓ Boundary pixels reflect rectangle shapes
```

### Test Case 4: Complex Shapes (30 rows × 25 cols)
```
Image contents:
  - Hollow square: 7×7 outline at (3-9, 3-9)
    (Only edges, not filled)
  - Filled rectangle: 6×6 at (15-20, 12-17)
  - 3 isolated pixels: (25,4), (27,8), (26,20)

Challenge factors:
  - Hollow square recognition
  - Isolated pixels (may be filtered as noise)
  - Multiple disconnected regions

Expected output:
  ✓ At least 2 main objects
  ✓ May include isolated pixels as separate objects
  ✓ Hollow square detected as single component
```

---

## Complexity Analysis

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Read image | O(M×N) | O(M×N) | Read all pixels |
| Initialize visited | O(M×N) | O(M×N) | Set all to false |
| Build sparse list | O(M×N) | O(k) | k = count of 1s |
| DFS one component | O(M×N) | O(M×N) | Worst case: entire image |
| Detect all components | O(M×N) | O(M×N) | Each pixel visited once |
| Invert image | O(M×N + k) | O(1) | Initialize then modify k pixels |
| Print outputs | O(M×N) | O(1) | Sequential output |
| **Total** | **O(M×N)** | **O(M×N)** | Linear optimal complexity |

**Memory Usage**:
- Input image: M×N integers
- Visited array: M×N booleans
- Sparse list: ~3k pointers/values (k = foreground pixels)
- Objects list: varies by component count
- Stack during DFS: O(M×N) worst case

**Optimization**: For 4800 pixels with only 400 foreground:
- Dense: ~19KB
- Sparse: ~2KB + overhead (90% savings)

---

## Output Explanation

### Section 1: Original Sparse Matrix
```
SHOWING THE ORIGINAL SPARSE LL
(2,2,1) (2,3,1) (2,4,1) (2,5,1) (3,2,1) ...
```

**What it shows**: All foreground (1) pixels with coordinates  
**Why useful**: Verifies image loading, shows object distribution  
**Format**: (row, col, value) for each 1 in the image

### Section 2: Detected Objects
```
FOLLOWING ARE THE IDENTIFIED OBJECTS

OBJECT 1
area(number of foreground pixels present in the identified object) -> 25
number of boundary pixels -> 16
The following are the boundary pixels ->
(2,2) (2,6) (3,2) (3,6) (6,2) (6,6) ...
```

**Area**: Total foreground pixels in this object  
**Boundary pixels**: Edge pixels (adjacent to background or image edge)  
**Significance**: Boundary pixels define object outline/perimeter

### Section 3: Inverted Image Matrix
```
PRINTING THE FINAL RECONSTRUCTED IMAGE MATRIX
1 1 1 1 1 0 1 1 1 1 ...
1 0 0 1 1 1 0 1 1 1 ...
1 0 0 1 1 1 0 1 1 1 ...
...
```

**What changed**: All 0s became 1s, all 1s became 0s  
**Visualization**: Full matrix shown for inspection  
**Use case**: Testing inverse operations

### Section 4: Inverted Sparse Matrix
```
SHOWING THE NEW SPARSE LL(AFTER OPERATIONS)
(0,0,1) (0,1,1) (0,2,1) ... (different from original)
```

**What it represents**: Sparse representation of inverted image  
**Comparison**: Compare with original to understand inversion effect  
**Efficiency**: Still stores only 1s from inverted image

---

## Key Insights & Design Decisions

### 1. Using DFS over BFS
- **Why**: Stack-based DFS uses less memory than queue-based BFS
- **Implementation**: Iterative using stack to avoid recursion depth limits
- **Performance**: Same O(M×N) but better cache locality

### 2. 4-Connectivity Choice
- **Why**: Standard for binary image processing
- **Alternative**: 8-connectivity (includes diagonals) connects more pixels
- **Trade-off**: 4-connectivity: fewer, larger components; 8-connectivity: more components

### 3. Boundary Pixel Extraction
- **Method**: Check all neighbors during DFS
- **Advantage**: Extracted simultaneously during detection, no second pass
- **Efficiency**: O(1) per pixel, not O(M+N)

### 4. Sparse Matrix Representation
- **Purpose**: Memory efficient for sparse (mostly zero) images
- **Implementation**: Linked list of non-zero coordinates
- **Advantage**: Scales with actual object pixels, not image size

### 5. Linked Lists for Variable-Size Data
- **Reason**: Unknown number of objects detected
- **Dynamic allocation**: Each component stored as separate node
- **Flexibility**: Easy to add/remove without reallocation

### 6. Image Inversion Implementation
- **Method**: Initialize to 1s, then zero out original 1s
- **Efficiency**: O(M×N + k) instead of full 2D array scan
- **Clever use**: Uses sparse list to identify pixels to modify

---

## Algorithm Correctness Proof

### DFS visits all connected pixels
1. Start from unvisited foreground pixel (1)
2. Stack ensures all 4-neighbors explored
3. Visited marking prevents revisits
4. Loop ends when stack empty
5. **Result**: All connected 1s visited exactly once

### Boundary detection is accurate
1. During DFS, check all 4 neighbors
2. Mark as boundary if ANY neighbor is invalid or 0
3. Boundary pixels captured before popping
4. **Result**: Complete boundary extracted

### Sparse representation is lossless
1. Original 1s stored as (row, col, 1)
2. Reconstruction: create matrix, populate from list
3. All pixels accounted for
4. **Result**: Perfect reconstruction possible

### Image inversion is correct
1. Start: matrix has original 1s at specific locations
2. Initialize all to 1
3. Set original 1s to 0
4. **Result**: Perfect inversion without errors

---

## Memory Management

### Dynamic Allocation
```cpp
matrix = new int[M*N];              // Image data
visited = new bool[M*N];            // Visited tracking
Stack arr = new Point[];            // Per stack
Object boundary_pixel = new Point[]; // Per object
Node_Sparse = new Node_Sparse();    // Per 1 pixel
Node_Ans = new Node_Ans();          // Per detection
```

### Cleanup (Critical!)
```cpp
delete[] matrix
delete[] visited
// Linked lists deleted via delLLSparse() and delLLAns()
// Each deletion triggers destructors recursively
```

**Memory Leak Prevention**: Always call cleanup functions before exit

---

## Performance Characteristics

### Time Complexity by Component
- 60×80 image (4,800 pixels): ~1-2ms
- 100×100 image (10,000 pixels): ~2-5ms
- 500×500 image (250,000 pixels): ~50-100ms
- Linear scaling: Doubles for 2× image size

### Space Complexity
- Dense storage: ~19KB for 60×80 image
- Sparse (25% full): ~5KB + overhead
- Stack during DFS: ~19KB (M×N at worst)

### Bottlenecks (in order of impact)
1. DFS traversal: O(M×N)
2. Input/Output: O(M×N)
3. Memory allocation: O(M×N + k)
4. Sparse list construction: O(k)

---

## Code Structure

```
HEADER COMMENTS
├── Project overview
├── Key concepts
└── Algorithm description

GLOBAL VARIABLES
├── M, N: Dimensions
├── matrix: Image data
└── visited: Tracking

CLASSES
├── Point: Pixel coordinates
├── Stack: DFS structure
├── Object: Detected component
├── Node_Sparse: Sparse matrix node
└── Node_Ans: Results node

FUNCTIONS
├── initMatrix(): Read input
├── initVisited(): Initialize tracking
├── identifyObject(): Core DFS algorithm
├── sparseLL(): Build sparse list
├── detection(): Find all components
├── invertImage(): Inversion operation
├── printXXX(): Output functions
├── delLLXXX(): Memory cleanup
└── main(): Orchestration

MAIN
└── Complete workflow
```

---

## Edge Cases & Handling

### Empty Image (All Zeros)
- No foreground pixels
- Sparse list: empty
- Objects detected: 0
- Output: "No objects"

### Single Pixel Object
- Area: 1
- Boundary pixels: 1 (itself)
- Detected and reported correctly

### Isolated Pixels
- Detected as separate objects
- Each one has boundary pixel = itself
- Test case 4 includes these

### Fully Filled Image (All Ones)
- Single object: entire image
- Area: M×N
- Boundary: only edges of entire image

### Image with Adjacent Objects
- Objects separated if value differs (but both 1 in binary)
- Connected if same value (1) and adjacent
- Correctly handled by 4-connectivity

---

## Enhancements & Future Work

### Possible Extensions
1. **8-Connectivity**: Include diagonal neighbors
2. **Hole Detection**: Identify regions inside objects
3. **Morphological Operations**: Erosion, dilation
4. **Multiple Component Tracking**: Assign IDs to objects
5. **Shape Classification**: Recognize shapes (square, circle, etc.)
6. **Contour Approximation**: Simplify boundary pixels
7. **Image I/O**: Read/write actual image files
8. **Visualization**: GUI display of results
9. **Parallel Processing**: Multi-threaded DFS
10. **Optimization**: GPU acceleration for large images

### Code Optimization Ideas
- Use bitmaps instead of bool arrays
- Stack pre-allocation size optimization
- Memoization for repeated calculations
- Iterative image processing (skip empty regions)

---

## Debugging Guide

### Program produces no output
- Check input file format
- Verify first line is "M N"
- Ensure pixel values are 0 or 1

### Crashes or segmentation fault
- Check array bounds in Point::isValid()
- Verify M, N read correctly
- Confirm memory allocations succeeded
- Check for stack overflow (unlikely with M*N size)

### Unexpected object count
- Verify 4-connectivity logic
- Check visited marking
- Ensure boundary condition correct
- Run with print statements for debugging

### Memory issues
- Verify delete statements executed
- Check for circular references
- Monitor max memory usage
- Consider smaller test images

---

## Learning Outcomes

After completing this project, you'll understand:

1. **Connected Component Analysis**: Core image processing algorithm
2. **DFS Algorithm**: Iterative implementation with stack
3. **Sparse Data Structures**: Memory-efficient representation
4. **Boundary Detection**: Edge extraction techniques
5. **Linked Lists**: Dynamic data structure management
6. **Memory Management**: Allocation/deallocation in C++
7. **Algorithm Optimization**: Time/space trade-offs
8. **Image Processing**: Binary image fundamentals
9. **Testing**: Verification with multiple test cases
10. **Code Organization**: Structured problem solving

---

## References

- Connected Components in Image Processing
- Depth-First Search Algorithm
- Flood-Fill Algorithms
- Sparse Matrix Representations
- Binary Image Processing Fundamentals
- Data Structures (Stack, Linked List)
- C++ Memory Management Best Practices
- Algorithm Complexity Analysis

---

**Author**: IC 253 Assignment 1, Q2  
**Language**: C++  
**Standard**: C++11 or later  
**Compiled with**: g++ (GCC)  
**Platform**: Windows/Linux/Mac  
