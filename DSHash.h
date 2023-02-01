//
// Created by Caleb Moore on 4/27/21.
//

#ifndef SEARCH_ENGINE_DSHASH_H
#define SEARCH_ENGINE_DSHASH_H

#include <vector>
#include <iostream>
#include <set>

using namespace std;

template<typename K, typename V>
class DSHash{
private:
    int elements = 0;
    struct Entry{
        K key;
        V value;
        Entry(K k, V v) {
            key = k;
            value = v;
            //value.insert(v);
        }
        Entry(K k){
            key = k;
        }
    };

    vector<Entry>* table = nullptr;
    int num_elements = 0;
    int table_size = 0;
    V* not_there = nullptr;

public:
    DSHash();
    //void insert(const K& theKey, const V& theVal);
    V& get_val(const K& theKey);
    int& get_num();

};

template<typename K, typename V>
DSHash<K, V>::DSHash() {
    table_size = 100000;
    table = new vector<Entry>[table_size];
}

//template<typename K, typename V>
//void DSHash<K, V>::insert(const K &theKey, const V &theVal) {
//    Entry e(theKey, theVal);
//    auto hash_val = hash<K>{}(theKey);
//    int location = hash_val % table_size;
//    //cout << theKey << ": " << hash_val << endl;
//    table[location].push_back(e);
//}

template<typename K, typename V>
V &DSHash<K, V>::get_val(const K &theKey) {
    auto location = hash<K>{}(theKey) % table_size;
    for(auto& c : table[location]){
        if(c.key == theKey){
            return c.value;
        }
    }
    Entry e(theKey);
    elements++;
    table[location].push_back(e);
    for(auto& c : table[location]){
        if(c.key == theKey){
            return c.value;
        }
    }
    return *not_there;
}

template<typename K, typename V>
int &DSHash<K, V>::get_num() {
    return elements;
}

#endif //SEARCH_ENGINE_DSHASH_H
