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

#endif
