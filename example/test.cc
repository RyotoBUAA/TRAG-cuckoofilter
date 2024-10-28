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
using cuckoofilter::EntityInfo;
using cuckoofilter::EntityAddr;

namespace cuckoofilter {
    std::map<std::string, EntityInfo*> addr_map;
}

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
    EntityNode * n = new_tree->get_root();
    for (EntityNode * temp : n->get_children()){
      // std::cout << temp->get_context() << std::endl;
    }
    forest.push_back(new_tree);
    success_num++;
    // std::cout << "tree: " << success_num << std::endl;
    // if (success_num > 50) break;
  }

  EntityInfo * info = cuckoofilter::addr_map["骨科"];
  EntityAddr * addr = info->head;
  int cnt = 0;
  while (addr != NULL){
    cnt++;
    addr = addr->next;
  }
  std::cout << "骨科" << ": " << cnt << std::endl;

  return 0;
}
