#include<fstream>
#include<unordered_set>
#include "common_class.h"
#include "singleton_diction.h"

using namespace std;

SingletonDiction & SingletonDiction::GetInstance()  
{  
  static SingletonDiction instance; 
  return instance; 
}

void SingletonDiction::Init()
{
  TextFileOperation::ReadText("wordlist.txt", uset_dictionary_);
}

bool SingletonDiction::LookupDict(string str_letters)
{
  // If letters found in dictionary, return true
  if (uset_dictionary_.find(str_letters) != (uset_dictionary_.end()))
  {
    return true;
  }
  // Removing suffix such as ed|ing|s|es and search again
  vector<string> pattern = {"ing", "ed", "es", "s"};

  int n_position = str_letters.length(); 
  int n_position_t;
  int n_length_t; 
  int n_suffix_flag = 0;
  string str_letters_nosuffix;
  auto iter = pattern.begin();
  

  while (iter != pattern.end()) 
  {
    n_length_t = iter->length();
    n_position_t = n_position - n_length_t;
    // Only check if equal or greater than 2 letters after removing suffix
    if (n_position_t >= 2) 
    {
      if (str_letters.compare(n_position_t,n_length_t,*iter) == 0)
      { 
        str_letters_nosuffix = str_letters.substr(0,n_position_t); 
        ++n_suffix_flag;
        break;
      }    
    }
    ++iter;
  }

  if (n_suffix_flag != 0)
  {
    if (uset_dictionary_.find(str_letters) != (uset_dictionary_.end()))
      return true;
  } 
  
  // Return false if reaching here.  
  return false;
}

