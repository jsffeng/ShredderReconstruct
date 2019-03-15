#ifndef __MAIN_THREAD__
#define __MAIN_THREAD__

#include <vector>
#include <mutex>
#include <string>
#include "data_type.h"
#include "text_unshredder.h"

class ThreadController
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
   
    // Called by DoTextUnshredderInThread 
    static void UpdateThreadStatus(ThreadStatus status);

    // Called by DoTextUnshredderInThread 
    static void RecordThreadResult(std::vector<std::string> vec_final_merged_text, int n_premature_column_count, bool b_premature_flag);

    // Called by DoTextUnshredderInThread 
    static void RecordThreadAbnormals();

  public:
    // Default is 0, the number will be increased if any exception throw from a thread
    static int n_thread_abnormals_;

    // Store final result
    static std::vector<std::string> vec_final_merged_text_;

    // Default b_premature_flag_ = false
    static bool b_premature_flag_;

    // Size of unselected columns when premature happens, default n_premature_column_count_ = 0
    static int n_premature_column_count_;

    // Entry function for threads
    static int DoTextUnshredderInThread(TextUnshredder text_unshred); 
};

#endif
