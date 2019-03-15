#ifndef __COLUMNSELECTMANAGER__
#define __COLUMNSELECTMANAGER__

#include <vector>
#include <string>
#include "data_type.h"

class ColumnSelectManager
{
#ifdef UTFLAG
  public:
#else
  private:
#endif

    // Data type to record the best matching column
    typedef struct {
      int n_number_in_pool;
      TwoWayDirections enum_best_match_direct;
    } BestMatchColumn;

    // Called only by RebuildColumnsByBestMatch 
    // Find best match in vec_column_pool_, if found, populate best_match_column_ , if fail, set
    // b_failure_flag_ true
    void FindBestMatch(); 

  public:
    // Containing one or more columns which have been ordered
    std::vector<std::vector<std::string>> vec_selected_columns_;
    // Containing all rest columns to be ordered
    std::vector<std::vector<std::string>> vec_column_pool_;

    BestMatchColumn best_match_column_;

    // b_failure_flag_ default is false, true means no matching column can be found in vec_column_pool_.
    bool b_failure_flag_;

    // Set best_match_column_.n_number_in_pool = -1
    // Set best_match_column_.enum_best_match_direct = LEFT
    // Set b_failure_flag_ = false
    ColumnSelectManager();

    // Initialize vec_selected_columns_ and vec_column_pool_ with columns
    void Init(const std::vector<std::vector<std::string>> & vec_text_columns); 

    // Find the best matching and then upate vec_selected_columns_ and vec_column_pool_
    void RebuildColumnsByBestMatch(); 

    // Add the best matching column to vec_selected_columns_
    void AddToSelectedColumns();

    // Remove the best matching column from vec_column_pool_,
    // then re-set best_match_column_ with default value
    void DeleteFromColumnPool();

};

#endif
