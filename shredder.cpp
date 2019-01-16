#include<fstream>
#include<iostream>
#include "shredder.h"
#include "singleton.h"

using namespace std;

void file_operation::read_text(const string file_nm, vector<string> &text_lines)
{
  string line;
  ifstream infile;

  infile.open(file_nm);
  if(!infile)
  {
     throw runtime_error("file cannot open to read!");
  }

  text_lines.clear();

  vector<string> temp;
  unsigned int len;
  unsigned int maxlen=0;
  while(getline(infile, line))
  {
      len=line.length();
      if ( len >  maxlen) maxlen = len;
      text_lines.push_back(line);
  }

  infile.close();

  // Convert blank at end of each line to ascii space charactors  
  auto iter=text_lines.begin();
  unsigned int siz;
  while (iter != text_lines.end()) 
  {
    siz=iter->size();
    while (siz < maxlen )
    { 
      iter->append(" ");
      siz++;
    }
    iter++;
  }

}

void file_operation::write_text(const string file_nm, const vector<string> text_lines)
{
  ofstream outfile;

  outfile.open (file_nm);
  if(!outfile)
  {
     throw runtime_error("file cannot open to write!");
  }

  for( int i=0; i < text_lines.size(); i++)
  {
      outfile << text_lines[i]<<endl;

  }
  outfile.close();
}

text_strip::text_strip (const int rand) : rand_num(rand) 
{
  if ( rand <= 0 )
  {
    throw runtime_error("rand_num value is invalid!");
  }
}

void text_strip::disorder(vector<vector<string>>& input) 
{
  unsigned int temp_t=0; 

  if ( input.size() == 0 )
  {
    throw runtime_error("Invalid input!");
  }
  else
  {
    Singleton_rand::max_val = input.size() - 1;
    Singleton_rand& ran = Singleton_rand::getInstance();

    for( int i=0; i < rand_num; i++)
    {
       temp_t =ran.rand_generator();
       input.insert(input.end(),input[temp_t]);	
       input.erase(input.begin() + temp_t);
    }
  }
}

void text_strip::transpose(vector<vector<string>>& input, vector<vector<string>>& input_r)
{
  vector<string> temp;
  input_r.clear();

  if (input.size() == 0)
  { 
     throw runtime_error("Invalid input!");
  } 

  for( int i=0; i < input[0].size(); i++)
  {
    for( int j=0; j< input.size(); j++)
    {
      temp.push_back(input[j][i]);
    }

    input_r.push_back(temp);
    temp.clear();
  }

}

shredder::shredder(const int num, const int width, const string infile, const string outfile) : text_strip(num), strip_width(width), infilename(infile), outfilename(outfile) 
{
}

void shredder::get_input()
{
  file_operation::read_text(infilename,source_data);
}

void shredder::create_output()
{
  vector<string> temp;
  string sub;

  if (shredded_text.size() == 0 )
  {
    throw runtime_error("Nothing to print to output file!");
  }

  for( int i=0; i < shredded_text.size(); i++)
  {
    for( int j=0; j< shredded_text[i].size(); ++j)
    {
       sub.append("|"+shredded_text[i][j]); 
    } 
  
    sub.append("|"); 
    temp.push_back(sub);
    sub.clear();
  }

  file_operation::write_text(outfilename, temp);
}

void shredder::do_shredder()
{
  vector<string> temp;
  string sub;

  if ( source_data.size() == 0 )
  {
    throw runtime_error("source_data empty, no data to shred!");
  }

  if ( strip_width <= 0 )
  {
    throw runtime_error("strip_width value is invalid!");
  }

  shredded_text.clear();
  trans_shredded_text.clear();

  auto iter=source_data.begin();

  while(iter != source_data.end())
  {
    int position=0;
    while(position < iter->size())
    {
      sub=iter->substr(position,strip_width); 
      // Ensure the last column in the paragraph has the same width of strip
      while (sub.length() < strip_width) sub.append(" ");
      temp.push_back(sub);
      position = position + strip_width;
    }
    shredded_text.push_back(temp);
    temp.clear();

    iter++;
  }
   
  // Transpose shredded_text to trans_shredded_text
  transpose(shredded_text,trans_shredded_text);

#ifndef UTFLAG
  disorder(trans_shredded_text);
#endif 

  // Empty shredded_text here
  shredded_text.clear();

  // Transpose trans_shredded_text to shredded_text
  transpose(trans_shredded_text,shredded_text);

}

