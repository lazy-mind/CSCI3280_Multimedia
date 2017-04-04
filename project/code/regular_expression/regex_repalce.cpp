#include <regex>   
#include <iostream>   
   
int main() {   
    char buf[20];   
    const char *first = "axayaz";   
    const char *last = first + strlen(first);   
    std::regex rx("a");   
    std::string fmt("A");   
    std::regex_constants::match_flag_type fonly =   
        std::regex_constants::format_first_only;   
   
    *std::regex_replace(&buf[0], first, last, rx, fmt) = '\0';   
    std::cout << &buf[0] << std::endl;   
   
    *std::regex_replace(&buf[0], first, last, rx, fmt, fonly) = '\0';   
    std::cout << &buf[0] << std::endl;   
   
    std::string str("adaeaf");   
    std::cout << std::regex_replace(str, rx, fmt) << std::endl;   
   
    std::cout << std::regex_replace(str, rx, fmt, fonly) << std::endl;   
   
    return 0;   
}   