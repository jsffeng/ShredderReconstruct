#include<fstream>
#include "common_classes.h"
#include "singleton_diction.h"

using namespace std;

SingletonDiction & SingletonDiction::GetInstance()  
{  
  static SingletonDiction s_instance; 
  return s_instance; 
}

void SingletonDiction::Init()
{
  TextFileOperation::ReadText("wordlist.txt", uset_dictionary_);
}

bool SingletonDiction::LookupDict(string str_letters)
{
  // If letters found in dictionary, return true, otherwise, return false.
  if (uset_dictionary_.find(str_letters) != (uset_dictionary_.end()))
  {
    return true;
  }
  else
  {  
    return false;
  }
}
