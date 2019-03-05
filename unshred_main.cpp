#include <iostream>
#include "singleton_diction.h"
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
#ifndef UTFLAG
    TextUnshredder text_unshredder("shredded_text.ascii", "restored_text.ascii");
#else
    TextUnshredder text_unshredder(str_infile, str_outfile);
#endif

    // Singleton class 
    SingletonDiction & singleton_dict = SingletonDiction::GetInstance();
    singleton_dict.Init();
    singleton_dict.BuildWordPiece();

    // Populate Populate vec_text_columns_ from input file
    text_unshredder.GetInput();

    // Shred text lines stored vec_str_source_data_ and save shredded result into vec_str_shredded_text_
    text_unshredder.DoTextUnshredder();

    // Print vec_merged_text_ to output file
    text_unshredder.CreateOutput();

    if (text_unshredder.b_premature_flag_ == true) 
    {
      cout <<"Partially restored! Number of "<< text_unshredder.n_premature_column_count_ << " text strips cannot be restored successfully and just append to the right side!" << endl;

      return 1;
    }
    else
    {
      cout <<"Successfully restored!" << endl;

      return 0;
    }
  
  }

  catch(std::exception & ref_exception)
  {
#ifndef UTFLAG
    cout << ref_exception.what() << endl;
    cout << "Error occured. Abort!" << endl; 
#endif
    return 2;
  }
}
