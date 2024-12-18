# Graceful Tree Generator

## Overview
The **Graceful Tree Generator** is a C++ program designed to validate and generate a "Graceful Tree" based on a given tree structure. A Graceful Tree is a specialized odd-numbered tree with the following properties:

1. The tree's elements consist of the first N odd numbers (1, 3, 5, 7, ...).
2. The absolute differences between each node and its parent must be unique.

If the given tree structure can be gracefully completed, the program outputs a new file containing the tree labeled with odd numbers. Otherwise, it indicates that a graceful labeling is not possible.

---

## Features
- Reads a tree structure from an input file.
- Validates whether the tree can be labeled as a Graceful Tree.
- Outputs the labeled tree to a file if successful.
- Provides clear error messages for invalid tree structures.

---

## Input and Output Format

### Input File Format
The input file should describe the tree structure. Each line specifies a node and its children in the following format:

```
<parent_node> <child1> <child2> ... <childN>
```

- Each node is represented by a string.
- The tree must have exactly one root, and cycles are not allowed.

#### Example Input:
```
A B C
B D E
C F
```
This input describes the following tree structure:
```
    A
   / \
  B   C
 / \   \
D   E   F
```

### Output File Format
If the tree can be labeled as a Graceful Tree, the output file contains the same tree structure with nodes labeled by their assigned odd numbers. Each node is followed by its assigned value in parentheses.

#### Example Output:
```
A(1) B(3) C(5)
B(3) D(7) E(9)
C(5) F(11)
```

---

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/graceful-tree-generator.git
   cd graceful-tree-generator
   ```

2. Build the project using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. The executable `TreeGenerator` will be generated in the `build` directory.

---

## Usage

Run the program by providing the path to the input file as a command-line argument:

```bash
./TreeGenerator <input_file_path>
```

### Example:
```bash
./TreeGenerator example_input.txt
```

If the tree can be labeled gracefully, the program outputs a file named `graceful_tree_output.txt` in the current working directory. If labeling is not possible, the program displays a message:
```
It is not possible to create a Graceful Tree with the given structure.
```

---

## Error Handling
The program provides detailed error messages for the following scenarios:
- The input file cannot be opened.
- The tree has multiple roots.
- The tree contains cycles or is empty.
- Nodes have multiple parents.

---

## Development

### Code Structure
- **`main.cpp`**: Contains the main logic to validate and generate the Graceful Tree.
- **`CMakeLists.txt`**: Build configuration for the project.

### Key Functions
- `read_tree`: Reads the tree structure from the input file.
- `find_root`: Identifies the root node of the tree.
- `build_parent_map`: Constructs a map of parent-child relationships.
- `assign_values`: Uses backtracking to assign values to nodes while ensuring graceful properties.
- `write_tree`: Writes the labeled tree to an output file.

---

## License
This project is licensed under the MIT License. See the LICENSE file for details.

---

## Contributions
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

1. Fork the repository.
2. Create a feature branch.
3. Commit your changes.
4. Submit a pull request.

---

## Acknowledgments
This project was inspired by the challenge of creating specialized tree structures in C++. Thank you to everyone who provided feedback and support during development.

---

## Contact
For questions or feedback, please open an issue on GitHub or reach out at nikidimitrow@gmail.com.