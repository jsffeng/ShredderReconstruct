#ifndef __SHREDDER__
#define __SHREDDER__

#include<vector>
#include<memory>

class TextFileOperation
{
  public:
    // Common function for reading text lines from an input file
    static void ReadText(const std::string str_filename, std::vector<std::string> &vec_str_text_lines);
    // Common function for writing text strips to an output file
    static void WriteText(const std::string str_filename, const std::vector<std::string> vec_str_text_lines);
};

// A class to contain common functions to deal with a group of shredded text strips stored in vector 
class TextStripOperation
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    // Random number used by Disorder() to generate shredded text strip in a random oder
    static const int s_random_number_;
  public:
    // Re-order the vector compoent - each componet contain a vector to store a text strip
    static void Disorder(std::vector<std::vector<std::string>>& vec_str_input);
    // Transpose row and column(each column is for a text strip) 
    static void Transpose(std::vector<std::vector<std::string>>& vec_str_input, std::vector<std::vector<std::string>>& vec_str_input_trans);
};

class TextShredder
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    // The width of shredded text strips. 
    const int n_strip_width_;
    // Input file name - contains a page for shredding
    const std::string str_in_filename_;
    // Output file name - contains shredded text strips.
    const std::string str_out_filename_;
    // The following variable will contain all texts in a page, each line will be stored in 
    // a vector component. Minimum number of blanks will be added to the end of some lines by 
    // TextFileOperation::ReadText() to ensure each lines have the same number of charactors 
    // for further process by programme.
    std::vector<std::string> vec_str_source_data_;
    // Appropriate number of blanks will be added to last shredded colomn if the size of last column 
    // less than n_strip_width_ . This is to ensure every colum has the same size as n_strip_width_ 
    // defined.
    std::vector<std::vector<std::string>> vec_str_shredded_text_;
    // Transpose of vec_str_shredded_text_
    std::vector<std::vector<std::string>> vec_str_trans_shredded_text_;

  public:
    TextShredder(const int n_width=2, const std::string str_in_file="full_text0.ascii", const std::string str_out_file="shredded_text0.ascii"); 

    // Populate vec_str_source_data_ from input file
    void GetInput();
    // Print vec_str_shredded_text_ to output file by delimiter "|"
    void CreateOutput();
    // Shred text lines stored vec_str_source_data_ and save shredded result into vec_str_shredded_text_
    void DoTextShredder();
};

#ifdef UTFLAG 
  // To change main() to UTmain() so as to be tested by Unit Testing
  extern int UTmain(const int n_width, const std::string str_infile, const std::string str_outfile);
#endif

#endif
