#ifndef __COLUMNMATCHMANAGER__
#define __COLUMNMATCHMANAGER__

#include <vector>
#include <string>
#include "data_type.h"

class ColumnMatchManager
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    std::vector<std::vector<std::string>> vec_text_columns_;
    std::vector<std::string> vec_new_column_;

    // Called by CalculateMatchRate
    void BuildLookupKey(std::vector<std::string> &vec_key_column, TwoWayDirections enum_direct);
  public:

    // Record match rate by LEFT and RIGHT directions
    MatchRate column_match_rate_[LIMIT];

    ColumnMatchManager(std::vector<std::vector<std::string>> & vec_text_columns, std::vector<std::string> & vec_new_column);

    // Populate column_match_rate_[]
    void CalculateMatchRate();
};

#endif
