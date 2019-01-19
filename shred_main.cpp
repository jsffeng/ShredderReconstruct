#include<iostream>
#include "shredder.h"

using namespace std;

#ifdef UTFLAG
int UTmain(const int width, const string infile, const string outfile)
#else
int main()
#endif
{
  try 
  {
#ifdef UTFLAG
    shredder text_sh(width,infile,outfile);
#else
    shredder text_sh(2,"full_text.ascii","shredded_text.ascii");
#endif
    text_sh.get_input();
    text_sh.do_shredder();
    text_sh.create_output();
  }

  catch(std::exception &e)
  {
#ifndef UTFLAG
    cout<<e.what()<<endl;
    cout <<"Error occured. Abort!" << endl; 
#endif
    return 1;
  }

  return 0;
}
