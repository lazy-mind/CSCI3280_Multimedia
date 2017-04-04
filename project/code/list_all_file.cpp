#include <iostream>
#include <curses.h>
#include <dirent.h>

int main()
{
	clrscr();
	int done, i=1;
	DIR *dir;
	dirent *pdir;
	cout<<"Press any key to list and view all the files in the current directory : \n";
	getch();
	dir=opendir(".");
	while(pdir=readdir(dir))
	{
		cout<<i<<"="pdir->d_name<<", ";
		i++;
	}
	closedir(dir);
	getch();
}