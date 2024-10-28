#include "cuckoofilter.h"

#include <assert.h>
#include <math.h>

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <map>
#include <utility>
#include <queue>
#include <set>

using cuckoofilter::CuckooFilter;
using cuckoofilter::EntityTree;
using cuckoofilter::EntityNode;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v"); 
    if (first == std::string::npos) return ""; 
    size_t last = str.find_last_not_of(" \t\n\r\f\v"); 
    return str.substr(first, (last - first + 1)); 
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(trim(token)); 
    }

    return tokens;
}

int main(int argc, char **argv) {
  // size_t total_items = 1000000;

  // // Create a cuckoo filter where each item is of type size_t and
  // // use 12 bits for each item:
  // //    CuckooFilter<size_t, 12> filter(total_items);
  // // To enable semi-sorting, define the storage of cuckoo filter to be
  // // PackedTable, accepting keys of size_t type and making 13 bits
  // // for each key:
  // //   CuckooFilter<size_t, 13, cuckoofilter::PackedTable> filter(total_items);
  // CuckooFilter<EntityNode, 32> filter(total_items);

  // // Insert items to this cuckoo filter
  // size_t num_inserted = 0;
  // for (size_t i = 0; i < total_items; i++, num_inserted++) {
  //   EntityNode temp = {"test"+std::to_string(i)};
  //   if (filter.Add(temp) != cuckoofilter::Ok) {
  //     break;
  //   }
  // }

  // // Check if previously inserted items are in the filter, expected
  // // true for all items
  // for (size_t i = 0; i < num_inserted; i++) {
  //   EntityNode temp = {"test"+std::to_string(i)};
  //   assert(filter.Contain(temp) == cuckoofilter::Ok);
  // }

  // // Check non-existing items, a few false positives expected
  // size_t total_queries = 0;
  // size_t false_queries = 0;
  // for (size_t i = total_items; i < 2 * total_items; i++) {
  //   EntityNode temp = {"test"+std::to_string(i)};
  //   if (filter.Contain(temp) == cuckoofilter::Ok) {
  //     false_queries++;
  //   }
  //   total_queries++;
  // }

  // // Output the measured false positive rate
  // std::cout << "false positive rate is "
  //           << 100.0 * false_queries / total_queries << "%\n";

  FILE * in = fopen("entities_file.csv", "r");
  char input[1024];
  std::set<std::pair<std::string, std::string>> data;
  std::set<std::string> out_degree;
  std::set<std::string> root_list;

  while (fgets(input, 1020, in) != NULL){
    std::vector<std::string> result = split(input, ',');
    if (result.size() == 2){
      data.insert({result[0], result[1]});
      out_degree.insert(result[0]);
    }
  }

  for (std::pair<std::string, std::string> edge : data){
    if (!out_degree.count(edge.second)) root_list.insert(edge.second);
  }

  std::cout << "total number of trees: " << root_list.size() << std::endl;

  std::vector<EntityTree*> forest;

  int success_num = 0;
  for (std::string root : root_list){
    EntityTree * new_tree = new EntityTree(root, data);
    // new_tree->print_tree();
    // EntityNode * n = new_tree->get_root();
    // for (EntityNode * temp : n->get_children()){
    //   std::cout << temp->get_context() << std::endl;
    // }
    forest.push_back(new_tree);
    success_num++;
    if (success_num > 50) break;
  }

  return 0;
}
