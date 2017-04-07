// regex_search example  
#include <iostream>  
#include <regex>  
#include <string>  
  
int main(){  
  std::string s ("this subject has a submarine as a subsequence");  
  std::smatch m;  
  std::regex e ("\\b(sub)([^ ]*)");   // matches words beginning by "sub"  
  
  std::cout << "Target sequence: " << s << std::endl;  
  std::cout << "Regular expression: /\\b(sub)([^ ]*)/" << std::endl;  
  std::cout << "The following matches and submatches were found:" << std::endl;  
  
  while (std::regex_search (s,m,e)) {  
    for (auto x=m.begin();x!=m.end();x++)   
      std::cout << x->str() << " ";  
    std::cout << "--> ([^ ]*) match " << m.format("$2") <<std::endl;  
    s = m.suffix().str();  
  }  
}  