#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Function to read the tree structure from the input file
map<string, vector<string>> read_tree(const string& file_path) {
    ifstream infile(file_path);
    if (!infile.is_open()) {
        cerr << "Error: Cannot open the file " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    map<string, vector<string>> tree;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string parent;
        ss >> parent;
        string child;
        vector<string> children;
        while (ss >> child) {
            children.push_back(child);
        }
        tree[parent] = children;
    }

    infile.close();
    return tree;
}

// Function to find the root of the tree
string find_root(const map<string, vector<string>>& tree) {
    set<string> all_children;
    for (const auto& pair : tree) {
        for (const auto& child : pair.second) {
            all_children.insert(child);
        }
    }

    string root = "";
    for (const auto& pair : tree) {
        if (all_children.find(pair.first) == all_children.end()) {
            if (root.empty()) {
                root = pair.first;
            } else {
                cerr << "Error: Multiple roots detected." << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    if (root.empty()) {
        cerr << "Error: No root found. The tree might have cycles or be empty." << endl;
        exit(EXIT_FAILURE);
    }

    return root;
}

// Function to build a parent map
map<string, string> build_parent_map(const map<string, vector<string>>& tree, const string& root) {
    map<string, string> parent_map;
    parent_map[root] = ""; // Root has no parent

    vector<string> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        string current = stack.back();
        stack.pop_back();
        for (const auto& child : tree.at(current)) {
            if (parent_map.find(child) != parent_map.end()) {
                cerr << "Error: Multiple parents detected for node " << child << "." << endl;
                exit(EXIT_FAILURE);
            }
            parent_map[child] = current;
            stack.push_back(child);
        }
    }

    return parent_map;
}

// Backtracking function to assign values
bool assign_values(
    const map<string, vector<string>>& tree,
    const vector<string>& node_order,
    size_t index,
    map<string, int>& assigned,
    set<int>& used_differences,
    set<int>& used_values,
    const vector<int>& odd_numbers,
    const map<string, string>& parent_map
) {
    if (index == node_order.size()) {
        return true; // All nodes assigned
    }

    string current_node = node_order[index];
    // Iterate through all possible odd numbers
    for (const auto& number : odd_numbers) {
        if (used_values.find(number) != used_values.end()) {
            continue; // Number already used
        }

        // Check difference with parent
        bool valid = true;
        int difference = 0;
        if (parent_map.at(current_node) != "") { // Not root
            string parent = parent_map.at(current_node);
            if (assigned.find(parent) == assigned.end()) {
                continue; // Parent not yet assigned
            }
            difference = abs(number - assigned[parent]);
            if (used_differences.find(difference) != used_differences.end()) {
                valid = false; // Difference already used
            }
        }

        if (valid) {
            // Assign the number
            assigned[current_node] = number;
            used_values.insert(number);
            if (parent_map.at(current_node) != "") {
                used_differences.insert(difference);
            }

            // Recurse to the next node
            if (assign_values(tree, node_order, index + 1, assigned, used_differences, used_values, odd_numbers, parent_map)) {
                return true; // Successful assignment
            }

            // Backtrack
            assigned.erase(current_node);
            used_values.erase(number);
            if (parent_map.at(current_node) != "") {
                used_differences.erase(difference);
            }
        }
    }

    return false; // No valid assignment found at this branch
}

// Function to write the labeled tree to an output file
void write_tree(const string& file_path, const map<string, vector<string>>& tree, const map<string, int>& assignment) {
    ofstream outfile(file_path);
    if (!outfile.is_open()) {
        cerr << "Error: Cannot open the file " << file_path << " for writing." << endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& pair : tree) {
        outfile << pair.first;
        // Append the assigned value
        if (assignment.find(pair.first) != assignment.end()) {
            outfile << "(" << assignment.at(pair.first) << ")";
        }
        for (const auto& child : pair.second) {
            outfile << " " << child;
            // Append the assigned value
            if (assignment.find(child) != assignment.end()) {
                outfile << "(" << assignment.at(child) << ")";
            }
        }
        outfile << "\n";
    }

    outfile.close();
    cout << "Graceful tree has been written to " << file_path << endl;
}

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file_path>" << endl;
        return EXIT_FAILURE;
    }

    string input_file = argv[1];
    // Read the tree
    map<string, vector<string>> tree = read_tree(input_file);
    int N = tree.size();
    if (N == 0) {
        cerr << "Error: The tree is empty." << endl;
        return EXIT_FAILURE;
    }

    // Find the root
    string root = find_root(tree);

    // Build parent map
    map<string, string> parent_map = build_parent_map(tree, root);

    // Generate first N odd numbers
    vector<int> odd_numbers;
    for (int i = 0; i < N; ++i) {
        odd_numbers.push_back(2 * i + 1);
    }

    // Order the nodes using BFS for better efficiency
    vector<string> node_order;
    vector<string> queue;
    queue.push_back(root);
    while (!queue.empty()) {
        string current = queue.front();
        queue.erase(queue.begin());
        node_order.push_back(current);
        for (const auto& child : tree.at(current)) {
            queue.push_back(child);
        }
    }

    // Initialize variables for backtracking
    map<string, int> assigned;
    set<int> used_differences;
    set<int> used_values;

    // Start backtracking
    bool success = assign_values(tree, node_order, 0, assigned, used_differences, used_values, odd_numbers, parent_map);

    if (success) {
        // Prepare the output file path
        string output_file = "graceful_tree_output.txt";
        write_tree(output_file, tree, assigned);
    } else {
        cout << "It is not possible to create a Graceful Tree with the given structure." << endl;
    }

    return 0;
}
