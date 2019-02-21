#ifndef __UNSHREDDER__
#define __UNSHREDDER__

#include<vector>
#include<string>
#include "data_type.h"

class ColumnSelectManager
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    typedef struct {
      int n_number_in_pool;
      TwoWayDirections enum_best_match_direct;
    } BestMatchColumn;

#ifdef UTFLAG
  public:
#else
  private:
#endif
    BestMatchColumn best_match_column_;
    std::vector<std::vector<std::string>> vec_selected_columns_;
    std::vector<std::vector<std::string>> vec_column_pool_;
    // Called only by RebuildColumnsByBestMatch 
    // Find best match in vec_column_pool_, if succeed, populate best_match_column_ , if fail, set
    // b_failure_flag_ yes
    void FindBestMatch(); 
    void DeleteFromColumnPool();

  public:
    // b_failure_flag_ default is no, yes means no matching can be found in vec_column_pool_.
    bool b_failure_flag_;
    // Setup best_match_column_.n_number_in_pool=-1,best_match_column_.enum_best_match_direct=0
    // Setup b_failure_flag_ = no 
    ColumnSelectManager();
    void Init(std::vector<std::vector<std::string>> & vec_text_columns); 
    // Find the best matching column and move it from vec_column_pool_ to vec_selected_columns_
    int RebuildColumnsByBestMatch(); 
    void AddToSelectedColumns();

};

// To construct text strips to a text page
class TextUnshredder
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    const std::string str_in_filename_;
    const std::string str_out_filename_;
    // Use blank character ' ' to record a blank, in this way, all rows have the same number of charactors.
    // This will be convinient and safe , especially when handling it like a 2-dimentional array.
    std::vector<std::vector<std::string>> vec_text_columns_;
    // Store final result
    std::vector<std::string> vec_merged_text_;
    ColumnSelectManager column_select_manager_;

  public:
    bool b_premature_flag_;
    // Size of unselected columns
    int n_premature_column_number_;
    // Set b_premature_flag_ = no, n_premature_column_number_ = 0
    TextUnshredder(const std::string str_in_file, const std::string str_out_file);
    // Populate vec_text_columns_ from str_in_filename_, with calling text_strip::transpose
    // Use blank charactor ' ' to record blanks,i.e. never use empty std::string "" to record blanks in the 
    // vec_text_columns_.
    void GetInput(); 
    void DoTextUnshredder();
    // Generate final output file with vec_merged_text_
    void CreateOutput(); 
};

#endif
