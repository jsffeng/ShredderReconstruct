#ifndef __UNSHREDDER__
#define __UNSHREDDER__

#include <vector>
#include <string>
#include "column_select_manager.h"

// To construct text strips to a text page
class TextUnshredder
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    ColumnSelectManager column_select_manager_;

  public:
    // Store text data read from GetInput()
    // Use blank character ' ' to record a blank, in this way, all rows have the same number of charactors.
    // This will be convinient and safe , especially when handling it like a 2-dimentional array.
    std::vector<std::vector<std::string>> vec_text_columns_;

    // Store final result
    std::vector<std::string> vec_merged_text_;

    bool b_premature_flag_;

    // Size of unselected columns
    int n_premature_column_count_;

    // Set b_premature_flag_ = false, n_premature_column_count_ = 0
    TextUnshredder();

    // Populate vec_text_columns_ from input file, with calling text_strip::transpose
    // Use blank charactor ' ' to record blanks,i.e. never use empty string "" to record blanks 
    // in the vec_text_columns_.
    void GetInput(const std::string str_in_file); 

    void DoTextUnshredder();

    // Generate final output file
    void CreateOutput(const std::string str_out_file); 
};

#ifdef UTFLAG
  // To change main() to UTmain() so as to be tested by Unit Testing
  extern int UTmain(const std::string str_infile, const std::string str_outfile, bool b_thread_abnormal_flag);
#endif


#endif
