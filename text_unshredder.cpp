#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "common_classes.h"
#include "text_unshredder.h"

using namespace std;

// Class TextUnshredder constructor
TextUnshredder::TextUnshredder(const string str_in_file, const string str_out_file) : str_in_filename_(str_in_file), str_out_filename_(str_out_file)
{
  b_premature_flag_ = false;
  n_premature_column_count_ = 0;

}

// Class TextUnshredder
// The text strip delimiter uses "|", so the text should not contain "|". In case that it contains,
// need to replace "|" with other special charactors, such as "*", etc before running this programme.

void TextUnshredder::GetInput()
{
  vector<string> vec_str_lines;
  vector<string> vec_str_temp;
  
  vector<vector<string>> vec_text_temp;
  
  TextFileOperation::ReadText(str_in_filename_, vec_str_lines);
  
  for (int i = 0; i < vec_str_lines.size(); ++i)
  {
    boost::split(vec_str_temp, vec_str_lines[i], boost::is_any_of("|"), boost::token_compress_on); 

    // There will be "" added in the begining and the end if vec_str_lines is like |ab|cd|, remove them if so
    if (vec_str_temp.begin()->empty())
       vec_str_temp.erase(vec_str_temp.begin()); 

    if ((vec_str_temp.end() - 1)->empty())
       vec_str_temp.erase(vec_str_temp.end() - 1); 

    vec_text_temp.emplace_back(vec_str_temp);
    vec_str_temp.clear();
  }
  
  // Verify input text as expected
  auto iter = vec_text_temp.begin();
  int n_size = (iter++)->size();
  while (iter != vec_text_temp.end())
  {
    if (iter->size() != n_size)
      throw runtime_error("Invalid input text strips!");
    
    auto iter_t = iter->begin();
    int n_str_size = (iter_t++)->size();
    while (iter_t != iter->end()) 
    {
      if (iter_t->size() != n_str_size)
        throw runtime_error("Invalid input text strips!");
  
      ++iter_t;
    }
       
    ++iter;
  }

  // Transpose vec_text_temp to vec_text_columns__
  TextStripOperation::Transpose(vec_text_temp, vec_text_columns_);

}

// Class TextUnshredder
void TextUnshredder::CreateOutput()
{
  for ( int i = 0; i < vec_merged_text_.size(); ++i)
  {
    boost::trim_right_if (vec_merged_text_[i], boost::is_any_of(" "));
  }

  TextFileOperation::WriteText(str_out_filename_, vec_merged_text_);
}

// Class TextUnshredder
void TextUnshredder::DoTextUnshredder()
{
  column_select_manager_.Init(vec_text_columns_);
  
  while (!column_select_manager_.vec_column_pool_.empty() && column_select_manager_.b_failure_flag_ == false)
  { 
    column_select_manager_.RebuildColumnsByBestMatch();
  }
  
  if (column_select_manager_.b_failure_flag_ == true) 
  {
    // Update b_premature_flag_ and n_premature_column_count_ 
    b_premature_flag_ = true;
    n_premature_column_count_ = column_select_manager_.vec_column_pool_.size();
  
    // Simply move column_select_manager_.vec_column_pool_ to 
    // column_select_manager_.vec_selected_columns_
    column_select_manager_.best_match_column_.enum_best_match_direct = RIGHT;
    
    while (!column_select_manager_.vec_column_pool_.empty())
    {
      column_select_manager_.best_match_column_.n_number_in_pool = 0; 
      column_select_manager_.AddToSelectedColumns();
      column_select_manager_.DeleteFromColumnPool();
    }
  }
  
  // Populate vec_merged_text_ with final result
  TextStripOperation::MergeText(column_select_manager_.vec_selected_columns_,vec_merged_text_);

}

