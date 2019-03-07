#ifndef __MAIN_THREAD__
#define __MAIN_THREAD__

#include <vector>
#include <mutex>
#include <string>
#include "data_type.h"
#include "text_unshredder.h"

class MainThread
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    // mutex for all threads
    static std::mutex status_mutex_;
    static std::mutex data_mutex_;

    // Default thread_status_ = NOTSTART
    static ThreadStatus thread_status_;
    
    static void UpdateThreadStatus(ThreadStatus status);
    static void RecordThreadResult(std::vector<std::string> vec_final_merged_text, int n_premature_column_count, bool b_premature_flag);
    static void RecordThreadAbnormals();

  public:
    // Increase the number any exception throw from a thread
    static int n_thread_abnormals_;

    // Store final result, default contain blank strings
    static std::vector<std::string> vec_final_merged_text_;

    // default b_premature_flag_ = false
    static bool b_premature_flag_;

    // Size of unselected columns, default n_premature_column_count_ = 0
    static int n_premature_column_count_;

    static int DoTextUnshredderInThread(TextUnshredder text_unshred); 
  
};

#endif
