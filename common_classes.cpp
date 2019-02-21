#include<fstream>
#include<unordered_set>
#include "common_classes.h"
#include "singleton_random.h"

using namespace std;

// Common function for reading text lines from an input file
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

void TextFileOperation::ReadText(const string str_filename, unordered_set<string> &uset_str_text_lines)
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
