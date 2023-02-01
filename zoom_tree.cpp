//
// Created by Caleb Moore on 4/14/21.
//

#include "zoom_tree.h"

void zoom_tree::insert(const string &s, const string& id) {
    curr_id = id;
    insert(s, root);
}

void zoom_tree::empty(zoom_tree::tree_node *&n) {
    if(n != nullptr){
        empty(n->left);
        empty(n->right);
        delete n;
    }
}

//void zoom_tree::display() {
//    cout << "the following is the in order traversal" << endl;
//    cout << setw(15) << setfill('-') << '-' << endl;
//    displayInOrder(root);
//}

void zoom_tree::speed_query(const string &s) {
    tree_node* here(root);

    while(here != nullptr){
        if(s < here->data){
            here = here->left;
        }
        else if(here->data < s){
            here = here->right;
        }
        else{
            break;
        }
    }

    if(here != nullptr) {
        cout << s << " was found in:" << endl;
        for (string const &doc_element : here->ids) {
            cout << doc_element << endl;
        }
    }
    else{
        cout << "no words found" << endl;
    }
}

void zoom_tree::create_file() {
    ofstream output;
    output.open("reusable.txt");
    displayInOrder(root, output);
    output.close();
}

int& zoom_tree::display_stats() {
    //cout << "num of nodes: " << num_nodes << endl;
    return num_nodes;
}

set<string>* zoom_tree::search(const string & s) {
    tree_node* here(root);

    while(here != nullptr){
        if(s < here->data){
            here = here->left;
        }
        else if(here->data < s){
            here = here->right;
        }
        else{
            break;
        }
    }

    if (here != nullptr){
        return &here->ids;
    }
    return nullptr;
}

int &zoom_tree::get_occurs(const string & t, const string & w) {
    tree_node* here(root);

    while (here != nullptr){
        if(w < here->data){
            here = here->left;
        }
        else if(here->data < w){
            here = here->right;
        }
        else{
            return here->get_occur(t);
        }
    }
    return neg;
}

void zoom_tree::clear() {
    empty(root);
}

void zoom_tree::printTop() {
    getTops(root);

}

//void displayInOrder(tree_node*& t){
//    if(t != nullptr){
//        displayInOrder(t->left);
//        cout << t->data << endl;
//        displayInOrder(t->right);
//    }
//}

