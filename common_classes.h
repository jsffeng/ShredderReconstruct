#ifndef __COMMON_CLASS__
#define __COMMON_CLASS__

#include<vector>
#include<string>

#ifdef UTFLAG 
#include<set>
#else
#include<unordered_set>
#endif

class TextFileOperation
{
  public:

    // Common function for reading text lines from an input file, usually used read text page
    static void ReadText(const std::string str_filename, std::vector<std::string> &vec_str_text_lines);

    // Common function for reading text lines from an input file, usually used to store dictionary
#ifdef UTFLAG 
    static void ReadText(const std::string str_filename, std::set<std::string> &uset_str_text_lines);
#else
    static void ReadText(const std::string str_filename, std::unordered_set<std::string> &uset_str_text_lines);
#endif

    // Common function for writing text strips to an output file
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

    // Re-order the vector component - each componet contain a vector to store a text strip
    static void Disorder(std::vector<std::vector<std::string>>& vec_str_input);

    // Transpose multiple columns, and each column is for a text strip
    static void Transpose(std::vector<std::vector<std::string>>& vec_str_input, std::vector<std::vector<std::string>>& vec_str_input_trans);

    static void MergeText(std::vector<std::vector<std::string>> & vec_str_input, std::vector<std::string>& vec_str_text);
};

class StringWordOperation
{
  public:
    static bool RemoveWordSuffix(std::string &str_lookup_key);
    static void FindLookupWordLeft(std::string & str_line, std::string & str_key, int n_column_width);
    static void FindLookupWordRight(std::string & str_line, std::string & str_key, int n_column_width);
};

#endif
