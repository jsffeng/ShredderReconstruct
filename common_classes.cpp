#include<fstream>

#ifdef UTFLAG
#include<set>
#else
#include<unordered_set>
#endif

#include "common_classes.h"
#include "singleton_random.h"

using namespace std;

// Common function for reading text lines from an input file, usually used read text page
void TextFileOperation::ReadText(const string str_filename, vector<string> &vec_str_text_lines)
{
  string str_line;
  ifstream f_infile;

  f_infile.open(str_filename);
  if (!f_infile)
  {
     throw runtime_error("file cannot open to read!");
  }

  if (!vec_str_text_lines.empty())
    vec_str_text_lines.clear();

  unsigned int n_length;
  unsigned int n_max_length = 0;
  while (getline(f_infile, str_line))
  {
      n_length = str_line.length();
      if (n_length > n_max_length) n_max_length = n_length;
      vec_str_text_lines.push_back(str_line);
  }

  f_infile.close();

  // Convert blank at end of each line to ascii space charactors, this is required by shredding process
  auto iter = vec_str_text_lines.begin();
  unsigned int n_size;
  while (iter != vec_str_text_lines.end()) 
  {
    n_size = iter->size();
    while (n_size < n_max_length)
    { 
      iter->append(" ");
      ++n_size;
    }
    ++iter;
  }

}

// Common function for reading text lines from an input file, usually used to store dictionary
#ifdef UTFLAG
void TextFileOperation::ReadText(const string str_filename, set<string> &uset_str_text_lines)
#else
void TextFileOperation::ReadText(const string str_filename, unordered_set<string> &uset_str_text_lines)
#endif
{
  string str_line;
  ifstream f_infile;

  f_infile.open(str_filename);
  if (!f_infile)
  {
    throw runtime_error("file cannot open to read!");
  }

  if (!uset_str_text_lines.empty())
    uset_str_text_lines.clear();

  while (getline(f_infile, str_line))
  {
    uset_str_text_lines.insert(str_line);
  }

  f_infile.close();
}

// Common function for writing text strips to an output file
void TextFileOperation::WriteText(const string str_filename, const vector<string> vec_str_text_lines)
{
  ofstream f_outfile;

  f_outfile.open (str_filename);

  if (!f_outfile)
  {
     throw runtime_error("file cannot open to write!");
  }

  for (int i = 0; i < vec_str_text_lines.size(); ++i)
  {
      f_outfile << vec_str_text_lines[i] << endl;

  }

  f_outfile.close();
}

const int TextStripOperation::s_random_number_ = 40;

// Re-order the vector compoent - each componet contain a vector to store a text strip
void TextStripOperation::Disorder(vector<vector<string>>& vec_str_input) 
{
  unsigned int n_temp = 0; 

  if (vec_str_input.size() == 0)
  {
    throw runtime_error("Invalid input!");
  }
  else
  {
    SingletonRandom::s_max_val_ = vec_str_input.size() - 1;
    SingletonRandom& random_instance = SingletonRandom::GetInstance();

    for (int i = 0; i < s_random_number_; ++i)
    {
       n_temp = random_instance.GenerateRandom();
       vec_str_input.insert(vec_str_input.end(), vec_str_input[n_temp]);	
       vec_str_input.erase(vec_str_input.begin() + n_temp);
    }
  }
}

// Transpose row and column(each column is for a text strip)
void TextStripOperation::Transpose(vector<vector<string>>& vec_str_input, vector<vector<string>>& vec_str_input_trans)
{
  vector<string> vec_temp;

  if (!vec_str_input_trans.empty())
    vec_str_input_trans.clear();

  if (vec_str_input.size() == 0)
  { 
     throw runtime_error("Invalid input!");
  } 

  for (int i = 0; i < vec_str_input[0].size(); ++i)
  {
    for (int j = 0; j< vec_str_input.size(); ++j)
    {
      vec_temp.push_back(vec_str_input[j][i]);
    }

    vec_str_input_trans.push_back(vec_temp);
    vec_temp.clear();
  }

}

// Merge multiple columns into one
void TextStripOperation::MergeText(vector<vector<string>> & vec_str_input, vector<string>& vec_str_text)
{
  string str_temp;

  if (vec_str_input.empty()) 
  {
    throw runtime_error("vec_str_input is empty, no text data to merge!"); 
  }

  if (!vec_str_text.empty())
    vec_str_text.clear();
 
  for (int j = 0; j < vec_str_input.begin()->size() ; ++j)
  {
    for (int i = 0; i < vec_str_input.size(); ++i)
    {
      str_temp.append(vec_str_input[i][j]);
    }

    vec_str_text.push_back(str_temp);
    str_temp.clear();
  }
}

// This function will use charactor ' ' as word delimiter, example:
// a1a2a3|a4a5  |a7a8a9
//     a3|a4a5a6|a7  a9
//       |a4a5a6|a7a8a9
//       |  a5a6|a7a8
// a1  a3|a4  a6|a7a8a9
// a1  a3|  a5a6|a7a8
void StringWordOperation::FindLookupWordLeft(string & str_line, string & str_key, int n_column_width)
{
  string str_key_t;

  if (str_line.size() == 0 || n_column_width <= 0)
    throw runtime_error("Invalid input to function FindLookupWordLeft()!");

  int n_boundary = n_column_width; 
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
      if (k > n_boundary) 
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

// This function will use charactor ' ' as word delimiter, example:
// a1a2a3|  a5a6|a7a8a9
//   a2a3|a4a5a6|a7  
//   a2a3|a4a5a6|
// a1  a3|a4a5  |
// a1a2a3|a4  a6|a7  a9
//   a2a3|a4a5  |a7  a9
void StringWordOperation::FindLookupWordRight(string & str_line, string & str_key, int n_column_width)
{
  string str_key_t;

  if (str_line.size() == 0 || n_column_width <= 0)
    throw runtime_error("Invalid input to function FindLookupWordRight()!");

  int n_boundary = str_line.size() - n_column_width - 1; 

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
      if (k < n_boundary)
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

// Removing suffix such as ed|ing|s|es, won't remove suffix if the number of remaining letters are 
// less than 2. This function is usually for dictionary lookup.
bool StringWordOperation::RemoveWordSuffix(string &str_lookup_key)
{
  vector<string> vec_suffix = {"ing", "ed", "es", "s"};

  if (str_lookup_key.size() == 0)
    throw runtime_error("Invalid input to function RemoveWordSuffix()!");
  
  int n_position = str_lookup_key.size(); 
  int n_position_t, n_length_t;
  bool b_remove_suffix = false;

  auto iter = vec_suffix.begin();
  
  while (iter != vec_suffix.end()) 
  {
    n_length_t = iter->size();
    n_position_t = n_position - n_length_t;

    // Need to ensure the number of remaining letters are equal or greater than 2 after removing suffix
    if (n_position_t >= 2) 
    {
      if (str_lookup_key.compare(n_position_t,n_length_t,*iter) == 0)
      { 
        str_lookup_key = str_lookup_key.substr(0, n_position_t); 
        b_remove_suffix = true;
        break;
      }
    }
    ++iter;
  }
  return b_remove_suffix;
}
