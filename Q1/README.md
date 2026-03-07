# Object Detection in Grayscale Images (Rectangular & Circular Patterns)

## Project Overview

This project implements an **object detection system** for identifying and classifying specific object-like patterns in grayscale images. The system detects rectangular and circular objects, extracts their properties, classifies their shape type, and generates visual representations with bounding boxes using a stack-based region collection process.

## Problem Statement

Given a 2D grayscale image (M×N matrix of pixel intensities 0-255), design and implement an algorithm to detect specific object-like patterns such as:
- **Rectangular objects**: Continuous regions of similar pixel intensity forming rectangular shapes
- **Circular objects**: Clusters of pixels forming an approximate circular boundary

The program must traverse the matrix efficiently using appropriate Data Structures and Algorithms (DSA) concepts to detect and classify whether valid rectangular or circular patterns exist in the image.

## Solution Approach

### Region Collection and Shape Classification

The solution scans the image point by point and uses a stack to gather one full object region at a time:

1. **Read Image**: Load the complete M×N grayscale image into memory.
2. **Initialize Tracking**: Build the `visited` array with this rule:
   - intensity `0` pixels are marked `true`
   - all remaining pixels are marked `false`
3. **Scan & Detect**: Move through each and every pixel in row-major order.
4. **Start Region Collection**: Whenever a pixel is still unvisited, call the helper function `buildComponent(row, col)`.
5. **Inside `buildComponent(row, col)`**:
   - push the starting pixel into the stack
   - mark it visited
   - repeatedly remove one pixel from the stack and store the popped pixel in `curr_pixel`
   - inspect the neighbourhood of `curr_pixel` in the 4 direct directions: up, down, left, right
   - push a neighbour only if it is inside the image, has not been visited, and satisfies `|seed_intensity - neighbour_intensity| < T`
   - keep updating the boundary-box attributes of the current region: minimum row, maximum row, minimum column, maximum column, and area
6. **Noise Filtering**: Ignore very small regions whose area is less than 1% of the total image size.
7. **Classify Shape Type**: Use `areaRatio` and box shape to decide whether the detected region is rectangular or circular.
8. **Visualize**: Draw white bounding boxes around detected objects and display them with ASCII art.

## Data Structures

### Class: Point
```cpp
Represents: Single pixel coordinate
Members:
   - rownum: Row index
   - colnum: Column index
Methods:
   - isValid(): Check if the point lies inside the image bounds
```

### Class: Stack
```cpp
Purpose: Store pixels that still need to be processed in the current region
Operations:
   - push(Point): Add a pixel to the top
   - pop(): Remove the top pixel and return it
   - isEmpty(): Check whether the stack still contains pixels
Time Complexity: O(1) per operation
Capacity: M*N pixels in the worst case
```

### Class: Component
```cpp
Represents: One detected region in the grayscale image
Members:
   - minrow: Smallest row index in the region
   - maxrow: Largest row index in the region
   - mincol: Smallest column index in the region
   - maxcol: Largest column index in the region
   - area: Number of pixels in the region
Methods:
   - height(): Height of the boundary box
   - width(): Width of the boundary box
   - boxArea(): Area of the boundary box
   - areaRatio(): ratio = region_area / boundary_box_area
```

### Class: Node
```cpp
Represents: One node in the linked list of detected regions
Members:
   - component: The stored Component
   - type: Shape label ('r', 'c', or 'o')
   - next: Pointer to the next node
Purpose: Dynamic storage of all accepted regions
```

### Visited Array
```cpp
Represents: Boolean tracking array of size M*N
Initialization:
   - true for background pixels with intensity 0
   - false for object pixels with non-zero intensity
Purpose: Prevent revisiting pixels that have already been processed
```

### Result Linked List
```cpp
Head pointer: Ans_H
Tail pointer: Ans_T
Purpose: Store all detected regions efficiently as they are found
Benefit: New regions can be appended in O(1) time
```

## Algorithms

### Algorithm: Region Collection with `buildComponent(row, col)`
```cpp
Purpose: Extract one complete grayscale region starting from an unvisited seed pixel
Input:
   - start_row, start_col: coordinates of the seed pixel
Working steps:
   1. Create an empty Component object
   2. Push the seed pixel into the stack
   3. Mark the seed pixel as visited
   4. Repeatedly pop a pixel into curr_pixel while the stack is not empty
   5. Update the boundary-box attributes of the region:
       - minrow
       - maxrow
       - mincol
       - maxcol
       - area
   6. Inspect the neighbourhood of curr_pixel in 4 directions:
       - up    (-1, 0)
       - down  (+1, 0)
       - left  (0, -1)
       - right (0, +1)
   7. Push a neighbour only if:
       - it lies inside the image
       - it has not been visited yet
       - its intensity differs from the seed by less than T
Output:
   - complete Component containing area and boundary-box values
```

**4-connectivity used by the code**:
```text
            UP (-1, 0)
               |
               |
LEFT (0, -1) -- CENTER -- RIGHT (0, +1)
               |
               |
         DOWN (+1, 0)
```

### Algorithm: Full Image Detection with `detection()`
```cpp
Purpose: Scan the full image and identify all significant regions
Working steps:
   1. Traverse the image row by row and column by column
   2. When an unvisited non-background pixel is found, call buildComponent(row, col)
   3. Receive the extracted Component
   4. Apply noise filtering
   5. If the region area is larger than 1% of the image size, store it in the result linked list
Output:
   - linked list of all accepted regions through Ans_H and Ans_T
```

### Algorithm: Similarity Test
```cpp
Threshold used: T = 10
Condition:
   abs(seed_intensity - neighbour_intensity) < T
Meaning:
   - smaller T gives stricter grouping
   - larger T allows more variation inside one region
```

In this implementation, the intensity of the starting seed pixel is used as the reference while checking neighbouring pixels.

### Algorithm: Noise Filtering
```cpp
Purpose: Ignore tiny regions that are likely noise
Condition:
   component.area > 0.01 * (M * N)
Meaning:
   - only regions larger than 1% of the full image are kept
```

### Algorithm: Shape Classification
```cpp
areaRatio:
   areaRatio = area / boundingBoxArea

Rectangular region:
   areaRatio > 0.90

Circular region:
   0.60 < areaRatio < 0.85
   and the boundary box is nearly square

Other region:
   any region that does not satisfy the above rules
```

The code also checks that the width and height of the boundary box differ by less than 10% of the larger dimension before the region is labeled circular.

### Algorithm: Boundary Construction with `buildBoundary()`
```cpp
Purpose: Draw a white rectangular boundary around every detected region
Working steps for each region:
   1. Draw the left edge from minrow to maxrow at mincol
   2. Draw the right edge from minrow to maxrow at maxcol
   3. Draw the top edge from mincol to maxcol at minrow
   4. Draw the bottom edge from mincol to maxcol at maxrow
Drawing value:
   - 255, which is the brightest grayscale intensity
```

### Algorithm: Visual Rendering with `printVisualImage()`
```cpp
Purpose: Convert grayscale intensities into ASCII characters for terminal display
Mapping:
   - 0 to 30    -> space
   - 31 to 60   -> `
   - 61 to 100  -> -
   - 101 to 150 -> +
   - 151 to 200 -> @
   - 201 to 255 -> $
Result:
   - the original intensity pattern remains visible
   - the white bounding box appears clearly as $
```

## File Structure

```
Q1/
├── detection_copy.cpp          # Main detection algorithm (FULLY COMMENTED)
├── matrixgenerator.cpp         # Generates test case input files
├── input_tc1.in                # Test case 1: 1 Square + 1 Rectangle + 1 Circle
├── input_tc2.in                # Test case 2: Two Circles (different sizes)
├── input_tc3.in                # Test case 3: Multiple Rectangles
├── input_tc4.in                # Test case 4: Rectangle and Circle far apart
└── README.md                   # This file
```

## Quick Start

Follow these steps to compile the project, generate test inputs, and run the detection program on each test case.

**Prerequisites**: GCC compiler (`g++`) installed and available in `PATH`

## Input Format

```
M N
pixel[0][0] pixel[0][1] ... pixel[0][N-1]
pixel[1][0] pixel[1][1] ... pixel[1][N-1]
...
pixel[M-1][0] pixel[M-1][1] ... pixel[M-1][N-1]
```

- **M, N**: Image dimensions (rows, columns)
- **pixel values**: Grayscale intensities (0-255)
  - 0 = Black (background, ignored)
  - 1-254 = Grayscale object pixels
  - 255 = White (bounding box markers)

## Output Format

The program produces the following outputs:

### 1. Component Analysis
For each detected object, displays:
- Object number
- areaRatio (actual pixels / bounding box area)
- Shape classification: **'r'** (rectangular) or **'c'** (circular)
- Bounding box coordinates (min/max row/col)
- Dimensions (height, width)
- Bounding box area

### 2. Bounding Box Visualization
- White bounding boxes (pixel value 255) drawn around each detected object
- Marks the rectangular region containing each object
- Used to highlight and verify detected regions

### 3. Visual Representation
ASCII art visualization with character mapping:
- **`' '`** (space): Background pixels (0-30)
- **`` ` ``** (backtick): Very dark (31-60)
- **`-`** (dash): Dark (61-100)
- **`+`** (plus): Medium (101-150)
- **`@`** (at sign): Bright (151-200)
- **`$`** (dollar): White boundaries/bounding boxes (201-255)

The visualization is produced by converting each grayscale value range into one display character. This keeps the terminal output readable while still preserving brightness differences across the image. The white box is drawn by setting the boundary pixels to intensity `255`, which is why the box appears clearly as `$` in the final output.

## Compilation & Execution

### Step 1: Compile the test case generator
```bash
g++ matrixgenerator.cpp -o matrixgenerator.exe
```

### Step 2: Generate test input files
```bash
./matrixgenerator.exe
```
This creates: `input_tc1.in`, `input_tc2.in`, `input_tc3.in`, `input_tc4.in`

### Step 3: Compile the detection program
```bash
g++ detection.cpp -o detection.exe
```

### Step 4: Run detection on test cases
```bash
./detection.exe < input_tc1.in
./detection.exe < input_tc2.in
./detection.exe < input_tc3.in
./detection.exe < input_tc4.in
```

### What the generator produces

- **Test Case 1**: Mixed shapes (1 square + 1 rectangle + 1 circle)
- **Test Case 2**: Two circles with different sizes and intensities
- **Test Case 3**: Two rectangles with different dimensions
- **Test Case 4**: Rectangle and circle far apart
- **Image size used in the generated cases**: `60 x 80`

## Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|-----------------|------------------|
| Read image | O(M×N) | O(M×N) |
| Initialize visited | O(M×N) | O(M×N) |
| Process one region | O(M×N) | O(M×N) |
| Detection (all components) | O(M×N) | O(M×N) |
| Build boundaries | O(perimeter) | O(1) |
| Print visual | O(M×N) | O(1) |
| **Total** | **O(M×N)** | **O(M×N)** |

## Memory Management

- **Dynamic allocation**: `matrix`, `visited`, and component linked list nodes
- **Cleanup**: `freeHeapMemory()` releases all allocated memory
- **Stack-based processing**: Avoids recursive call stack overflow for large regions

The detected regions are stored in a linked list using `Ans_H` and `Ans_T`, so each accepted region can be added efficiently at the end of the result list.

## Code Structure

```
Global Variables
├── M, N: Image dimensions
├── matrix: Pixel data
├── visited: tracking array
└── T: Similarity threshold

Classes
├── Point: Pixel coordinates
├── Stack: working structure for region collection
├── Component: Detected object properties
└── Node: Linked list for storing regions

Functions
├── initMatrix(): Read image data
├── initVisited(): Initialize tracking
├── buildComponent(row, col): Gather one full region
├── detection(): Find all regions
├── printIdentifiedComponents(): Display results
├── buildBoundary(): Draw bounding boxes
├── printVisualImage(): ASCII visualization
└── freeHeapMemory(): Memory cleanup

Main Program
└── Orchestrates complete workflow
```

## Test Cases

### Test Case 1: Mixed Shapes
- 1 filled square (intensity 90)
- 1 filled rectangle (intensity 140)
- 1 filled circle (intensity 180)
- All separated, easily distinguishable

### Test Case 2: Two Circles
- Larger circle (radius 10, intensity 120)
- Smaller circle (radius 7, intensity 170)
- Test circular shape detection

### Test Case 3: Multiple Rectangles
- Large rectangle (intensity 100)
- Smaller rectangle (intensity 150)
- Test rectangular shape detection

### Test Case 4: Separated Shapes
- Rectangle (top-left, intensity 80)
- Circle (bottom-right, intensity 160)
- More challenging: must identify multiple distinct components

## Customization

### Adjusting Sensitivity
Change threshold T in line:
```cpp
int T = 10;  // Modify this value
```

### Changing Noise Filter
Modify component area threshold in `detection()`:
```cpp
if (component.area > 0.01*(M*N))  // Change 0.01 (1%) as needed
```

### Changing Test Case Parameters
Edit `matrixgenerator.cpp`:
- Modify M, N for different image sizes
- Change shape coordinates, sizes, and intensities
- Add more test cases in `generateTestCase()`

## Expected Output

For each detected component:
```
Component 1
AreaRatio -> 0.98
Type -> r
PROPERTIES OF THE BOUNDARY BOX
Min Row 5
Max Row 16
Min Col 5
Max Col 16
Height 12
Width 12
Box Area 144

[Visual representation follows]
```

## Output Interpretation Guide

### areaRatio Range

- **Above 0.90**: Likely a rectangle because the region fills most of its box
- **0.60 - 0.85**: Likely a circle when the boundary box is also nearly square
- **Outside these ranges**: Irregular or other shape

### Type Field

- **`r`**: Rectangular region
- **`c`**: Circular region
- **`o`**: Other irregular region

### Visual Markers

- space: background
- grayscale characters: object pixels with different brightness
- **`$`**: bounding-box pixels drawn by the program

## Edge Cases Handled

1. **Empty image**: No components → "no region identified"
2. **Tiny components**: Automatic filtering as noise
3. **Touching components**: Different intensities → separate detection
4. **Boundary pixels**: Correctly included in bounding box
5. **Out-of-bounds**: Boundary checking prevents crashes

## Algorithmic Advantages

✓ **O(M×N) optimal complexity**: Each pixel visited exactly once  
✓ **Stack-based processing**: No recursion depth limit issues  
✓ **4-connectivity**: Finds true adjacent components  
✓ **Noise filtering**: Built-in outlier removal  
✓ **Shape classification**: Automatic type detection  
✓ **Visual feedback**: ASCII art for quick verification  

## Troubleshooting

### Problem: `g++` not found

- Install GCC or MinGW/MSVC as appropriate for your system
- Ensure the compiler is available in `PATH`

### Problem: Compilation errors

- Check C++ syntax in the source files
- Use at least C++11: `g++ -std=c++11 detection_copy.cpp -o detection.exe`
- Ensure file names match exactly

### Problem: Input file not found

- Run `matrixgenerator.exe` first to create the input files
- Confirm that `input_tc1.in` to `input_tc4.in` exist in the current folder

### Problem: Program crashes or shows no result

- Check that the input format is correct
- Verify that dimensions match the amount of pixel data
- Confirm pixel values stay within `0` to `255`
- If no object is reported, the region may have been filtered out by the 1% noise threshold

## Success Criteria

You have successfully run Q1 when:

- all 4 input files are generated without errors
- the detector compiles without errors
- each test case produces component output
- areaRatio, type, and bounding-box values are shown
- the visual output displays the bounding boxes clearly
- the program exits without runtime or memory errors

## Key Insights

1. **Linear indexing** for 2D arrays: `arr[i][j]` → `arr[i*N+j]` improves cache locality
2. **Stack-based traversal**: Using an explicit stack keeps the processing order simple and avoids recursion
3. **Visited tracking**: Eliminates redundant processing and cycle detection
4. **Neighbour-based region growth**: Pixels are grouped by local intensity similarity and 4-neighbour checks
5. **Shape metrics**: areaRatio helps separate dense rectangular regions from circular ones
6. **Linked lists**: Dynamic data structure perfect for unknown component count

---

**Author**: DAKSH RATHI  
**Date**: 2026
**Language**: C++  
**Standard**: C++11 or later
