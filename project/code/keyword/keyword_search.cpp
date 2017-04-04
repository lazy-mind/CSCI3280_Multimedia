#include <iostream>
#include <fstream>
#include <string>
using namespace std ;
 
int main()
{
    ifstream infile ;
    //char filename[100];
    //char filepath[100];
    int number_of_file=0;
    string str ;
    string key ;
    int cond=1;
 
    cout << "input key word: " ;
    cin >> key ;
    
    // std::string filename = std::to_string(i);
    string filename = "../database/music_information.txt";
    cout << "***  there are files under the directory \"music\"  ***" <<endl ;
    infile.open( filename );
    if ( infile.good() )
    {
        while( infile >> str )
        {
            if (1==1){
                cout << "*** here is a new item ***" << endl;
            }
            if (str == "\n"){
                cout << "here is a new item" << endl;
            }
            if ( str==key )
            {
                //cout << filename <<endl ;
                string filepath = "../music/" + str + ".txt";
                cout << "   the file path is : "<< filepath <<endl ;
                number_of_file++;
                cond = 0 ;
            }
        }
        infile.close();
    }
    
    if(number_of_file>0){
                cout << "   totally number of file : "<< number_of_file <<endl ;}
    if(cond==1){
        cout << "no match!" <<endl ;}
    return -1;
}
