#ifndef __SHREDDER__
#define __SHREDDER__

#include<vector>
#include<memory>

class file_operation
{
  public:
    static void read_text(const std::string file_nm, std::vector<std::string> &text_lines);
    static void write_text(const std::string file_nm, const std::vector<std::string> text_lines);
};

class text_strip_operation
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    static const int rand_num;
  public:
    static void disorder(std::vector<std::vector<std::string>>& input);
    static void transpose(std::vector<std::vector<std::string>>& input, std::vector<std::vector<std::string>>& input_r);
};

class shredder
{
#ifdef UTFLAG 
  public:
#else 
  private:
#endif
    const int strip_width;
    const std::string infilename;
    const std::string outfilename;
    // Minimum blanks will be added to the end of some lines,
    // to ensure all lines have the same number of charactors.
    std::vector<std::string> source_data;
    // Minimum blanks will be added to the end of lines if the size of last column less than strip_width.
    // This is to ensure every colum has the same size as strip_width defined.
    std::vector<std::vector<std::string>> shredded_text;
    std::vector<std::vector<std::string>> trans_shredded_text;

  public:
    shredder(const int width=2, const std::string infile="full_text0.ascii", const std::string outfile="shredded_text0.ascii"); 

    //populate source_data from input file
    void get_input();
    //write shredded_text to output file by delimiter "|"
    void create_output();
    // shred source_data and stored into shredded_text.
    void do_shredder();
};

#ifdef UTFLAG 
  // To change main() to UTmain() so as to be tested
  extern int UTmain(const int width, const std::string infile, const std::string outfile);
#endif

#endif
