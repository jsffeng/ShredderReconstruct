#ifndef __MAIN_THREAD__
#define __MAIN_THREAD__

#include <std::vector>
#include <std::string>
#include "data_type.h"

class MainThread
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    void UpdateThreadStatus();
    void ProcessThreadResult();
  public:

    // Store final result, default contain blank std::strings
    static std::vector<std::string> vec_final_merged_text_;

    // default b_premature_flag_ = false
    static bool b_premature_flag_;

    // Size of unselected columns, default n_premature_column_count_ = 0
    static int n_premature_column_count_;

    // default thread_status_ = NOTSTART
    static ThreadStatus thread_status_;
    
    static void  RunThreadUnshredder(); 
  
};

#endif
