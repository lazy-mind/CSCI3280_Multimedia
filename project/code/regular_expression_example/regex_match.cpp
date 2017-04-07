/*
 * the program is from the internet
 * it search the thing that match exactly (not partial) 
 */

// regex_match example  
#include <iostream>  
#include <string>  
#include <regex>  
  
int main ()  
{  
  
  if (std::regex_match ("subject", std::regex("(sub)(.*)") ))  
    std::cout << "1. string literal matched\n";  
  
  std::string s ("subject");  
  std::regex e ("(subj)(.*)");  
  if (std::regex_match (s,e))  
    std::cout << "2. string object matched\n";  
  
  if ( std::regex_match ( s.begin(), s.end(), e ) )  
    std::cout << "3. range matched\n";  
  
  std::cmatch cm;    // same as std::match_results<const char*> cm;  
  std::regex_match ("subject",cm,e);  
  std::cout << "4. string literal with " << cm.size() << " matches\n";  
  
  std::smatch sm;    // same as std::match_results<string::const_iterator> sm;  
  std::regex_match (s,sm,e);  
  std::cout << "5. string object with " << sm.size() << " matches\n";  
  
  std::regex_match ( s.cbegin(), s.cend(), sm, e);  
  std::cout << "6. range with " << sm.size() << " matches\n";  
  
  // using explicit flags:  
  std::regex_match ( "7. subject", cm, e, std::regex_constants::match_default );  
  
  std::cout << "8. the matches were: ";  
  for (unsigned i=0; i<sm.size(); ++i) {  
    std::cout << "[" << sm[i] << "] ";  
  }  
  
  std::cout << std::endl;  
  
  return 0;  
}  