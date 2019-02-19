#include "common_class.h"
#include "shredder.h"
#include "singleton.h"

using namespace std;

TextShredder::TextShredder(const int n_width, const string str_in_file, const string str_out_file) : n_strip_width_(n_width), str_in_filename_(str_in_file), str_out_filename_(str_out_file) 
{
}

// Populate vec_str_source_data_ from input file
void TextShredder::GetInput()
{
  TextFileOperation::ReadText(str_in_filename_, vec_str_source_data_);
}

// Print vec_str_shredded_text_ to output file by delimiter "|"
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

  TextFileOperation::WriteText(str_out_filename_, vec_temp);
}

// Shred text lines stored vec_str_source_data_ and save shredded result into vec_str_shredded_text_
void TextShredder::DoTextShredder()
{
  vector<string> vec_temp;
  string str_sub;

  if (vec_str_source_data_.size() == 0)
  {
    throw runtime_error("vec_str_source_data_ is empty, no data to shred!");
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

