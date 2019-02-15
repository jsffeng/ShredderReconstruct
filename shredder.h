#ifndef __SHREDDER__
#define __SHREDDER__

#include<vector>
#include<memory>

class TextFileOperation
{
  public:
    static void ReadText(const std::string str_filename, std::vector<std::string> &vec_str_text_lines);
    static void WriteText(const std::string str_filename, const std::vector<std::string> vec_str_text_lines);
};

class TextStripOperation
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    static const int s_random_number_;
  public:
    static void Disorder(std::vector<std::vector<std::string>>& vec_str_input);
    static void Transpose(std::vector<std::vector<std::string>>& vec_str_input, std::vector<std::vector<std::string>>& vec_str_input_trans);
};

class TextShredder
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    const int n_strip_width_;
    const std::string str_in_filename_;
    const std::string str_out_filename_;
    // Minimum blanks will be added to the end of some lines,
    // to ensure all lines have the same number of charactors.
    std::vector<std::string> vec_str_source_data_;
    //Minimum blanks will be added to the end of lines if the size of last column less than n_strip_width_.
    // This is to ensure every colum has the same size as n_strip_width_ defined.
    std::vector<std::vector<std::string>> vec_str_shredded_text_;
    std::vector<std::vector<std::string>> vec_str_trans_shredded_text_;

  public:
    TextShredder(const int n_width=2, const std::string str_in_file="full_text0.ascii", const std::string str_out_file="shredded_text0.ascii"); 

    //populate vec_str_source_data_ from input file
    void GetInput();
    //write vec_str_shredded_text_ to output file by delimiter "|"
    void CreateOutput();
    // shred vec_str_source_data_ and stored into vec_str_shredded_text_.
    void DoTextShredder();
};

#ifdef UTFLAG 
  // To change main() to UTmain() so as to be tested
  extern int UTmain(const int n_width, const std::string str_infile, const std::string str_outfile);
#endif

#endif
