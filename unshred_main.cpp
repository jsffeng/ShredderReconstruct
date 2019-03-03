#include<iostream>
#include "singleton_diction.h"
#include "text_unshredder.h"

using namespace std;

#ifdef UTFLAG
int UTmain(const string str_infile, const string str_outfile)
#else
// Main function
int main()
#endif
{
  try 
  {
#ifdef UTFLAG
    TextUnshredder text_unshredder(str_infile, str_outfile);
#else
    TextUnshredder text_unshredder("shredded_text.ascii", "restored_text.ascii");
#endif

    // Singleton class 
    SingletonDiction & singleton_dict = SingletonDiction::GetInstance();
    singleton_dict.Init();

    // Populate Populate vec_text_columns_ from input file
    text_unshredder.GetInput();

    // Shred text lines stored vec_str_source_data_ and save shredded result into vec_str_shredded_text_
    text_unshredder.DoTextUnshredder();

    // Print vec_merged_text_ to output file
    text_unshredder.CreateOutput();
  }

  catch(std::exception & ref_exception)
  {
#ifndef UTFLAG
    cout << ref_exception.what() << endl;
    cout << "Error occured. Abort!" << endl; 
#endif
    return 1;
  }

  return 0;
}
