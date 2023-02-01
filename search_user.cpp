//
// Created by Caleb Moore on 5/3/21.
//

#include "search_user.h"

void search_user::run_s_e() {
    string curr_state;
    //make selection menu
    start:
    cout << left << setw(45) << setfill(' ') <<  "What would you like to do:" << endl;
    cout << setw(45) << setfill('*') << "load from jsons " << " 0" << endl;
    cout << setw(45) << setfill('*') << "load from persistence file " << " 1" << endl;
    cout << setw(45) << setfill('*') << "clear data " << " 2" << endl;
    cout << setw(45) << setfill('*') << "Search " << " 3" << endl;
    cout << setw(45) << setfill('*') << "get stats " << " 4" << endl;
    cout << setw(45) << setfill('*') << "Shut down " << " 5" << endl << endl;

    //get first current task
    getline(cin, curr_state);
    if (curr_state == "0" || curr_state == "1" || curr_state == "2" || curr_state == "3" ||
        curr_state == "4" || curr_state == "5"){}
    else{
        goto start;
    }

    //make a science engine
    science_engine se;

    while(curr_state != "5"){
        int task = stoi(curr_state);

        if(task == 0){
            cout << "give direct path" << endl;
            string file_p;
            getline(cin, file_p);
            se.load_stop();
            se.parse_files(file_p);
            se.make_file();
        }
        else if(task == 1){
            se.load_persist();
        }
        else if(task == 2){
            se.clear_avl();
        }
        else if(task == 3){
            se.do_searches();
        }
        else{
            se.get_info();
        }

        try_again:
        cout << left << setw(45) << setfill(' ') <<  "What is next:" << endl;
        cout << setw(45) << setfill('*') << "load from jsons " << " 0" << endl;
        cout << setw(45) << setfill('*') << "load from persistence file " << " 1" << endl;
        cout << setw(45) << setfill('*') << "clear data " << " 2" << endl;
        cout << setw(45) << setfill('*') << "Search " << " 3" << endl;
        cout << setw(45) << setfill('*') << "get stats " << " 4" << endl;
        cout << setw(45) << setfill('*') << "Shut down " << " 5" << endl << endl;
        getline(cin, curr_state);
        if (curr_state == "0" || curr_state == "1" || curr_state == "2" || curr_state == "3" ||
            curr_state == "4" || curr_state == "5"){}
        else{
            goto try_again;
        }
    }
}
