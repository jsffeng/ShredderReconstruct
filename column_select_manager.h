#ifndef __COLUMNSELECTMANAGER__
#define __COLUMNSELECTMANAGER__

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

    // Called only by RebuildColumnsByBestMatch 
    // Find best match in vec_column_pool_, if found, populate best_match_column_ , if fail, set
    // b_failure_flag_ true
    void FindBestMatch(); 

  public:
    std::vector<std::vector<std::string>> vec_selected_columns_;
    std::vector<std::vector<std::string>> vec_column_pool_;

    BestMatchColumn best_match_column_;

    // b_failure_flag_ default is false, true means no matching can be found in vec_column_pool_.
    bool b_failure_flag_;

    // Setup best_match_column_.n_number_in_pool=-1,best_match_column_.enum_best_match_direct=LEFT
    // Setup b_failure_flag_ = false
    ColumnSelectManager();

    void Init(const std::vector<std::vector<std::string>> & vec_text_columns); 

    // Find the best matching column and move it from vec_column_pool_ to vec_selected_columns_
    void RebuildColumnsByBestMatch(); 

    void AddToSelectedColumns();

    void DeleteFromColumnPool();

};

#endif
