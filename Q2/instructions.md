# Binary Image Object Detection & Inversion - Execution Instructions

## Project Overview

This Q2 project processes binary images (pixels: 0 or 1) to:
1. **Detect Objects**: Identify connected components (foreground regions)
2. **Extract Boundaries**: Find edge pixels for each detected object
3. **Compress Data**: Represent images using sparse matrix linked lists
4. **Invert Image**: Swap foreground (1) and background (0)
5. **Reconstruct**: Generate sparse representation of inverted image

---

## Quick Start

**Prerequisites**: GCC compiler (g++) installed and available in PATH

### Compile & Run
```bash
# Step 1: Compile generator
g++ matrixgenerator.cpp -o matrixgenerator.exe

# Step 2: Generate test cases
./matrixgenerator.exe

# Step 3: Compile detector
g++ detection_copy.cpp -o detection.exe

# Step 4: Run on test cases
./detection.exe < input_tc1.in
./detection.exe < input_tc2.in
./detection.exe < input_tc3.in
./detection.exe < input_tc4.in
```

---

## Detailed Execution Steps

### Step 1: Compile Test Case Generator

```bash
g++ matrixgenerator.cpp -o matrixgenerator.exe
```

**What this does**:
- Compiles the binary image generator program
- Creates executable `matrixgenerator.exe`
- No errors = successful compilation

**Generator creates 4 test cases**:

| Test Case | Description | Dimensions |
|-----------|-------------|------------|
| TC1 | 1 Square + 1 Rectangle + 1 Circle | 20×25 |
| TC2 | 2 Circles (different sizes) | 25×20 |
| TC3 | 3 Rectangles (various positions) | 18×30 |
| TC4 | 1 Hollow Square + Rectangle + Isolated Pixels | 30×25 |

**About the generator**:
- Creates test images with known components
- Useful for verifying detection accuracy
- Can be modified to generate custom images
- Images saved as `input_tc*.in` files

---

### Step 2: Generate Test Input Files

```bash
./matrixgenerator.exe
```

**Expected output**:
```
input_tc1.in generated
input_tc2.in generated
input_tc3.in generated
input_tc4.in generated
```

**What each file contains**:

Each input file has format:
```
M N
row_0_values
row_1_values
...
row_M-1_values
```

Example (first few lines):
```
20 25
0 0 0 0 0 1 0 0 0 0 ... (25 values)
0 1 1 0 0 0 1 0 0 0 ... (25 values)
0 1 1 0 0 0 1 0 0 0 ... (25 values)
...
```

Where:
- `0` = Background pixel (ignored)
- `1` = Foreground pixel (object pixel)

---

### Step 3: Compile Detection Program

```bash
g++ detection_copy.cpp -o detection.exe
```

**What this does**:
- Compiles the binary image processing algorithm
- Uses DFS-based flood-fill for component detection
- Creates executable `detection.exe`

**Key program features**:
- Detects connected components (objects)
- Identifies boundary pixels (edges)
- Represents images using sparse matrices
- Inverts binary images
- Outputs detailed analysis

---

### Step 4: Run Detection on Each Test Case

Execute detection on each test file:

```bash
./detection.exe < input_tc1.in
```

Then repeat for tc2, tc3, tc4:
```bash
./detection.exe < input_tc2.in
./detection.exe < input_tc3.in
./detection.exe < input_tc4.in
```

---

## Output Format & Interpretation

### Output Section 1: Original Sparse Matrix

```
SHOWING THE ORIGINAL SPARSE LL
(2,2,1) (2,3,1) (2,4,1) (2,5,1) (3,2,1) (3,3,1) ...
```

**Explanation**:
- Lists all foreground pixels (value 1) as (row, col, value)
- Sparse representation: only stores 1s, not 0s
- Useful for memory-efficient image storage
- Format: `(row,col,value)` separated by spaces

**Example interpretation**:
- `(2,2,1)` = Pixel at row 2, column 2 has value 1
- `(2,3,1)` = Pixel at row 2, column 3 has value 1
- And so on...

---

### Output Section 2: Detected Objects

```
FOLLOWING ARE THE IDENTIFIED OBJECTS

OBJECT 1
area(number of foreground pixels present in the identified object) -> 16
number of boundary pixels -> 12
The following are the boundary pixels ->
(2,2) (2,5) (3,2) (3,5) (4,2) (4,5) (5,2) (5,3) (5,4) (5,5) ...

OBJECT 2
area(number of foreground pixels present in the identified object) -> 35
number of boundary pixels -> 20
The following are the boundary pixels ->
...
```

**Explanation**:
- Each object is a connected component of foreground pixels
- **Area**: Total count of foreground pixels in the object
- **Boundary Pixels**: Edge pixels touching background or image boundary
- **Boundary coordinates**: List of all edge pixel locations

**What boundary pixels tell us**:
- Region's perimeter length (approximately)
- Shape outline for visual reconstruction
- Used for contour analysis and shape classification

**Example**: A square object and a circle will have different boundary patterns
- Square: Corners appear in boundary list
- Circle: More distributed boundary around edges

---

### Output Section 3: Inverted Image Matrix

```
PRINTING THE FINAL RECONSTRUCTED IMAGE MATRIX
1 1 1 1 1 0 1 1 1 1 1 ...
1 0 0 1 1 1 0 1 1 1 1 ...
1 0 0 1 1 1 0 1 1 1 1 ...
...
```

**Explanation**:
- Shows the inverted binary image (all 0s and 1s swapped)
- Original foreground (1) becomes background (0)
- Original background (0) becomes foreground (1)
- Displayed as M rows × N columns grid
- Space-separated values for readability

**Use cases for inversion**:
- Analyzing background regions instead of objects
- Testing inverse operations
- Special image processing techniques

---

### Output Section 4: Inverted Sparse Matrix

```
SHOWING THE NEW SPARSE LL(AFTER OPERATIONS)
(0,0,1) (0,1,1) ... (different coordinates from original)
```

**Explanation**:
- Sparse representation of the inverted image
- Contains coordinates of the NEW foreground (what was background)
- Allows efficient storage of inverted image
- Shows which background pixels became foreground

**Why this matters**:
- Compare original and inverted sparse lists to understand image structure
- Verify inversion was correct by checking sizes
- Efficient representation for further processing

---

## Algorithm Summary

### Component Detection (DFS Flood-Fill)

```
For each unvisited foreground pixel:
  1. Start DFS from this seed pixel
  2. Use stack to track pixels to process
  3. Explore 4-connected neighbors
  4. Include neighbor if it's foreground (1) and unvisited
  5. Mark current pixel as boundary if:
     - Neighbor is out of bounds, OR
     - Neighbor is background (0)
  6. Store all boundary pixels
  Return: Complete object with area and boundaries
```

**4-Connectivity** (adjacent means sharing an edge):
```
      UP
      ↓
LEFT ← X → RIGHT
      ↑
    DOWN
```

### Sparse Matrix Representation

**Original image** (inefficient):
- Store all M×N pixels (mostly zeros)
- Memory: M×N integers

**Sparse representation** (efficient):
- Store only non-zero pixels (ones)
- Memory: ~(count of 1s) × 3 integers
- Linked list stores: row, col, value for each 1

**Efficiency gain**:
- 60×80 image = 4,800 pixels
- If only 400 pixels are 1s: 4,400 pixels saved (92% reduction!)

---

## Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| Read image | O(M×N) | O(M×N) |
| Build sparse list | O(M×N) | O(count of 1s) |
| Detect one object (DFS) | O(M×N) | O(M×N) |
| Detect all objects | O(M×N) | O(M×N) |
| Invert image | O(M×N) | O(1) |
| Print matrix | O(M×N) | O(1) |
| **Total** | **O(M×N)** | **O(M×N)** |

---

## Test Case Details

### Test Case 1: Mixed Shapes (20×25)
- **Square**: Filled 4×4 pixels at (2-5, 2-5)
- **Rectangle**: Filled 5×7 pixels at (6-10, 10-16)
- **Circle**: Radius 3 centered at (15, 19)
- **Expected**: 3 distinct objects detected

### Test Case 2: Two Circles (25×20)
- **Circle 1**: Radius 3 centered at (7, 6)
- **Circle 2**: Radius 2 centered at (18, 14)
- **Expected**: 2 circular objects with different sizes

### Test Case 3: Multiple Rectangles (18×30)
- **Rectangle 1**: 5×7 pixels at (2-6, 2-8)
- **Rectangle 2**: 5×7 pixels at (9-13, 12-18)
- **Rectangle 3**: 5×4 pixels at (4-8, 23-26)
- **Expected**: 3 objects, possibly some overlapping boundaries

### Test Case 4: Hollow Square & Rectangle (30×25)
- **Hollow Square**: Outline 7×7 at (3-9, 3-9)
  - Hollow = only edges pixels, not filled
  - Recognition challenge: single connected component
- **Filled Rectangle**: 6×6 pixels at (15-20, 12-17)
- **Isolated Pixels**: 3 single pixels (possibly noise)
- **Expected**: 2-5 objects depending on noise handling

---

## Key Concepts Explained

### Connected Component
- Group of adjacent foreground pixels connected through 4-connectivity
- All pixels in a component must reach each other through foreground neighbors
- Objects are separated if no path exists between them

### Boundary Pixel
- Foreground pixel (1) adjacent to:
  - At least one background pixel (0), OR
  - Image boundary (out of bounds)
- Boundary pixels form the perimeter/outline of an object

### Sparse Matrix
- Efficient representation storing only non-zero elements
- Saves memory for images with mostly zeros (common in binary images)
- Linked list implementation: flexible size, efficient storage

### Image Inversion
- Logical NOT operation: flip all bits
- 0 becomes 1, 1 becomes 0
- Used in image processing for various operations
- Symmetry analysis: useful for understanding image content

---

## Memory Management

### Allocation
```cpp
matrix = new int[M*N]           // Pixel data
visited = new bool[M*N]         // Visited tracking
boundary_pixel = new Point[...]  // Per object
sparse list nodes = new Node_Sparse[...]
results list nodes = new Node_Ans[...]
```

### Deallocation (cleanup at end)
```cpp
delete[] matrix
delete[] visited
delLLSparse(Sparse_H, Sparse_T)  // Sparse linked list
delLLAns(Ans_H, Ans_T)           // Results linked list
```

**Important**: Always free memory to avoid memory leaks

---

## Running All Tests at Once

### Option 1: Bash/Git Bash Script

Create `run_all_tests.sh`:
```bash
#!/bin/bash
echo "Compiling..."
g++ matrixgenerator.cpp -o generator.exe
g++ detection_copy.cpp -o detector.exe

echo "Generating test cases..."
./generator.exe

echo "Running all tests..."
for i in 1 2 3 4; do
  echo ""
  echo "====== TEST CASE $i ======"
  ./detector.exe < input_tc$i.in
done
```

Then run:
```bash
bash run_all_tests.sh
```

### Option 2: Windows PowerShell

```powershell
g++ matrixgenerator.cpp -o matrixgenerator.exe; ./matrixgenerator.exe
g++ detection_copy.cpp -o detection.exe
@(1,2,3,4) | ForEach-Object { "=== TEST CASE $_===" ; ./detection.exe < input_tc$_.in }
```

---

## Troubleshooting

### Problem: Compilation fails
**Solution**:
- Check C++ file syntax
- Use: `g++ -std=c++11 detection_copy.cpp -o detection.exe`
- Ensure all headers are present
- Check for typos in class/function names

### Problem: "File not found" error
**Solution**:
- Run matrixgenerator.exe first to create input files
- Check files exist: `input_tc1.in`, `input_tc2.in`, etc.
- Verify working directory
- Use absolute paths if needed

### Problem: Program crashes or segmentation fault
**Solution**:
- Ensure input dimensions match actual data
- Check pixel values are 0 or 1
- Verify input file format is correct
- Check memory allocation success
- May indicate out-of-bounds access

### Problem: No output or empty output
**Solution**:
- Check if there are actually foreground pixels in the image
- Verify input file isn't all zeros
- Check program didn't crash silently
- Test with different test case

### Problem: Memory issues/slow performance
**Solution**:
- Large images consume more memory and time
- For M=100, N=100: ~10KB data + overhead
- If too slow, reduce image size
- Check for infinite loops (unlikely) or large intermediate data

---

## Expected Results Summary

### Test Case 1
✓ Should detect 3 objects  
✓ One square, one rectangle, one circle  
✓ Different boundary patterns for each

### Test Case 2
✓ Should detect 2 objects  
✓ Both circular  
✓ Different sizes apparent in area

### Test Case 3
✓ Should detect 3 objects  
✓ All rectangular  
✓ Possibly overlapping regions due to proximity

### Test Case 4
✓ Should detect at least 2 main objects  
✓ Hollow square detected as single component  
✓ Filled rectangle clearly separated  
✓ Isolated pixels may be detected or ignored

---

## Success Checklist

Program runs successfully when:
- ✓ All input files generated without errors
- ✓ Detection program compiles without errors
- ✓ Program processes each test case
- ✓ Sparse matrix shown for original image
- ✓ Objects detected with area and boundaries
- ✓ Boundary pixels listed correctly
- ✓ Inverted image displayed as matrix
- ✓ New sparse matrix shown for inverted image
- ✓ All output properly formatted
- ✓ No crashes or memory errors
- ✓ Program terminates cleanly

---

## Customization Guide

### Modify Image Size
In `matrixgenerator.cpp`:
```cpp
if(tc==1){
    M = 20;  // Change rows
    N = 25;  // Change columns
    // ...
}
```

### Modify Test Case Content
Replace shape generation code in `generateTestCase()`:
```cpp
// Your custom shapes here
for(int i=...; i<=...; i++)
    for(int j=...; j<=...; j++)
        a[i*N+j] = 1;  // Set foreground
```

### Add New Test Case
```cpp
if(tc==5){
    M = 30;
    N = 40;
    // ... generate custom binary image
}
```

### Create Custom Input File
Text editor format:
```
M N
pixel_row0_col0 pixel_row0_col1 ... pixel_row0_colN-1
pixel_row1_col0 pixel_row1_col1 ... pixel_row1_colN-1
...
```

---

## Performance Expectations

| Operation | Time |
|-----------|------|
| Compile generator | ~0.1s |
| Generate 4 test cases | ~0.05s |
| Compile detector | ~0.1s |
| Run detection on one test | ~0.02s |
| Run all 4 tests | ~0.1s |
| **Total end-to-end** | **~0.4s** |

---

## Next Steps

After successful execution:
1. Analyze output - verify object detection accuracy
2. Compare boundary pixels between objects
3. Verify sparse representation efficiency
4. Create custom test cases to challenge the algorithm
5. Study the fully-commented `detection_copy.cpp` code
6. Explore modifications and extensions
7. Review README.md for comprehensive documentation

---

## References

- Depth-First Search (DFS) algorithm
- Connected components in graphs
- Flood-fill algorithms
- Sparse matrix representations
- Binary image processing fundamentals
- Linked list data structures
