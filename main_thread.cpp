#include <mutex>
#include "text_unshredder.h"
#include "main_thread.h"

using namespace std;

// mutex for all threads
mutex MainThread::status_mutex_;
mutex MainThread::data_mutex_;

// Increase the number any exception throw from a thread
int MainThread::n_thread_abnormals_ = 0;

// Store final result
vector<string> MainThread::vec_final_merged_text_;

bool MainThread::b_premature_flag_ = false;

// Size of unselected columns
int MainThread::n_premature_column_count_ = 0;

ThreadStatus MainThread::thread_status_ = NOTSTART; 

int MainThread::DoTextUnshredderInThread(TextUnshredder text_unshred)
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

void MainThread::UpdateThreadStatus(ThreadStatus status)
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

void MainThread::RecordThreadResult(vector<string> vec_final_merged_text, int n_premature_column_count, bool b_premature_flag)
{

  lock_guard<mutex> lockGuard(data_mutex_);

  vec_final_merged_text_.clear();
  vec_final_merged_text_.assign(vec_final_merged_text.begin(), vec_final_merged_text.end());
  b_premature_flag_ = b_premature_flag;
  n_premature_column_count_ = n_premature_column_count;
  
}

void MainThread::RecordThreadAbnormals()
{
  lock_guard<mutex> lockGuard(status_mutex_);
  ++n_thread_abnormals_;
}
