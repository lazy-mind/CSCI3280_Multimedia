#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
using namespace std;
string Chop(string &str)
{
    string res = str;
    int len = str.length();
    if (str[len - 1] == 'r')
    {
        res.replace(len - 1, 1, '');
    }
    len = str.length();
    if (str[len - 1] == 'n')
    {
        res.replace(len - 1, 1, '');
    }
    return res;
}
void DumpEntry(_finddata_t &data)
{
    string createtime(ctime(&data.time_create));
    cout << Chop(createtime) << "t";
    cout << data.size << "t";
    if ((data.attrib & _A_SUBDIR) == _A_SUBDIR)
    {
        cout << "[" << data.name << "]" << endl;
    }
    else
    {
        cout << data.name << endl;
    }
}
int main()
{
    _finddata_t data;
    int ff = _findfirst ("../*.*", &data);
    if (ff != -1)
    {
        int res = 0;
        while (res != -1)
        {
            DumpEntry(data);
            res = _findnext(ff, &data);
        }
        _findclose(ff);
    }
    return 0;
}