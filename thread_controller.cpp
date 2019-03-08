#include <mutex>
#include "text_unshredder.h"
#include "thread_controller.h"

using namespace std;

// mutex for all threads
mutex ThreadController::status_mutex_;
mutex ThreadController::data_mutex_;

// Increase the number any exception throw from a thread
int ThreadController::n_thread_abnormals_ = 0;

// Store final result
vector<string> ThreadController::vec_final_merged_text_;

bool ThreadController::b_premature_flag_ = false;

// Size of unselected columns
int ThreadController::n_premature_column_count_ = 0;

ThreadStatus ThreadController::thread_status_ = NOTSTART; 

int ThreadController::DoTextUnshredderInThread(TextUnshredder text_unshred)
{
  try
  { 
    text_unshred.DoTextUnshredder();
  
    if (NOTSTART == thread_status_)
    {
      RecordThreadResult(text_unshred.vec_merged_text_, text_unshred.n_premature_column_count_, text_unshred.b_premature_flag_);
  
      if (b_premature_flag_ == false)
        UpdateThreadStatus(SUCCESS);
      else
        UpdateThreadStatus(PARTICIAL);
    }
    else if (PARTICIAL == thread_status_)
    {
      if ( text_unshred.b_premature_flag_ == false)
      {
        RecordThreadResult(text_unshred.vec_merged_text_, text_unshred.n_premature_column_count_, text_unshred.b_premature_flag_);
        UpdateThreadStatus(SUCCESS);  
      }
      else
      {
        if (text_unshred.n_premature_column_count_ < n_premature_column_count_ ) 
        {
          RecordThreadResult(text_unshred.vec_merged_text_, text_unshred.n_premature_column_count_, text_unshred.b_premature_flag_);
        }
      }
    }
  }
  
  catch(...)
  {
      RecordThreadAbnormals(); 
      return 1;
  }

  return 0;
}

void ThreadController::UpdateThreadStatus(ThreadStatus status)
{

  if (PARTICIAL == status || SUCCESS == status) 
  {
    lock_guard<mutex> lockGuard(status_mutex_);

    thread_status_ = status; 

  }
  else
  {
    throw runtime_error("Invalid input to function UpdateThreadStatus()!");
  }

}

void ThreadController::RecordThreadResult(vector<string> vec_final_merged_text, int n_premature_column_count, bool b_premature_flag)
{
  if (n_premature_column_count < 0)
    throw runtime_error("Invalid input to function RecordThreadResult()!");

  lock_guard<mutex> lockGuard(data_mutex_);

  vec_final_merged_text_.clear();
  vec_final_merged_text_.assign(vec_final_merged_text.begin(), vec_final_merged_text.end());
  b_premature_flag_ = b_premature_flag;
  n_premature_column_count_ = n_premature_column_count;
  
}

void ThreadController::RecordThreadAbnormals()
{
  lock_guard<mutex> lockGuard(status_mutex_);
  ++n_thread_abnormals_;
}
