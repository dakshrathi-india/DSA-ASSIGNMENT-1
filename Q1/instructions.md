# Object Detection in Grayscale Images - Execution Instructions

## Project Overview

This project detects and classifies rectangular and circular objects in grayscale images using Depth-First Search (DFS) flood-fill algorithm. The system identifies connected regions of similar intensity and classifies them as either rectangular or circular patterns.

## Quick Start

Follow these steps to compile the project, generate test inputs, and run the detection algorithm on each test case.

**Prerequisites**: GCC compiler (g++) installed and available in PATH

---

## Detailed Steps

### Step 1: Compile the Test Case Generator

First, compile `matrixgenerator.cpp` which creates 4 test input files:

```bash
g++ matrixgenerator.cpp -o matrixgenerator.exe
```

**What this does**:
- Compiles the grayscale image generator program
- Creates executable `matrixgenerator.exe`
- No error means compilation succeeded

**What matrixgenerator.cpp contains**:
- Test Case 1: Mixed shapes (1 square + 1 rectangle + 1 circle) - tests multi-object detection
- Test Case 2: Two circles with different sizes and intensities - tests circular pattern detection
- Test Case 3: Two rectangles with different dimensions - tests rectangular pattern detection
- Test Case 4: Rectangle and circle far apart - tests separated object detection
- Image dimensions: 60 pixels (height) × 80 pixels (width)
- All images use grayscale intensities (0-255)

---

### Step 2: Generate Test Input Files

Run the compiled generator:

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

**Files created** in the same directory:

| File | Description |
|------|-------------|
| `input_tc1.in` | Test Case 1: Mixed shapes (square, rectangle, circle) |
| `input_tc2.in` | Test Case 2: Two circles of different sizes |
| `input_tc3.in` | Test Case 3: Two rectangles of different dimensions |
| `input_tc4.in` | Test Case 4: Rectangle and circle separated far apart |

Each file contains:
- First line: `60 80` (image dimensions: 60 rows, 80 columns)
- Next 60 lines: 80 pixel intensity values (0-255) per line
- 0 = background (ignored)
- 1-254 = object pixels with various intensities
- 255 = boundaries (marked by the detection algorithm)

---

### Step 3: Compile the Detection Program

Compile the main detection algorithm:

```bash
g++ detection_copy.cpp -o detection.exe
```

**What this does**:
- Compiles the object detection and classification algorithm
- Uses DFS-based flood-fill to identify objects
- Classifies objects as rectangular or circular
- Creates executable `detection.exe`

**Key program features**:
- Reads grayscale image dimensions and pixel data
- Detects connected components (objects) using DFS
- Classifies each object as rectangular or circular based on fill ratio and bounding box shape
- Draws bounding boxes around detected objects
- Outputs detailed analysis and ASCII visualization
- Automatically manages memory cleanup

---

### Step 4: Run Detection on Each Test Case

Execute the detection program on each test input:

```bash
./detection.exe < input_tc1.in
```

```bash
./detection.exe < input_tc2.in
```

```bash
./detection.exe < input_tc3.in
```

```bash
./detection.exe < input_tc4.in
```

**What happens**:
1. Program reads the grayscale image from input file
2. Scans image systematically (left to right, top to bottom)
3. For each unvisited object pixel, launches DFS to extract entire connected region
4. Calculates properties: bounding box, area, fill ratio
5. Classifies each object as rectangular or circular based on shape metrics
6. Draws white bounding boxes (sets pixels to 255)
7. Displays 3 outputs:
   - **Object Analysis**: Detailed metrics for each detected rectangular/circular object
   - **Visual Representation**: ASCII art showing detected bounding boxes
   - **Shape Classification**: 'r' for rectangular, 'c' for circular

---

## Expected Output Format

### Component Analysis Output
```
The identified components are ->

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
...
```

Where:
- **FillRatio**: (actual_pixels) / (bounding_box_area)
  - ≥ 0.95 = **Rectangular object** (dense, fills most of bounding box)
  - 0.65-0.90 = **Circular object** (matches theoretical π/4 ≈ 0.785 for perfect circle)
  - Outside these ranges = Non-matching patterns (filtered or classified as "Other")
- **Type**: Shape classification result
  - `r` = **Rectangular** - continuous region with high fill ratio
  - `c` = **Circular** - clustered region forming approximate circular boundary
  - `o` = **Other** - irregular patterns not matching rectangular or circular criteria
- **Min/Max Row/Col**: Bounding box coordinates - rectangular region containing the object
- **Height/Width**: Dimensions of the bounding box
- **Box Area**: Total area of bounding box region

### Visual Representation Output
```
Visual Representation

        
  `````        
  `   `        $$$$$$$
  ` o `        $     $
  `   `        $  +  $
  `````        $     $
               $$$$$$$
```

Character meanings:
- ` ` (space): Background pixels
- `` ` ``: Very dark pixels (30-60)
- `-`: Dark pixels (60-100)
- `+`: Medium pixels (100-150)
- `@`: Bright pixels (150-200)
- `$`: White pixels (200-255) = **Bounding boxes**

---

## Running Multiple Tests Quickly

### Option 1: Run all tests with a script (Linux/Mac/Git Bash)

Create a file `run_all_tests.sh`:
```bash
#!/bin/bash
echo "Compiling matrix generator..."
g++ matrixgenerator.cpp -o generator.exe
echo "Generating test cases..."
./generator.exe
echo "Compiling detection program..."
g++ detection_copy.cpp -o detector.exe
echo "Running all tests..."
for i in 1 2 3 4; do
  echo ""
  echo "================ TEST CASE $i ================"
  ./detector.exe < input_tc$i.in
done
```

Then run:
```bash
bash run_all_tests.sh
```

### Option 2: Run all tests sequentially (Windows PowerShell)

```powershell
g++ matrixgenerator.cpp -o matrixgenerator.exe; ./matrixgenerator.exe
g++ detection_copy.cpp -o detection.exe
@(1,2,3,4) | ForEach-Object { "=== TEST CASE $_ ===" ; ./detection.exe < input_tc$_.in }
```

---

## Troubleshooting

### Problem: "g++ not found" or "command not found"
**Solution**: GCC compiler not installed or not in PATH
- Install GCC: `apt-get install g++` (Linux) or use MinGW/MSVC on Windows
- Ensure g++ is in system PATH

### Problem: Compilation errors
**Solution**:
- Check C++ syntax in source files
- Use at least C++11: `g++ -std=c++11 detection_copy.cpp -o detection.exe`
- Ensure file names match exactly

### Problem: "File not found" when running detection
**Solution**:
- Ensure input files exist: check for `input_tc1.in`, etc.
- Run matrixgenerator.exe first to create files
- Check current working directory matches file location

### Problem: Program crashes or seg fault
**Solution**:
- Ensure input files are properly formatted
- Check that images have correct dimensions (M×N)
- Verify pixel values are integers in range [0, 255]

### Problem: No output or empty output
**Solution**:
- Check if components are being detected (noise filter may exclude all components)
- Reduce noise threshold in code: change `0.01*(M*N)` to smaller value
- Verify input image contains non-zero pixels

---

## Algorithm Summary

**What the detection program does**:

1. **Read Image**: Load M×N matrix of pixel intensities
2. **Initialize**: Mark background pixels (0) as visited
3. **Detect Components**: 
   - Scan entire image systematically
   - For each unvisited pixel, launch DFS
   - Extract all connected pixels with similar intensity
   - Filter out noise (components < 1% of image area)
4. **Analyze**: Calculate metrics (area, fill ratio, bounding box)
5. **Classify**: Determine shape type (circular/rectangular/other)
6. **Visualize**: Draw boundaries and print results

**Time Complexity**: O(M×N) - linear in image size  
**Space Complexity**: O(M×N) - for image and tracking arrays

---

## Input File Format

If creating custom test cases, follow this format:

```
M N
row0_col0 row0_col1 ... row0_colN-1
row1_col0 row1_col1 ... row1_colN-1
...
rowM-1_col0 rowM-1_col1 ... rowM-1_colN-1
```

Example (5×5 image):
```
5 5
0 0 0 0 0
0 100 100 0 0
0 100 100 0 0
0 0 0 150 0
0 0 0 0 0
```

---

## Performance Expectations

| Task | Time (seconds) |
|------|----------------|
| Compile generator | < 0.1 |
| Generate 4 test cases | < 0.1 |
| Compile detector | < 0.1 |
| Run detection on one test | < 0.05 |
| Run all 4 tests | < 0.2 |
| **Total end-to-end** | **< 0.5** |

---

## Output Interpretation Guide

### Fill Ratio (0.0 - 1.0)
- **0.95 - 1.0**: Nearly perfect rectangle (dense fill)
- **0.65 - 0.90**: Likely a circle (π/4 ≈ 0.785)
- **< 0.65 or > 0.90**: Irregular/other shapes

### Component Type
- **c** (Circular): Round objects with 65-90% fill ratio and square bounding box
- **r** (Rectangular): Dense objects with ≥95% fill ratio
- **o** (Other): Irregular shapes not matching above criteria

### Visual Markers
- Black/space areas: Background (value 0)
- Gray ASCII chars: Object pixels (values 1-254)
- **`$` characters**: Detection boundaries drawn by program (value 255)

---

## Success Criteria

You have successfully run the project when:

✓ All 4 input files generated without errors  
✓ Detection program compiles without errors  
✓ Program processes each test case and produces output  
✓ Component analysis shows detected objects with proper metrics  
✓ Visual representation shows ASCII art with bounding boxes (`$` markers)  
✓ Shape types (c/r/o) appear for detected components  
✓ No segmentation faults or runtime errors  
✓ All memory properly freed (program terminates cleanly)  

---

## Next Steps

After successful runs:
- Analyze results to verify detection accuracy
- Adjust threshold `T` parameter for different sensitivity levels
- Create custom test cases with different shapes and intensities
- Modify parameters in matrixgenerator.cpp for different images
- Study code comments for algorithm details
- See README.md for comprehensive project documentation
