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
//  In above example, a1, a2 or b1, etc. is a string with same width, "|" is the delimiter
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

#include <mutex>
#include "text_unshredder.h"
#include "thread_controller.h"

using namespace std;

// mutex for all threads
mutex ThreadController::status_mutex_;
mutex ThreadController::data_mutex_;

// The number will be increased if any exception throw from a thread
int ThreadController::n_thread_abnormals_ = 0;

// Store final result
vector<string> ThreadController::vec_final_merged_text_;

bool ThreadController::b_premature_flag_ = false;

// Size of unselected columns when premature happens
int ThreadController::n_premature_column_count_ = 0;

ThreadStatus ThreadController::thread_status_ = NOTSTART; 

// Entry function for threads
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

// Called by DoTextUnshredderInThread
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

// Called by DoTextUnshredderInThread
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

// Called by DoTextUnshredderInThread
void ThreadController::RecordThreadAbnormals()
{
  lock_guard<mutex> lockGuard(status_mutex_);
  ++n_thread_abnormals_;
}
