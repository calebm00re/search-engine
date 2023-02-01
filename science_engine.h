//
// Created by Caleb Moore on 4/15/21.
//

#ifndef SEARCH_ENGINE_SCIENCE_ENGINE_H
#define SEARCH_ENGINE_SCIENCE_ENGINE_H
#include "zoom_tree.h"
#include "simdjson.h"
#include "porter2_stemmer.h"
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include "DSHash.h"


using namespace std;
using namespace simdjson;
using namespace Porter2Stemmer;

class science_engine{
private:
    zoom_tree z;
    string important_path;
    set<string> stopWords;
    //set<string> authors_set;
    int fileNums = 0;
    int wordTotal = 0;
    set<string> allAuthor;
    DSHash<string, set<string>> authors;
    int temp_count;
    map<string, string> titles;
    map<string, string> publication;
    map<string, string> oneauth;
    unordered_map<string, int> word_ct;

public:
    void load_stop();
    void parse_files(const string&);
    void get_docs(const string&);
    inline void parse_str(string&, string&);
    void make_file();
    void do_searches();
    void get_info();
    void clear_avl();
    void make_author_file();
    void load_persist();
    void make_data_file();
    //void make_important_data();
};

#endif //SEARCH_ENGINE_SCIENCE_ENGINE_H
