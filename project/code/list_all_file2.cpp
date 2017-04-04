#include <iostream>
#include <dirent.h>

#ifdef WIN32
    bool islinux = false;
    
#else
    bool islinux = true;
#endif


int getdir (std::string dir, std::vector &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main(){
    std::string dir = std::string(".");
    std::vector<std::string> files = std::vector<std::string>();

    getdir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {
        std::cout << files[i] << std::endl;
    }
}
