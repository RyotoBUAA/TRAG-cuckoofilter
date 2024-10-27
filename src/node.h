#ifndef CUCKOO_FILTER_NODE_H_
#define CUCKOO_FILTER_NODE_H_

#include <string>
#include <map>
#include <set>
#include <vector>

namespace cuckoofilter{

    struct EntityNode{

        std::string content;

        operator uint64_t() const {
            uint64_t result = 0, b = 31, mod = 998244353;
            int n = content.size();
            for (int i=0;i<n;i++){
                result = result*b + content[i];
                if (result >= mod) result %= mod;
            }
            return result;
        }

    };

    struct EntityAddr{
        int * addr;
        EntityAddr * next;
    };

    struct EntityInfo{
        int temperature;
        EntityAddr * head;
    };

    class EntityNode {

        std::string entity;
        EntityNode * parent;
        vector<EntityNode*> children;

        EntityNode(std::string entity_name) : entity(entity_name) {}

        void add_children(EntityNode * node){
            node->parent = this;
            children.push_back(node);
        }

        vector<EntityNode*> get_children(){
            return children;
        }

        EntityNode * get_parent(){
            return parent;
        }

        std::string get_entity(){
            return entity;
        }

        std::string get_context(){
            vector<EntityNode*> ancestors = this->get_ancestors();
            int ancestor_length = ancestors.size();
            std::string context = "";
            if (ancestor_length > 0){
                context += "在某个树型关系中，"+this->entity+"的向上的层级关系有：";
                for (int i=0;i<ancestor_length;i++){
                    context += ancestors[i]->entity;
                    if (i<ancestor_length-1) context += "、";
                }
            }

            int children_length = children.size();
            if (children_length > 0){
                if (ancestor_length > 0) context += "；";
                context += this->entity+"的向下的子节点有：";
                for (int i=0;i<children_length;i++){
                    context += children[i]->entity;
                    if (i<children_length-1) context += "、";
                }
            }

            context += "。";
            return context;
        }

        vector<EntityNode*> get_ancestors(){
            vector<EntityNode*> ancestors;
            EntityNode * ancestor = this->parent;
            while (ancestor != NULL){
                ancestors.push_back(ancestor);
                ancestor = ancestor->parent;
            }
            return ancestors;
        }



    };


    class EntityTree {

        EntityNode * root;

        EntityTree(std::string root_entity, set< pair<string, string> > data){

        }

    };

}

#endif