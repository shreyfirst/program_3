#include <iostream>
#include <fstream>
#include "json.hpp"
#include "priorityqueue.h"

using namespace std;

int main(int argc, char** argv) {
  // Check if filename is provided as argument
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  // Read input JSON file
  ifstream input_file(argv[1]);
  if (!input_file.is_open()) {
    cerr << "Error opening file " << argv[1] << endl;
    return 1;
  }
  nlohmann::json input_json;
  input_file >> input_json;

  // Get metadata from input JSON
  size_t max_size = input_json["metadata"]["maxHeapSize"];
  size_t num_ops = input_json["metadata"]["numOperations"];

  // Create priority queue object
  PriorityQueue queue(max_size);

  // Execute operations from input JSON
  for (auto it = input_json.begin(); it != input_json.end(); ++it) {
    // Skip metadata
    if (it.key() == "metadata") {
      continue;
    }

    // Execute operation
    string op = it.value()["operation"];
    if (op == "insert") {
      double vari = static_cast<double>(it.value()["key"]) + 0.0;
      // std::cout << vari << std::endl;
      Key key = vari;
      Value value = std::make_pair(0, 0);
      queue.insert(std::make_pair(key, value));
    } else if (op == "removeMin") {
      queue.removeMin();
    } else {
      cerr << "Error: Unknown operation " << op << endl;
      return 1;
    }
  }

  // Create output JSON object
  nlohmann::json output_json = queue.JSON();
  output_json["metadata"]["numOperations"] = num_ops;

  cout << output_json.dump(2) << endl;

  return 0;
}

