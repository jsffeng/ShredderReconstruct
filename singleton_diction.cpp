#include <fstream>
#include "common_classes.h"
#include "singleton_diction.h"

using namespace std;

// Hungry Singleton
SingletonDiction & SingletonDiction::GetInstance()  
{  
  static SingletonDiction s_instance; 
  return s_instance; 
}

// Read dictionary from disk to memory
void SingletonDiction::Init()
{
  TextFileOperation::ReadText("dict/wordlist.txt", set_dictionary_);
}

// Build word piece with minimal charactor size 2, this will be used as the base for lookup.
// Example, word "nice" will be expanded by this function to word piece like ni,nic,nice, ic, ice, ce.
void SingletonDiction::BuildWordPiece()
{

  set<string> set_dictionary_temp;

  if (set_dictionary_.empty())
    throw runtime_error("set_dictionary_ is empty, function BuildWordPiece() quit!");

  auto iter_temp = set_dictionary_.begin();
  while (iter_temp != set_dictionary_.end())
  {
    set_dictionary_temp.insert(*iter_temp);
    ++iter_temp;
  }

  int n_size;
  string str_sub;

  auto iter = set_dictionary_temp.begin();
  while (iter != set_dictionary_temp.end())
  {
    n_size = (*iter).size();

    if ( n_size <= 2) 
    {
      ++iter;
      continue;
    }
    else
    {
      for (int n_pos = 0; n_pos < n_size - 1; ++n_pos) 
      {
         for (int n_len = 2; n_len <= n_size - n_pos; ++n_len)
         {
           str_sub = iter->substr(n_pos, n_len);
           set_dictionary_.insert(str_sub);
         }
      }
    }
    ++iter;
  }

}

// Lookup the str_letters to see if exists in dictionary
bool SingletonDiction::LookupDict(string str_letters)
{

  if (str_letters.empty())
    throw runtime_error("Invalid input to function LookupDict()!");

  // If letters found in dictionary, return true, otherwise, return false.
  if (set_dictionary_.find(str_letters) != (set_dictionary_.end()))
  {
    return true;
  }
  else
  {  
    return false;
  }
}
