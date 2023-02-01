//
// Created by Caleb Moore on 4/14/21.
//

#ifndef SEARCH_ENGINE_ZOOM_TREE_H
#define SEARCH_ENGINE_ZOOM_TREE_H
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <unordered_map>
#include <map>
#include "porter2_stemmer.h"

using namespace std;
using namespace Porter2Stemmer;

class zoom_tree{
private:
    struct tree_node{
        string data;
        int val{1};
        unordered_map<string, int> occurs;
        tree_node* left{nullptr};
        tree_node* right{nullptr};
        int height{0};
        set<string> ids;
        int negative = -1;
        tree_node() = default;
        tree_node(const tree_node& x){
            data = x.data;
            left = x.left;
            right = x.right;
            height = x.height;
            ids = x.ids;
        }
        tree_node& operator= (const tree_node& x)= default;
        /*  had this but it got mad and said it was trivial
            data = x.data;
            left = x.left;
            right = x.right;
            height = x.height;
            return *this;
         */
        tree_node(const string& s){
            data = s;
        }
        inline void add_doc(const string& s){
            ids.insert(s);
            if(occurs.find(s) != occurs.end()){
                occurs.find(s)->second++;
                //cout << "testing here: " << occurs.find(s)->second << endl;
            }
            else{
                occurs.emplace(s, 1);
            }
        }
        int& get_occur(const string& i){
            if(occurs.find(i) != occurs.end()){
                //cout << "returning in get_occur :" << occurs.find(i)->second << endl;
                return occurs.find(i)->second;
            }
            return negative;
        }
    };

    //useful variables
    tree_node* temp;
    tree_node* root;
    string curr_id;
    set<string> no_match;
    int num_nodes{0};
    int neg = -1;
    int rankSize{0};
    set<pair<int, string>> rankings;
    int rankMin{1000};
//    map<int, string> chart;
//    map<int, string>::iterator it;

    inline int height(tree_node*& t){
        if (t == nullptr){
            return -1;

        }
        else{
            return t->height;
        }
    }

    inline void balance(tree_node*& t){
        if(height(t->left) - height(t->right) > 1){
            if(height(t->left->left) >= height(t->left->right)){
                //rotateWithLeftChile(t); //case one
                temp = t->left;
                t->left = temp->right;
                temp->right = t;
                t->height = max(height(t->left), height(t->right)) + 1;
                t = temp;
            }
            else{
                //doubleWithLeftChild(t); //case two
                temp = t->left->right;
                t->left->right = temp->left;
                temp->left = t->left;
                t->left->height = max(height(t->left->right), height(t->left->left)) + 1;
                t->left = temp;
                //rotate second part
                temp = t->left;
                t->left = temp->right;
                temp->right = t;
                t->height = max(height(t->left), height(t->right)) + 1;
                t = temp;
            }
        }
        else if(height(t->right) - height(t->left) > 1){
            if(height(t->right->left) >= height(t->right->right)){
                //doubleWithRightChild(t); //case three
                temp = t->right->left;
                t->right->left = temp->right;
                temp->right = t->right;
                t->right->height = max(height(t->right->right), height(t->right->left)) + 1;
                t->right = temp;
                //second rotate
                temp = t->right;
                t->right = temp->left;
                temp->left = t;
                t->height = max(height(t->right), height(t->left)) + 1;
                t = temp;
            }
            else{
                //rotateWithRightChild(t); //case four
                temp = t->right;
                t->right = temp->left;
                temp->left = t;
                t->height = max(height(t->right), height(t->left)) + 1;
                t = temp;
            }
        }
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    inline void insert(const string& s, tree_node*& t){
        if(t == nullptr){
            t = new tree_node(s);
            t->add_doc(curr_id);
            num_nodes++;
            //set the new node up
        }
        else if(t->data < s){
            insert(s, t->right);
        }
        else if(s < t->data){
            insert(s, t->left);
        }
        else {
            t->add_doc(curr_id);
            t->val++;
        }
        balance(t);
    }

    void displayInOrder(tree_node*& t, ostream& o){
        if(t != nullptr){
            displayInOrder(t->left, o);
            o << t->data << " ";
            for(string const& s : t->ids){
                o << s << " ";
            }
            o << endl;
            //output << t->data << endl;
            displayInOrder(t->right, o);
        }
    }

    void getTops(tree_node*& t){
//        if(t != nullptr){
//            getTops(t->left);
//            if(rankSize < 50){
//                rankings.insert(make_pair(t->val, t->data));
//                rankSize++;
//            }
//            else if(rankSize == 50){
//                for(auto s : rankings){
//                    if(s.first < rankMin){
//                        rankMin = s.first;
//                    }
//                }
//
//            }
//            else{
//                if(t->val < rankMin){
//
//                }
//            }
//
//        }
    }

public:
    zoom_tree() : root(nullptr), temp(nullptr){no_match.insert("no match");}

    zoom_tree(const zoom_tree& rhs) : root(nullptr), temp(nullptr){
        *this = rhs;
    }

    ~zoom_tree(){
        empty(root);
    }

    void insert(const string& s, const string& id);
    void empty(tree_node*& n);
    //void display();
    void speed_query(const string&);
    void create_file();
    int& display_stats();
    set<string>* search(const string&);
    int& get_occurs(const string&, const string&);
    void clear();
    void printTop();
};

#endif //SEARCH_ENGINE_ZOOM_TREE_H
