#include <windows.h>  
#include <algorithm>  
#include <iostream>  
#include <iomanip>  
#include <fstream>  
#include <string>  
using namespace std;  
const int EXT_MAXSIZE=5; //扩展名的最大长度，包括"."  
const int EXT_ARRAY_SIZE=20; //扩展名数组的大小  
string EXT[EXT_ARRAY_SIZE]; //扩展名存放数组  
  
  
bool IsRoot(string Path)  
{  
    string Root;  
    Root=Path.at(0)+":\\";  
    if(Root==Path)  
        return true;  
    else  
        return false;  
}  
bool ext_ok(string ext_str)  
{  
    if(EXT[0]==".")  
        return true;  
    int i=0;  
    while(EXT[i]!=".")  
    {  
        if(strcmp(ext_str.c_str(),EXT[i].c_str())==0)   
            return true;  
        i++;  
    }  
    return false;  
}  
void FindInFile(string & filename,string & Find_str,fstream & outfile)  
{  
    cout<<"当前查找路径:"<<filename<<endl;  
    fstream infile(filename.c_str(),ios::in);  
    int length=Find_str.length();  
    if(!infile)  
    {  
        cout<<"打开文件"<<filename<<"失败"<<endl;  
    }  
    string temp;  
    int find_num=0;  
    char c;  
    while(temp.size()<Find_str.size())  
    {  
  
        if(infile.get(c))  
        {  
  
            temp+=toupper(c);  
  
        }  
        else  
        {  
            return ;  
        }  
    }  
    while(infile.get(c))  
    {  
          
          
        if(temp==Find_str)  
            find_num++;  
          
        temp=temp.substr(1,temp.size())+char(toupper(c));  
    }  
  
      
  
    if(find_num)  
    {  
        cout<<"查找到了："<<find_num<<endl;  
        outfile<<setw(3)<<find_num<<" \""<<Find_str<<"\" had been found in:"<<"\t"<<filename<<endl;  
        cout<<setw(3)<<find_num<<" \""<<Find_str<<"\" had been found."<<endl;  
    }  
      
    return ;  
}  
string get_ext(string filename)  
{  
    string ext;  
    int i=filename.size();  
    while(filename[i]!='.' && filename.size()-i<=EXT_MAXSIZE && i)  
    {  
        ext+=filename[i];  
        i--;  
    }  
    ext+='.';  
    reverse(ext.begin(),ext.end());  
    return ext;  
}  
void FindInAll(string & Path,string & Find_str,fstream & outfile)  
{  
    string szFind;  
    szFind=Path;  
    if(!IsRoot(szFind))  
        szFind+="\\";  
    szFind+="*.*";  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hFind=FindFirstFile(szFind.c_str(),& FindFileData);  
    if(hFind==INVALID_HANDLE_VALUE)  
        return ;  
    do  
    {  
        if(FindFileData.cFileName[0]=='.') //过滤本级目录和父目录  
            continue;  
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果找到的是目录，则进入此目录进行递归  
        {  
            string szFile;  
            if(IsRoot(Path))  
                szFile=Path+FindFileData.cFileName;  
            else  
                szFile=Path+"\\"+FindFileData.cFileName;  
            FindInAll(szFile,Find_str,outfile);  
        }  
        else //找到的是文件  
        {  
            string szFile;  
            if(IsRoot(Path))  
                szFile=Path+FindFileData.cFileName;  
            else  
                szFile=Path+"\\"+FindFileData.cFileName;  
            //cout<<szFile<<endl;  
            //cout<<FindFileData.cFileName<<endl;  
            //cout<<get_ext(FindFileData.cFileName)<<endl;  
            if(ext_ok(get_ext(szFile)))  
                FindInFile(szFile,Find_str,outfile);  
        }  
    }  
    while(FindNextFile(hFind,& FindFileData));  
    FindClose(hFind);  
}  
int main()  
{  
    char find_str[2000];  
  
    cout<<"输入要查找的目录:(如:D:\\C++)查找会包括其子目录."<<endl;  
    char ch_path[MAX_PATH];  
    cin.getline(ch_path,MAX_PATH,'\n');  
    string Path=ch_path;  
  
    if(Path=="") //路径出错  
        exit(0);  
  
    cout<<"输入要查找的字符串(请务必要大写,多个查询时请用,逗号分开,按回车结束,如AAA,BBB:):"<<endl;  
    cin.getline(find_str,2000,'\n');  
  
    string Find_str=find_str;  
  
  
    cout<<"输入查找文件名的扩展名:格式(.*),如果在所有文件中查找,直接输入\".\"!"<<endl;  
    int i=0;  
  
    while(cin>>EXT[i++],EXT[i-1]!=".");  
  
    fstream outfile("result.txt",ios::out);  
    int pos=0;  
    int npos=0;  
    string str;  
    locale::global(locale(""));  
    while((npos=Find_str.find_first_of(",",pos))!=string::npos)  
    {  
  
        str=Find_str.substr(pos,npos-pos);  
        outfile<<setw(3)<<"---查找字符串"<<str<<"开始:"<<endl;  
        cout<<"find_str:"<<str<<endl;  
        FindInAll(Path,str,outfile); //在指定目录及其子目录查找指定字符串  
        outfile<<setw(3)<<"---查找字符串:"<<str<<"结束!"<<endl;  
        pos=npos+1;  
    }  
  
    str=Find_str.substr(pos,Find_str.length());  
    cout<<"find_str:"<<str<<endl;  
    outfile<<setw(3)<<"---查找字符串"<<str<<"开始:"<<endl;  
    FindInAll(Path,str,outfile);  
    outfile<<setw(3)<<"---查找字符串:"<<str<<"结束!"<<endl;  
    locale::global(locale("C"));  
    outfile.close();  
    cout<<"查找完成,请查看该目录下的result.txt文件!"<<endl;  
    system("pause");  
    return 0;  
}  