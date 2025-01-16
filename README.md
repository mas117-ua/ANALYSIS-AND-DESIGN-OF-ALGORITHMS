# 🎯 Algorithm Analysis and Design Project

## 📚 Overview
This project implements and analyzes various algorithms focusing on two main areas:
1. Minimum Cost Path (MCP) Finding Algorithms
2. Sorting Algorithm Analysis (QuickSort vs HeapSort)

## 🔍 Part 1: Minimum Cost Path Algorithms

### Implemented Approaches
1. **Branch and Bound (BB)** `mcp_bb.cc`
   - Uses priority queue for optimization
   - Implements pruning techniques
   - Dynamic programming for bound calculation

2. **Backtracking (BT)** `mcp_bt.cc`
   - Recursive implementation
   - Path tracking
   - Optimization using visit tracking

3. **Greedy Algorithm** `mcp_greedy.cc`
   - Forward and backward path calculation
   - Dynamic programming approach
   - Optimal substructure utilization

### 🎮 Features
- Supports 8-directional movement
- Path visualization in 2D
- Path encoding
- Performance metrics tracking:
  - Number of visited nodes
  - Number of explored paths
  - Number of leaf nodes
  - Number of unfeasible paths
  - Execution time

### 🔧 Usage
```
# Branch and Bound
./mcp_bb [-p] [--p2D] -f input_file

# Backtracking
./mcp_bt [-p] [--p2D] -f input_file

# Greedy
./mcp_greedy [--p2D] -f input_file
```

Parameters:
- `-p`: Show encoded path
- `--p2D`: Show 2D visualization
- `-f`: Input file path

## 🔄 Part 2: Sorting Algorithm Analysis

### Implemented Algorithms
1. **Middle QuickSort**
   - Pivot selection: Middle element
   - In-place implementation
   - Recursive partitioning

2. **HeapSort**
   - Max-heap implementation
   - In-place sorting
   - Sink operation optimization

### 📊 Analysis Scenarios
Tests performed on three types of arrays:
1. Random arrays
2. Sorted arrays
3. Reverse sorted arrays

Key metrics:
- Program steps (in millions)
- Array sizes from 2^15 to 2^20
- 30 samples per configuration

### 📈 Performance Analysis
```
#QUICKSORT VERSUS HEAPSORT
#Average processing Msteps (millions of program steps)
#Number of samples: 30

Size       Random Arrays    Sorted Arrays    Reverse Sorted
-----------------------------------------------------
2^15       QuickSort vs    QuickSort vs     QuickSort vs
           HeapSort        HeapSort         HeapSort
2^16       ...            ...              ...
...
2^20
```

## 🛠️ Implementation Details

### Data Structures
- Priority Queues (`<queue>`)
- Dynamic Arrays (`<vector>`)
- Custom Node structures for path tracking

### Memory Management
- Dynamic allocation for arrays
- Proper cleanup in destructors
- Memory leak prevention

### Optimization Techniques
- Bound calculations
- Path pruning
- Early termination
- Efficient data structures

## 🎯 Results and Analysis

### MCP Algorithms Comparison
- **Branch and Bound**: Best for finding optimal solutions with pruning
- **Backtracking**: Good for smaller grids and complete path exploration
- **Greedy**: Fastest but may not find optimal solution

### Sorting Algorithms Insights
- **QuickSort**: 
  - Better for random arrays
  - Poor performance on sorted/reverse sorted arrays
  - O(n log n) average case
- **HeapSort**:
  - Consistent performance across all cases
  - Guaranteed O(n log n) worst case
  - More stable performance

## 📦 Build Instructions
```
# Compile all programs
g++ -o mcp_bb mcp_bb.cc
g++ -o mcp_bt mcp_bt.cc
g++ -o mcp_greedy mcp_greedy.cc
g++ -o qs-vs-hs qs-vs-hs.cc

# Run tests
./mcp_bb -f test_map.txt
./mcp_bt -f test_map.txt
./mcp_greedy -f test_map.txt
./qs-vs-hs
```

## 🔬 Methodology
- Systematic testing approach
- Statistical analysis with 30 samples
- Multiple input sizes and configurations
- Performance metrics tracking
- Comparative analysis

