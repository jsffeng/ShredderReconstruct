//////////////////////////////////////////////////////////////////////////////////////////  
//
//  Naming convention used in source code and header files in this programme.
//
//  Let's assume the following text content to be unshredded by this programme.
//  
//  a1|a2|a3
//  b1|b2|b3
//  c1|c2|c3
//  d1|d2|d3
// 
//  In abvoe, a1, a2, b1, etc. is a string with same width, "|" is delimiter used by programme.
//
//  In this programme, above data will be stored into following 2 dimentional vector:
//  {
//    {a1,b1,c1,d1},
//    {a2,b2,c2,d2},
//    {a3,b3,c3,d3}
//  }
//  
//  A text strip refer to {a1,b1,c1,d1}, or {a2,b2,c2,d2}, etc in above. 
//  A "column" often used to refer a text strip in data or variables in this programme.
//  
//////////////////////////////////////////////////////////////////////////////////////////  

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "common_classes.h"
#include "singleton_diction.h"
#include "column_match_manager.h"

using namespace std;

const float kToleranceRate = 0.15;

// Class ColumnMatchManager constructor
ColumnMatchManager::ColumnMatchManager(vector<vector<string>> & vec_text_columns,vector<string> & vec_new_column)
{
  vec_text_columns_.assign(vec_text_columns.begin(), vec_text_columns.end());
  vec_new_column_.assign(vec_new_column.begin(),vec_new_column.end());
}

// Class ColumnMatchManager
// Result stored to vec_key_column
void ColumnMatchManager::BuildLookupKey(vector<string> &vec_key_column, TwoWayDirections enum_direct) 
{
  vector<string> vec_merged_column;
  string str_merge_t;
  string str_key_temp;

  if (!vec_key_column.empty())
    vec_key_column.clear();

  if (vec_text_columns_.empty())
    throw runtime_error("vec_text_columns_ is empty, no data to match!");

  // Merged text stored in vec_merged_column
  TextStripOperation::MergeText(vec_text_columns_, vec_merged_column);

  // Delimiters for different words
  string s_to_replace = "[],-.:;!/\?\"\n\t\r)([]"; 
  string str_blank = " ";

  for (int i = 0; i < vec_new_column_.size(); ++i)
  {
    if (enum_direct == LEFT)
    {
      str_merge_t = vec_new_column_[i] + vec_merged_column[i]; 
    }
    else
    {
      str_merge_t = vec_merged_column[i] + vec_new_column_[i];
    }

    // Translate delimiters to " "
    str_merge_t = boost::replace_all_regex_copy(str_merge_t, boost::regex(s_to_replace), str_blank);

    // Extract the key word used for searching from a series of words delimited by ' '
    if (enum_direct == LEFT)
    {
       FindMergedWordLeft(str_merge_t, str_key_temp); 
    }
    else
    {
       FindMergedWordRight(str_merge_t, str_key_temp); 
    }

    // Remove 's, 'd  at the end of the word if there is

    int n_position = str_key_temp.size() - 2;
    string str_tmp = str_key_temp.substr(n_position, 2);
    
    if ("\'s" == str_tmp || "\'d" == str_tmp)
    {
      str_key_temp.erase(n_position,2);
    } 

    // If length of word is equal to or less than 2, ignore dictionary lookup
    if (str_key_temp.size() <= 2)
    {
      str_key_temp = "";
    }
    else
    {
      for (int i = 0; i < str_key_temp.size(); ++i)
      {
        // If any charactor is a number or non alphabetic such as '\'','\\', ignore dictionary lookup 
        // If any charactor is upper case letter, ignore dictionary lookup
        if (!isalpha(str_key_temp[i]) || isupper(str_key_temp[i]))
        {
          str_key_temp = "";
          break;
        }
      }
    }

    vec_key_column.push_back(str_key_temp);
    str_key_temp.clear();
  } 
}

// Class ColumnMatchManager
// Use charactor ' ' as word delimiter, example:
// a1a2a3|a4a5  |a7a8a9
//     a3|a4a5a6|a7  a9
//       |a4a5a6|a7a8a9
//       |  a5a6|a7a8
// a1  a3|a4  a6|a7a8a9
// a1  a3|  a5a6|a7a8
void ColumnMatchManager::FindMergedWordLeft(string & str_line, string & str_key)
{
  string str_key_t;

  int n_boundary = vec_new_column_.begin()->size(); 
  for (int k = 0; k < str_line.size(); ++k)
  {
    if (' ' != str_line[k]) 
    {
      if (str_key_t.empty() && k >= n_boundary)
      {
        break;
      }
      else
      {
        str_key_t = str_key_t + str_line[k]; 
      }
    }
    else
    {
      if (k >= n_boundary) 
      {
        break;
      }
      else
      {
        if (!str_key_t.empty())
          str_key_t.clear();
      }
    }
  }

  str_key = str_key_t;
}

// Class ColumnMatchManager
// Use charactor ' ' as word delimiter, example:
// a1a2a3|  a5a6|a7a8a9
//   a2a3|a4a5a6|a7  
//   a2a3|a4a5a6|
// a1  a3|a4a5  |
// a1a2a3|a4  a6|a7  a9
//   a2a3|a4a5  |a7  a9
void ColumnMatchManager::FindMergedWordRight(string & str_line, string & str_key)
{
  string str_key_t;

  int n_boundary = str_line.size() - vec_new_column_.begin()->size() - 1; 

  for (int k = str_line.size() - 1; k >= 0; --k)
  {
    if (' ' != str_line[k]) 
    {
      if (str_key_t.empty() && k <= n_boundary)
      {
        break;
      }
      else
      {
        str_key_t = str_line[k] + str_key_t; 
      }
    }
    else
    {
      if (k <= n_boundary)
      {
        break;
      }
      else
      {
        if (!str_key_t.empty())
          str_key_t.clear();
      }
    }
  }

  str_key = str_key_t;
}

// Class ColumnMatchManager
// If not found in dictionary, removing suffix such as ed|ing|s|es for re-searching
bool ColumnMatchManager::RemoveWordSuffix(string &str_lookup_key)
{
  vector<string> vec_suffix = {"ing", "ed", "es", "s"};

  int n_position = str_lookup_key.size(); 
  int n_position_t, n_length_t;
  bool b_remove_suffix = false;

  auto iter = vec_suffix.begin();
  
  while (iter != vec_suffix.end()) 
  {
    n_length_t = iter->size();
    n_position_t = n_position - n_length_t;

    // Need to ensure the remaining letters are equal or greater than 2 after removing suffix
    if (n_position_t >= 2) 
    {
      if (str_lookup_key.compare(n_position_t,n_length_t,*iter) == 0)
      { 
        str_lookup_key = str_lookup_key.substr(0, n_position_t - 1); 
        b_remove_suffix = true;
        break;
      }
    }
    ++iter;
  }

  return b_remove_suffix;

}

// class ColumnMatchManager
void ColumnMatchManager::CalculateMatchRate()
{
  vector<string> vec_word_column;

  int n_found = 0;
  int n_notfound = 0;
  bool b_flag_lookup, b_flag_suffix;

  SingletonDiction & singleton_dict = SingletonDiction::GetInstance();

  for(TwoWayDirections i = LEFT; i < LIMIT; i = (TwoWayDirections)(i+1))
  {
    BuildLookupKey(vec_word_column, i);
   
    for (int k = 0; k < vec_word_column.size(); ++k) 
    { 
      if (!vec_word_column[k].empty()) 
      {
        b_flag_lookup = singleton_dict.LookupDict(vec_word_column[k]);

        if (b_flag_lookup == true)
        {
          ++n_found;
        }
        else
        {
          // If not found in dictionary, removing suffix such as ed|ing|s|es if there is.
          b_flag_suffix = RemoveWordSuffix(vec_word_column[k]);
          if (b_flag_suffix == true)
          {
            b_flag_lookup = singleton_dict.LookupDict(vec_word_column[k]);
   
            if (b_flag_lookup == true)
              ++n_found;
            else
              ++n_notfound;
          }
          else
          {
            ++n_notfound;
          }
        }
      }
    }

    column_match_rate_[i].f_match_rate = (n_found + 1)/(n_notfound + 1);
    column_match_rate_[i].f_notmatch_rate = n_notfound/vec_word_column.size();

    vec_word_column.clear(); 
  }
}
