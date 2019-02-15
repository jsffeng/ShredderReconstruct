#include<fstream>
#include<iostream>
#include "shredder.h"
#include "singleton.h"

using namespace std;

void FileOperation::ReadText(const string str_filename, vector<string> &vec_str_text_lines)
{
  string str_line;
  ifstream f_infile;

  f_infile.open(str_filename);
  if (!f_infile)
  {
     throw runtime_error("file cannot open to read!");
  }

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

  // Convert blank at end of each line to ascii space charactors  
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

void FileOperation::WriteText(const string str_filename, const vector<string> vec_str_text_lines)
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

void TextStripOperation::Transpose(vector<vector<string>>& vec_str_input, vector<vector<string>>& vec_str_input_trans)
{
  vector<string> vec_temp;
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

TextShredder::TextShredder(const int n_width, const string str_in_file, const string str_out_file) : n_strip_width_(n_width), str_in_filename_(str_in_file), str_out_filename_(str_out_file) 
{
}

void TextShredder::GetInput()
{
  FileOperation::ReadText(str_in_filename_, vec_str_source_data_);
}

void TextShredder::CreateOutput()
{
  vector<string> vec_temp;
  string str_sub;

  if (vec_str_shredded_text_.size() == 0)
  {
    throw runtime_error("Nothing to print to output file!");
  }

  for (int i = 0; i < vec_str_shredded_text_.size(); ++i)
  {
    for (int j = 0; j< vec_str_shredded_text_[i].size(); ++j)
    {
       str_sub.append("|" + vec_str_shredded_text_[i][j]); 
    } 
  
    str_sub.append("|"); 
    vec_temp.push_back(str_sub);
    str_sub.clear();
  }

  FileOperation::WriteText(str_out_filename_, vec_temp);
}

void TextShredder::DoTextShredder()
{
  vector<string> vec_temp;
  string str_sub;

  if (vec_str_source_data_.size() == 0)
  {
    throw runtime_error("vec_str_source_data_ empty, no data to shred!");
  }

  if (n_strip_width_ <= 0)
  {
    throw runtime_error("n_strip_width_ value is invalid!");
  }

  vec_str_shredded_text_.clear();
  vec_str_trans_shredded_text_.clear();

  auto iter = vec_str_source_data_.begin();

  while (iter != vec_str_source_data_.end())
  {
    int n_position = 0;
    while (n_position < iter->size())
    {
      str_sub = iter->substr(n_position, n_strip_width_); 
      // Ensure the last column in the paragraph has the same width of strip
      while (str_sub.length() < n_strip_width_) str_sub.append(" ");
      vec_temp.push_back(str_sub);
      n_position = n_position + n_strip_width_;
    }
    vec_str_shredded_text_.push_back(vec_temp);
    vec_temp.clear();

    ++iter;
  }
   
  // Transpose vec_str_shredded_text_ to vec_str_trans_shredded_text_
  TextStripOperation::Transpose(vec_str_shredded_text_, vec_str_trans_shredded_text_);

#ifndef UTFLAG
  TextStripOperation::Disorder(vec_str_trans_shredded_text_);
#endif 

  // Empty vec_str_shredded_text_ here
  vec_str_shredded_text_.clear();

  // Transpose vec_str_trans_shredded_text_ to vec_str_shredded_text_
  TextStripOperation::Transpose(vec_str_trans_shredded_text_, vec_str_shredded_text_);

}

