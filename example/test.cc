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
using cuckoofilter::EntityStruct;

namespace cuckoofilter {
    std::map<std::string, EntityInfo*> addr_map;
    EntityInfo * temp_info;
    EntityInfo * result_info;
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

  std::cout << "char: " << sizeof(char) << std::endl;
  std::cout << "EntityInfo *: " << sizeof(EntityInfo*) << std::endl;

  FILE * in = fopen("entities_file.csv", "r");
  char input[1024];
  std::set<std::pair<std::string, std::string>> data;
  std::set<std::string> out_degree;
  std::set<std::string> root_list;
  std::set<std::string> entity_set;

  while (fgets(input, 1020, in) != NULL){
    std::vector<std::string> result = split(input, ',');
    if (result.size() == 2){
      data.insert({result[0], result[1]});
      out_degree.insert(result[0]);
      entity_set.insert(result[0]), entity_set.insert(result[1]);
    }
  }
  fclose(in);

  for (std::pair<std::string, std::string> edge : data){
    if (!out_degree.count(edge.second)) root_list.insert(edge.second);
  }

  std::cout << "total number of entities: " << entity_set.size() << std::endl; // 3148
  std::cout << "total number of roots: " << root_list.size() << std::endl; // 690

  std::vector<EntityTree*> forest;

  int success_num = 0;
  std::cout << "build tree..." << std::endl;
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

  std::cout << "build tree success, the length of forest: " << success_num << std::endl;

  // EntityInfo * info = cuckoofilter::addr_map["骨科"];
  // EntityAddr * addr = info->head;
  // int cnt = 0;
  // while (addr != NULL){
  //   cnt++;
  //   addr = addr->next;
  // }
  // std::cout << "骨科" << ": " << cnt << std::endl;

  CuckooFilter<EntityStruct, 16> filter(1000);
  size_t num_inserted = 0;
  for (size_t i = 0; i < 10; i++, num_inserted++) {
    EntityStruct temp = {"test"+std::to_string(i)};
    cuckoofilter::temp_info = cuckoofilter::addr_map["骨科"];
    if (filter.Add(temp) != cuckoofilter::Ok) {
      break;
    }
  }


  return 0;
}
