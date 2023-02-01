//
// Created by Caleb Moore on 4/15/21.
//

#include "science_engine.h"

void science_engine::parse_files(const string & s) {

    important_path = s;
    dom::parser parser;
    string temp;
    string temp_name;
    string id;
    string title;
    bool firstAuth = true;

    for(const auto& file : filesystem::directory_iterator(s)){
        if(file.path().extension() == ".json") {
            temp_count = 0;
            fileNums++;
            auto thejson = parser.load(file.path());

            id = string(thejson["paper_id"]);

            title = string (thejson["metadata"]["title"]);
            titles.emplace(id, title);
            //cout << "TITLE: " << string(std::string_view(title)) << endl;
            //temp = (string(std::string_view(title)));
            parse_str(title, id);

            //dont need the authors for speed demo
            firstAuth = true;
            auto temp_authors = thejson["metadata"]["authors"];
            for (auto a : temp_authors) {
                //cout << std::string_view(a["last"]) << endl;
                temp_name = string(string_view(a["last"]));
                transform(temp_name.begin(), temp_name.end(), temp_name.begin(), [](unsigned char c) {return tolower(c);});
                allAuthor.insert(temp_name);
                if(firstAuth){
                    oneauth.emplace(id, temp_name);
                    firstAuth = false;
                }
                authors.get_val(temp_name).insert(id);
            }

            for (auto b : thejson["abstract"]) {
                //cout << std::string_view(b["text"]) << endl;
                temp = string(string_view(b["text"]));
                parse_str(temp, id);
            }
            for (auto c : thejson["body_text"]) {
                //cout << std::string_view(c["text"]) << endl;
                temp = string(string_view(c["text"]));
                parse_str(temp, id);
            }
            word_ct.emplace(id, temp_count);
        }
    }

}

void science_engine::parse_str(string & s, string& id) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {return tolower(c);});
    stringstream line(s);
    string temp;

    while(getline(line, temp, ' ')){
        //make sure the word is not a stop word
        if(ispunct(temp[temp.length() - 1])){
            temp.erase(temp.end());
        }
        if(stopWords.find(temp) == stopWords.end()){
            temp_count++;
            wordTotal++;
            //put stemming and inserting here
            stem(temp);
            z.insert(temp, id);
        }
    }
}

void science_engine::get_docs(const string & s) {
    z.speed_query(s);
}

void science_engine::load_stop() {
    ifstream input;
    string tempStop;

    input.open("stop.txt");

    if(!input.is_open()){
        cout << "help. have issue" << endl;
    }

    else{
        while(!input.eof()){
            getline(input, tempStop);
            stopWords.insert(tempStop);
        }
    }
    input.close();

//    string testing = "testing";
//    stem(testing);
//    cout << testing << endl;
    //cout << "total stop words :" << stopWords.size() << endl;
}

void science_engine::make_file() {
//    z.create_file();
//    make_author_file();
    make_data_file();
    //make_important_data();
}

void science_engine::get_info() {
    //display stats in zoom tree gets num nodes
    int avgWords = wordTotal / fileNums;
    cout << setw(45) << setfill(' ') << "Number of files: " << fileNums << endl;
    cout << setw(45) << setfill(' ') << "Average words/file: " << avgWords << endl;
    cout << setw(45) << setfill(' ') << "Total tree elements: " << z.display_stats() << endl;
    cout << setw(45) << setfill(' ') << "Total number of authors: " << authors.get_num() << endl;
    cout << "Top 50 words in data: " << endl;
    //z.printTop();
    cout << endl;
}

void science_engine::do_searches() {
    //get first input
    string search_line;
    cout << "make a searches or remain to main menu with \"end search\"" << endl;
    getline(cin, search_line);
    transform(search_line.begin(), search_line.end(), search_line.begin(), [](unsigned char c) {return tolower(c);});

    //used for parsing search and ranking
    string temp;
    string main_word;

    //set for finding docs
    set<string> adding;
    set<string> anding_words;
    set<string> orring_words;
    set<string> subtracting;
    set<string> authored;
    set<string> total;
    set<string> final;
    map<string, int> ranks;
    bool clearAll = false;
    /*
    set<pair<double, string>> rank;
    set<pair<double, string>> rankFinal;
     */


    //know what the last key query word was
    string key_query;

    //loop through inputs
    while(search_line != "end search"){
        stringstream line(search_line);

        while(getline(line, temp, ' ')) {

            //do the stuff for AND
            if (temp == "and") {
                while (getline(line, temp, ' ')) {
                    if (temp == "not") {
                        while (getline(line, temp, ' ')) {
                            stem(temp);
                            subtracting.insert(temp);
                            while(getline(line, temp, ' ')) {
                                if (temp == "author") {
                                    while(getline(line, temp, ' ')){
                                        authored = authors.get_val(temp);
                                        if(authored.empty()){
                                            clearAll = true;
                                        }
                                        goto done;
                                    }
                                }
                                else{
                                    goto done;
                                }
                            }
                        }
                    }
                    else if(temp == "author") {
                        while(getline(line, temp, ' ')) {
                            authored = authors.get_val(temp);
                            if(authored.empty()){
                                clearAll = true;
                            }
                            goto done;
                        }
                    }
                    else{
                        stem(temp);
//                        if(important) {
//                            main_word = temp;
//                            important = false;
//                        }
                        anding_words.insert(temp);
                    }
                }
            }


            //do the stuff for OR
            else if (temp == "or") {
                while (getline(line, temp, ' ')) {
                    if (temp == "not") {
                        while (getline(line, temp, ' ')) {
                            stem(temp);
                            subtracting.insert(temp);
                            while(getline(line, temp, ' ')) {
                                if (temp == "author") {
                                    while(getline(line, temp, ' ')){
                                        authored = authors.get_val(temp);
                                        if(authored.empty()){
                                            clearAll = true;
                                        }
                                        goto done;
                                    }
                                }
                                else{
                                    goto done;
                                }
                            }
                        }
                    }
                    else if(temp == "author") {
                        while(getline(line, temp, ' ')) {
                            authored = authors.get_val(temp);
                            if(authored.empty()){
                                clearAll = true;
                            }
                            goto done;
                        }
                    }
                    else{
                        stem(temp);
//                        if(important) {
//                            main_word = temp;
//                            important = false;
//                        }
                        orring_words.insert(temp);
                    }
                }
            }
            else {
                stem(temp);
//                if(important) {
//                    main_word = temp;
//                    important = false;
//                }
                orring_words.insert(temp);
                while (getline(line, temp, ' ')) {
                    if (temp == "not") {
                        while (getline(line, temp, ' ')) {
                            stem(temp);
                            subtracting.insert(temp);
                            while(getline(line, temp, ' ')) {
                                if (temp == "author") {
                                    while(getline(line, temp, ' ')){
                                        authored = authors.get_val(temp);
                                        if(authored.empty()){
                                            clearAll = true;
                                        }
                                        goto done;
                                    }
                                }
                                else{
                                    goto done;
                                }
                            }
                        }
                    }
                    else if(temp == "author") {
                        while(getline(line, temp, ' ')) {
                            authored = authors.get_val(temp);
                            if(authored.empty()){
                                clearAll = true;
                            }
                            goto done;
                        }
                    }
                    else{
                        goto done;
                    }
                }
            }
        }
        //end part for the displaying of docs
        done:

        int numsCurr;
        if(!anding_words.empty()){
            bool first = true;
            for(string const& s : anding_words){
                if(first){
                    first = false;
                    if(z.search(s) != nullptr) {
                        total = *z.search(s);
                    }
                    for(string const& k : total){
                        numsCurr = z.get_occurs(k, s);
                        if(numsCurr > 0){
                            ranks[k] = numsCurr;
                        }
                    }
                }
                else{
                    if(z.search(s) != nullptr) {
                        for (string const &t : *z.search(s)) {
                            if(total.find(t) != total.end()){
                                final.insert(t);
                                numsCurr = z.get_occurs(t, s);
                                if(numsCurr > 0) {
                                    ranks[t] += numsCurr;
                                }
                            }
                        }
                        total = final;
                        final.clear();
                    }
                }
            }
            if(!authored.empty()){
                for(string const& s : authored){
                    if(total.find(s) != total.end()){
                        final.insert(s);
                    }
                }
                total = final;
                final.clear();
            }
            if(!subtracting.empty()){
                for(string const& s : subtracting){
                    if(z.search(s) != nullptr) {
                        for (string const &t : *z.search(s)) {
                            if (total.find(t) != total.end()) {
                                total.erase(total.find(t));
                            }
                        }
                    }
                }
            }
        }
        else if(!orring_words.empty()){
            bool orStart = true;
            for(string const& s : orring_words){
                if(z.search(s) != nullptr){
                    for(string const& t : *z.search(s)){
                        total.insert(t);
                        if(orStart){
                            orStart = false;
                            numsCurr = z.get_occurs(t, s);
                            if(numsCurr > 0){
                                ranks[t] = numsCurr;
                            }
                        }
                        else{
                            numsCurr = z.get_occurs(t, s);
                            if(numsCurr > 0){
                                ranks[t] += numsCurr;
                            }
                        }
                    }
                }
            }
            if(!authored.empty()){
                for(string const& s : authored){
                    if(total.find(s) != total.end()){
                        final.insert(s);
                    }
                }
                total = final;
                final.clear();
            }
            if(!subtracting.empty()){
                for(string const& s : subtracting){
                    if(z.search(s) != nullptr) {
                        for (string const &t : *z.search(s)) {
                            if (total.find(t) != total.end()) {
                                total.erase(total.find(t));
                            }
                        }
                    }
                }
            }
        }

        cout << "your results are :" << endl;

        if(!clearAll) {

            map<double, string> finalRanks;
            for (string const &s : total) {
                double tempFreq = double(ranks[s]) / double(word_ct.find(s)->second);
                finalRanks[tempFreq] = s;
            }

            int maxDocs = 1;
            for (auto &e : finalRanks) {
                cout << maxDocs << ".)" << endl;
                cout << "Paper ID: " << e.second << endl;
                cout << "Paper Title: " << titles.at(e.second) << endl;
                maxDocs++;
                if (maxDocs == 16) {
                    break;
                }
            }
            cout << "closer look? (y/n)" << endl;
            string randomness;
            getline(cin, randomness);
            if(randomness == "y"){
                cout << "which one (valid number or ima seg fault right here, right now)" << endl;
                int tempNext;
                cin >> tempNext;
                cout << tempNext+1;
                int count = 1;
                string mainTemp;
                string new_path = important_path + "/";
                for(auto& e : finalRanks){
                    if(count == tempNext){
                        important_path += e.second;
                        break;
                    }
                    count++;
                }
                important_path += ".json";
                dom::parser parser;
                //cout << "first" << endl;
                auto thejson = parser.load(important_path);
                //cout << "SECOND" << endl;
                mainTemp = string(thejson["metadata"]["title"]);
                cout << "here is the title : " << mainTemp << endl;
            }
        }

        clearAll = false;
        adding.clear();
        anding_words.clear();
        orring_words.clear();
        subtracting.clear();
        authored.clear();
        total.clear();
        final.clear();
        ranks.clear();
        //rank.clear();

        //get next search line
        cout << "search again or 'end seach'" << endl;
        getline(cin, search_line);
        transform(search_line.begin(), search_line.end(), search_line.begin(), [](unsigned char c) {return tolower(c);});
    }
}

void science_engine::clear_avl() {
    z.clear();
}

void science_engine::make_author_file() {
    ofstream o;
    o.open("authors.txt");
    for(string const& s : allAuthor){
        o << s << " ";
        for(string const& t : authors.get_val(s)){
            o << t << " ";
        }
        o << endl;
    }
    o.close();
}

void science_engine::load_persist() {
    ifstream input;

    input.open("reusable.txt");
    if(!input.is_open()){
        cout << "help stuck" << endl;
    }
    else{
        string temp_line;
        string s;
        string w;
        while(!input.eof()){
            getline(input, temp_line);
            stringstream line(temp_line);
            while(getline(line, s, ' ')){
                w = s;
                while(getline(line, s, ' ')){
                    z.insert(w, s);
                }
            }
        }
    }
    input.close();


    input.open("authors.txt");
    if(!input.is_open()){
        cout << "help. am more stuck1" << endl;
    }
    else{
        string temp_line;
        string s;
        string a;
        while(!input.eof()){
            getline(input, temp_line);
            stringstream line(temp_line);
            while(getline(line, s, ' ')){
                a = s;
                while(getline(line, s, ' ')){
                    authors.get_val(a).insert(s);
                }
            }
        }
    }
    input.close();


    input.open("stats.txt");
    if(!input.is_open()){
        cout << "help. am more stuck" << endl;
    }
    else{
        string temp1;
        string temp2;

        getline(input, temp1);
        getline(input, temp2);

        wordTotal = stoi(temp1);
        fileNums = stoi(temp2);

    }
    input.close();
}


void science_engine::make_data_file() {
    ofstream o;
    o.open("stats.txt");
    string temp;
    o << wordTotal << endl;
    o << fileNums << endl;
//    o << (wordTotal / fileNums) << endl;
//    o << z.display_stats() << endl;
//    o << authors.get_num() << endl;
    o.close();
}
