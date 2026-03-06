# Object Detection in Grayscale Images (Rectangular & Circular Patterns)

## Project Overview

This project implements an **object detection system** for identifying and classifying specific object-like patterns in grayscale images. The system detects rectangular and circular objects, extracts their properties, classifies their shape type, and generates visual representations with bounding boxes using Depth-First Search (DFS) based flood-fill algorithm.

## Problem Statement

Given a 2D grayscale image (M×N matrix of pixel intensities 0-255), design and implement an algorithm to detect specific object-like patterns such as:
- **Rectangular objects**: Continuous regions of similar pixel intensity forming rectangular shapes
- **Circular objects**: Clusters of pixels forming an approximate circular boundary

The program must traverse the matrix efficiently using appropriate Data Structures and Algorithms (DSA) concepts to detect and classify whether valid rectangular or circular patterns exist in the image.

## Solution Approach

### Algorithm: DFS-based Flood Fill with Shape Classification

The solution uses an **iterative DFS** (Depth-First Search) implementation with a stack to identify and classify connected components:

1. **Read Image**: Load M×N grayscale image into memory
2. **Initialize Tracking**: Mark background pixels (0) as visited, object pixels as unvisited
3. **Scan & Detect**: For each unvisited object pixel:
   - Launch DFS flood-fill to extract the entire connected component
   - Record bounding box coordinates and pixel count
   - Filter out noise (components < 1% of total image area)
4. **Classify Shape Type**: Determine if object is rectangular or circular based on fill ratio:
   - **Circular**: Fill ratio 0.65-0.90 (π/4 ≈ 0.785) AND bounding box is nearly square
   - **Rectangular**: Fill ratio ≥ 0.95 (dense, fills most of bounding box)
5. **Visualize**: Draw white bounding boxes around detected objects and display with ASCII art

### Key Data Structures

- **Point Class**: Represents pixel coordinates with boundary validation
- **Stack Class**: Implements LIFO structure for DFS traversal
- **Component Class**: Stores detected component's bounding box and metrics
- **Node Class**: Linked list node for storing detected components
- **Visited Array**: Boolean tracking for DFS algorithm

## File Structure

```
Q1/
├── detection_copy.cpp          # Main detection algorithm (FULLY COMMENTED)
├── matrixgenerator.cpp         # Generates test case input files
├── input_tc1.in                # Test case 1: 1 Square + 1 Rectangle + 1 Circle
├── input_tc2.in                # Test case 2: Two Circles (different sizes)
├── input_tc3.in                # Test case 3: Multiple Rectangles
├── input_tc4.in                # Test case 4: Rectangle and Circle far apart
├── instructions.md             # Project execution instructions
└── README.md                   # This file
```

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
- Fill ratio (actual pixels / bounding box area)
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
g++ detection_copy.cpp -o detection.exe
```

### Step 4: Run detection on test cases
```bash
./detection.exe < input_tc1.in
./detection.exe < input_tc2.in
./detection.exe < input_tc3.in
./detection.exe < input_tc4.in
```

## Algorithm Details

### 4-Connectivity for Component Detection
Components are identified using **4-connectivity** (adjacent pixels share an edge):
```
      UP (−1,0)
        ↓
LEFT(0,−1) ← CENTER → RIGHT(0,+1)
        ↑
     DOWN (1,0)
```

### Similarity Threshold (T)
- **Default T = 10**: Maximum intensity difference between adjacent pixels
- Neighbors included if `|seed_intensity - neighbor_intensity| < T`
- **Smaller T** → Stricter grouping → More components detected
- **Larger T** → Looser grouping → Fewer, larger components

### Noise Filtering
Components with area < 1% of total image area are filtered out:
```
minimum_pixels = 0.01 × M × N
```

### Shape Classification

**Fill Ratio** = (actual_pixels) / (bounding_box_area)

#### Rectangular Components
- Fill ratio ≥ 0.95 (dense, fills box)
- Example: Perfect rectangle fills 100%

#### Circular Components
- Fill ratio: 0.65 - 0.90
- Theoretical: π/4 ≈ 0.785 (78.5%)
- Bounding box must be nearly square (aspect ratio ~1:1)

#### Other Components
- Fill ratio outside above ranges
- Irregular or unusual shapes

## Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|-----------------|------------------|
| Read image | O(M×N) | O(M×N) |
| Initialize visited | O(M×N) | O(M×N) |
| DFS for one component | O(M×N) | O(M×N) |
| Detection (all components) | O(M×N) | O(M×N) |
| Build boundaries | O(perimeter) | O(1) |
| Print visual | O(M×N) | O(1) |
| **Total** | **O(M×N)** | **O(M×N)** |

## Memory Management

- **Dynamic allocation**: `matrix`, `visited`, and component linked list nodes
- **Cleanup**: `freeHeapMemory()` releases all allocated memory
- **Stack-based DFS**: Avoids recursive call stack overflow for large components

## Code Structure

```
Global Variables
├── M, N: Image dimensions
├── matrix: Pixel data
├── visited: DFS tracking array
└── T: Similarity threshold

Classes
├── Point: Pixel coordinates
├── Stack: DFS working structure
├── Component: Detected object properties
└── Node: Linked list for storing components

Functions
├── initMatrix(): Read image data
├── initVisited(): Initialize tracking
├── buildComponent(row, col): DFS flood-fill
├── detection(): Find all components
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
FillRatio -> 0.98
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

## Edge Cases Handled

1. **Empty image**: No components → "no region identified"
2. **Tiny components**: Automatic filtering as noise
3. **Touching components**: Different intensities → separate detection
4. **Boundary pixels**: Correctly included in bounding box
5. **Out-of-bounds**: Boundary checking prevents crashes

## Performance Notes

- **Image size M×N = 60×80 = 4,800 pixels**: ~1000x faster than O(n²) brute force
- **Memory usage**: O(M×N) = ~19KB for test cases
- **Linear scaling**: Doubling image size doubles computation time (linear growth)

## Algorithmic Advantages

✓ **O(M×N) optimal complexity**: Each pixel visited exactly once  
✓ **Iterative DFS**: No recursion depth limit issues  
✓ **4-connectivity**: Finds true adjacent components  
✓ **Noise filtering**: Built-in outlier removal  
✓ **Shape classification**: Automatic type detection  
✓ **Visual feedback**: ASCII art for quick verification  

## Future Enhancements

- 8-connectivity support (include diagonal neighbors)
- Moment-based shape descriptors (circularity, elongation)
- Contour tracing for boundary extraction
- Image preprocessing (blur, threshold) capabilities
- Multiple input image formats (PNG, JPG)
- Interactive visualization with GUI
- Parallel DFS for very large images

## Key Insights

1. **Linear indexing** for 2D arrays: `arr[i][j]` → `arr[i*N+j]` improves cache locality
2. **DFS over BFS**: Stack-based DFS uses less memory and finds components efficiently
3. **Visited tracking**: Eliminates redundant processing and cycle detection
4. **Flood-fill paradigm**: Standard technique in image processing (paint bucket tool)
5. **Shape metrics**: Fill ratio elegantly captures shape information
6. **Linked lists**: Dynamic data structure perfect for unknown component count

## References

- Depth-First Search (DFS) algorithm fundamentals
- Flood-fill algorithms in image processing
- Connected components analysis
- Image morphology and shape classification
- Memory-efficient DSA implementations

---

**Author**: IC 253 Assignment 1  
**Date**: 2026
**Language**: C++  
**Standard**: C++11 or later
