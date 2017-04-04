/*
 * the program is from the internet
 * it search the thing that partially match 
 */

// regex_search example  
#include <iostream>  
#include <regex>  
#include <string> 
#include <fstream>
#include <sstream>

using namespace std ;

string* search_by_keywords(string* key_array, string database_path, string musiclib_path, int &item_num) {

    // cout<<"debug search"<<endl;
   
    // how many keys are there
    int key_num=0;
    while (!(key_array[key_num].empty())){key_num++;}

    // get how many songs are there in the music database
    ifstream fd1 ;
    fd1.open( database_path );
    int song_num =0;
    string each_song;
    while(getline(fd1,each_song)){   song_num++;    }
    fd1.close();
    string* all_music = new string[song_num];


    // open the database again and do the matching one by one
    ifstream infile;
    infile.open( database_path );

    int index=0;
    string music_path;


    // cout<<"debug read and find"<<endl;

    //for each item, do a searching
    while(getline(infile,each_song)){

        // variables to use
        int key_i=0;
        int counter=0;
        string music_name;
        string song_copy=each_song;

        // do the key search for each key on the item
        for(key_i=0;key_i<key_num;key_i++){
                
                // renew the string as the original one
                each_song = song_copy;
                // define regular expression method
                smatch match_word;
                string key = "([^ ]*)"+key_array[key_i]+"([^ ]*)";
                regex pattern (key);
                
                // if the searching is correct, figure the file name
                while (regex_search (each_song,match_word,pattern)) { 
                        music_path="../music/";
                        counter++;
                        //for (auto x=match_word.begin();x!=match_word.end();x++){}
                        int i=0;
                        for(i=0 ;i<each_song.length(); i++){
                            if(each_song.at(i)==' '){break;}
                            music_path = music_path + each_song.at(i);
                        }
                        
                        music_path = music_path + ".txt";
                        each_song = match_word.suffix().str();
                }  
        }

        // if all the key is match, then store it, and record how many items are totally matched
        if(counter==key_num){
            all_music[index]=music_path;
            index++;
        }}


    // the array that returns
    string* return_list = new string[index];
    // cout<<"debug : the number of index is : "<<index<<endl;
    int copy =0;
    for(copy=0;copy<index;copy++){
        return_list[copy]= all_music[copy];
    }
    // cout<<"debug : the number of copy is : "<<copy<<endl;
    // cout<<"debug exit"<<endl;
    item_num = index;
    return return_list;
}

int main(){  
    //get the keyword one want to search
    cout << "" << endl;

    //open the music database
    string database = "../database/music_information.txt";
    string music_path="../music/";


    //process the keywords
    cout << "please enter what you want to search: " ;
    string key;
    getline(cin, key);

    int key_num=0;
    int key_i=0;
    istringstream iss(key);
 

    // count the key length
    while (iss) {
        string word;
        iss >> word;
        key_num++;
    }

    // assign the key to the array
    string* key_array = new string[key_num];
    istringstream iss_new(key);
    while (iss_new) {
        string word;
        iss_new >> word;
        key_array[key_i] = word;
        key_i++;
    }


    // useless checking code here
    {
        // // string* key_array = new string[2];
        // // key_array[0]="app";
        // // key_array[1]="big";

        // int i=0;
        // for(i=0; i<3; i++){
        //     //cout <<key_array[i]<<endl;
        // }
    }

    //get the music path
    int item_num=0;
    string* candidate_list = search_by_keywords(key_array, database, music_path, item_num);

    //while ((!candidate_list[item_num].empty())){item_num++;}

    //cout<<"debug : the number of item with this method is : "<< length(candidate_list)<<endl;

    // cout<<"debug : the number of item is : "<<item_num<<endl;
    
    int i=0;
    for(i=0; i<item_num; i++){
        cout<<"the music path is : "<<candidate_list[i]<<endl;
    }

    cout << "" << endl;
    return 0;
}   