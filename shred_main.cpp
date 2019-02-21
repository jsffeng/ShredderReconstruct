#include<iostream>
#include "text_shredder.h"

using namespace std;

#ifdef UTFLAG
int UTmain(const int n_width, const string str_infile, const string str_outfile)
#else
// Main function
int main()
#endif
{
  try 
  {
#ifdef UTFLAG
    TextShredder text_shredder(n_width, str_infile, str_outfile);
#else
    // Provide letter width in each shredded strip, input file contaning text for shredding, and
    // output file to store shredded result.
    TextShredder text_shredder(2, "full_text.ascii", "shredded_text.ascii");
#endif
    // Populate vec_str_source_data_ from input file
    text_shredder.GetInput();
    // Shred text lines stored vec_str_source_data_ and save shredded result into vec_str_shredded_text_
    text_shredder.DoTextShredder();
    // Print vec_str_shredded_text_ to output file by delimiter "|"
    text_shredder.CreateOutput();
  }

  catch(std::exception &ref_exception)
  {
#ifndef UTFLAG
    cout << ref_exception.what() << endl;
    cout << "Error occured. Abort!" << endl; 
#endif
    return 1;
  }

  return 0;
}
