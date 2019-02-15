#include<iostream>
#include "shredder.h"

using namespace std;

#ifdef UTFLAG
int UTmain(const int n_width, const string str_infile, const string str_outfile)
#else
int main()
#endif
{
  try 
  {
#ifdef UTFLAG
    TextShredder text_shredder(n_width, str_infile, str_outfile);
#else
    TextShredder text_shredder(2, "full_text.ascii", "shredded_text.ascii");
#endif
    text_shredder.GetInput();
    text_shredder.DoTextShredder();
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
