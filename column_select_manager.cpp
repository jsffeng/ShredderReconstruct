//////////////////////////////////////////////////////////////////////////////////////////  
//
//  Naming conventions used in source files and header files. 
//
//  Let's assume the following text content to be unshredded by this programme.
//  
//  a1|a2|a3
//  b1|b2|b3
//  c1|c2|c3
//  d1|d2|d3
// 
//  In above example,, a1, a2, b1, etc. is a string with same width, "|" is the delimiter
//  used by programme.
//
//  In the programme, above data will be stored into following 2 dimensional vector:
//  {
//    {a1,b1,c1,d1},
//    {a2,b2,c2,d2},
//    {a3,b3,c3,d3}
//  }
//  
//  A text strip refers to {a1,b1,c1,d1}, or {a2,b2,c2,d2}, etc. 
//  A "column" is often used to refer a text strip in data members or variables in the programme.
//  
//////////////////////////////////////////////////////////////////////////////////////////  

#include <stdexcept>
#include "common_classes.h"
#include "column_match_manager.h"
#include "column_select_manager.h"

using namespace std;

// This constant stand for the tolerance level for not matching rate.
// It would be helpful especially for those cases that some words are valid but unlikely to be found 
// in the dictionary, such as street name, etc.
const float kToleranceRate = 0.25;

// This constant stand for a minimum acceptable matching rate. If the matching rate for all the rest
// columns is below that value, means the programme cannot continue and run into pre-mature.
const float kAcceptableMatchRate = 1;

// Class ColumnSelectManager constructor
ColumnSelectManager::ColumnSelectManager()
{
  b_failure_flag_ = false;
  best_match_column_.n_number_in_pool = -1;
  best_match_column_.enum_best_match_direct = LEFT;
}

// Initialize vec_selected_columns_ and vec_column_pool_ with columns
void ColumnSelectManager::Init(const vector<vector<string>> & columns)
{
    if (!vec_selected_columns_.empty())
      vec_selected_columns_.clear();

    if (!vec_selected_columns_.empty())
      vec_column_pool_.clear();

    int n_column_size = columns.size();

    if (n_column_size < 2)
      throw runtime_error("Invalid input to function ColumnSelectManager::Init()");

    vec_column_pool_.assign(columns.begin(),columns.end());

#ifndef UTFLAG
    TextStripOperation::Disorder(vec_column_pool_);
#else
    // In UT, the vec_selected_columns_ in vec_column_pool_ are fixed,
    // which caused each thread generate the same result.
#endif
  
    vec_selected_columns_.emplace_back(*vec_column_pool_.begin());

    vec_column_pool_.erase(vec_column_pool_.begin());

}

// Add the best matching column to vec_selected_columns_
void ColumnSelectManager::AddToSelectedColumns()
{     
  if (best_match_column_.n_number_in_pool < 0 || best_match_column_.n_number_in_pool >= static_cast<int>(vec_column_pool_.size())) 
    throw runtime_error("Abnormal Error");

  if (best_match_column_.enum_best_match_direct == LEFT)
  {
    vec_selected_columns_.emplace(vec_selected_columns_.begin(), vec_column_pool_[best_match_column_.n_number_in_pool]);
  }
  else
  {
    vec_selected_columns_.emplace_back(vec_column_pool_[best_match_column_.n_number_in_pool]);
  }
}

// Remove the best matching column from vec_column_pool_,
// then re-set best_match_column_ with default value
void ColumnSelectManager::DeleteFromColumnPool()
{
  if (best_match_column_.n_number_in_pool == -1) 
    throw runtime_error("Function DeleteFromColumnPool() should not be called when best_match_column_.n_number_in_pool is -1");

  if (best_match_column_.n_number_in_pool < 0 || best_match_column_.n_number_in_pool >= vec_column_pool_.size()) 
    throw runtime_error("Abnormal Error");

  vec_column_pool_.erase(vec_column_pool_.begin() + best_match_column_.n_number_in_pool);

  // Reset the following is -1 after removing it from vec_column_pool_
  best_match_column_.n_number_in_pool = -1;
}

// Called only by RebuildColumnsByBestMatch
// Find best match in vec_column_pool_, if found, populate best_match_column_ , if fail, set
// b_failure_flag_ true
void ColumnSelectManager::FindBestMatch()
{
   if (vec_selected_columns_.empty() || vec_column_pool_.empty())
     throw runtime_error("Cannot run function FindBestMatch() when vec_selected_columns_ and/or vec_column_pool_ empty!");
    
   if (best_match_column_.n_number_in_pool != -1)
     throw runtime_error("Should not run function FindBestMatch() when best_match_column_.n_number_in_pool is not equal to -1");

   MatchRate match_rate_t;
   TwoWayDirections enum_direct_t;

   int n_number = -1;
   float f_best_match_rate = kAcceptableMatchRate;

   auto iter = vec_column_pool_.begin();

   while (iter != vec_column_pool_.end() )
   {   
       ColumnMatchManager * p_column_match_manager = new ColumnMatchManager(vec_selected_columns_, *iter); 
 
       p_column_match_manager->CalculateMatchRate();
       for(TwoWayDirections j = LEFT ;j < LIMIT; j = (TwoWayDirections)(j + 1))
       {
         match_rate_t = p_column_match_manager->column_match_rate_[j];
         if (match_rate_t.f_notmatch_rate > kToleranceRate)
         {
 	       continue;
         }
 	   else
         {
            if (match_rate_t.f_match_rate > f_best_match_rate)
            {
 	         f_best_match_rate = match_rate_t.f_match_rate;
 	         n_number = static_cast<int> (iter - vec_column_pool_.begin());
                 enum_direct_t = j;
            }
         }
       }

       delete p_column_match_manager;
       ++iter;
   }

   if (n_number == -1)
   {
      // f_best_match_rate should be equal to kAcceptableMatchRate at this time, otherwise, throw an error.
      if (f_best_match_rate != kAcceptableMatchRate)
        throw runtime_error("Abnormal error!");

      // Failed to find any matching columns, pre-matured!
      b_failure_flag_ = true;
   }
   else
   { 
     best_match_column_.n_number_in_pool = n_number; 

     // enum_direct_t must have been assigned a value, otherwise, throw an error.
     if (enum_direct_t != LEFT && enum_direct_t != RIGHT )
       throw runtime_error("Abnormal error!");

     best_match_column_.enum_best_match_direct = enum_direct_t;

     b_failure_flag_ = false;
   }
}

// Find the best matching and then upate vec_selected_columns_ and vec_column_pool_  
void ColumnSelectManager::RebuildColumnsByBestMatch()
{
  if (vec_selected_columns_.empty() || vec_column_pool_.empty())
    throw runtime_error("Cannot run function RebuildColumnsByBestMatch() when vec_selected_columns_ and/or vec_column_pool_ empty!");

  int n_column_pool_number_t;
  TwoWayDirections enum_direct_t;

  if (b_failure_flag_ != false || best_match_column_.n_number_in_pool != -1)
    throw runtime_error("Should not call RebuildColumnsByBestMatch() when b_failure_flag_ is not false or best_match_column_.n_number_in_pool is not -1 ");

  FindBestMatch();

  // If b_failure_flag_ is true, do nothing.
  if (b_failure_flag_ != true ) 
  {
    // Reconstruct vec_selected_columns_ with BestMatchColumn
    AddToSelectedColumns();
  
    // Reconstruct vec_column_pool_ with removing column in BestMatchColumn
    DeleteFromColumnPool();
  }
}

