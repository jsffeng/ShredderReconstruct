#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <thread>
#include <iostream>
#include "singleton_diction.h"
#include "main_thread.h"
#include "common_classes.h"
#include "text_unshredder.h"

using namespace std;

#ifndef UTFLAG
// Main function
int main()
#else
  int UTmain(const string str_infile, const string str_outfile)
#endif
{

  try 
  {
    // Singleton class 
    SingletonDiction & singleton_dict = SingletonDiction::GetInstance();
    singleton_dict.Init();
    singleton_dict.BuildWordPiece();

    const int max_thread_number = 4;
    thread unshred_thread[max_thread_number]; 

    TextUnshredder text_unshredder;

    // Populate Populate vec_text_columns_ from input file
#ifndef UTFLAG
    text_unshredder.GetInput("shredded_text.ascii");
#else
    text_unshredder.GetInput(str_infile);
#endif

    for (int i = 0; i < max_thread_number; ++i)
    {
      unshred_thread[i] = thread(MainThread::DoTextUnshredderInThread,text_unshredder);
    }

    for (int i = 0; i < max_thread_number; ++i)
      unshred_thread[i].join(); 

    if (MainThread::n_thread_abnormals_ > 0)
    {
      string s_abormals_count = to_string(MainThread::n_thread_abnormals_);
      string s_threads_count = to_string(max_thread_number);
      string s_error_message = s_abormals_count + " out of " + s_threads_count + " threads threw abormal errors!";
      //throw runtime_error("Abnormal error occured in threads!");
      throw runtime_error(s_error_message);
    }
   
    if (MainThread::vec_final_merged_text_.empty())
    {
      throw runtime_error("Abnormal error occured!");
    }
    else
    {
      for ( int i = 0; i < MainThread::vec_final_merged_text_.size(); ++i)
      {
        boost::trim_right_if (MainThread::vec_final_merged_text_[i], boost::is_any_of(" "));
      }

#ifndef UTFLAG
      TextFileOperation::WriteText("restored_text.ascii", MainThread::vec_final_merged_text_);
#else
      TextFileOperation::WriteText(str_outfile, MainThread::vec_final_merged_text_);
#endif
    }

    if (MainThread::b_premature_flag_ == true) 
    {
      cout <<"Partially restored! Number of "<< MainThread::n_premature_column_count_ << " text strips cannot be restored successfully and just append to the right side!" << endl;

      return 1;
    }
    else
    {
      cout <<"Successfully restored!" << endl;

      return 0;
    }
  }

  catch(exception & ref_exception)
  {

#ifndef UTFLAG
    cout << ref_exception.what() << endl;
    cout << "Error occured. Abort!" << endl; 
#endif
    return 2;
  }
}
