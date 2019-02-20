ifndef __UNSHREDDER__
#define __UNSHREDDER__

#include<vector>
#include<string>

// Data types used by multiple classes
typedef struct {
	float f_match_rate;
	float f_notmatch_rate;
} MatchRate;

typedef enum 
{
    LEFT = 0,
    RIGHT,
    LIMIT = 2
} TwoWayDirections;


class ColumnMatchManager
{
#ifdef UTFLAG
  public:
#else
  private:
    vector<vector<string>> vec_text_columns_;
    vector<string> vec_new_column_;

    // Called by CalculateMatchRate
    void BuildLookupKey(vector<string> &vec_key_column, TwoWayDirections enum_direct);
#endif
  public:
    // Record match rate By different directions: LEFT and RIGHT
    MatchRate column_match_rate_[LIMIT];

    ColumnMatchManager(vector<vector<string>> & vec_text_columns, vector<string> & vec_new_column);
    // Populate column_match_rate_[]
    void CalculateMatchRate();
    bool RemoveKeySuffix(string &str_lookup_key);
};

class ColumnSelectManager
{
#ifdef UTFLAG
  public:
#else
  private:
    typedef struct {
      int n_number_in_pool;
      TwoWayDirections enum_best_match_direct;
    } BestMatchColumn;

#ifdef UTFLAG
  public:
#else
  private:
    BestMatchColumn best_match_column_;
    vector<vector<string>> vec_selected_columns_;
    vector<vector<string>> vec_column_pool_;
    // Called only by RebuildColumnsByBestMatch 
    // Find best match in vec_column_pool_, if succeed, populate best_match_column_ , if fail, set
    // b_failure_flag_ yes
    void FindBestMatch(); 
    void DeleteFromColumnPool();

  public:
    // b_failure_flag_ default is no, yes means no matching can be found in vec_column_pool_.
    bool b_failure_flag_;
    // Setup best_match_column_.n_number_in_pool=-1,best_match_column_.enum_best_match_direct=0
    // Setup b_failure_flag_ = no 
    ColumnSelectManager();
    void Init(vector<vector<string>> & vec_text_columns); 
    // Find the best matching column and move it from vec_column_pool_ to vec_selected_columns_
    int RebuildColumnsByBestMatch(); 
    void AddToSelectedColumns();

};

// To construct text strips to a text page
class TextUnshredder
{
#ifdef UTFLAG
  public:
#else
  private:
    const string str_in_filename_;
    const string str_out_filename_;
    vector<vector<string>> vec_text_columns_;
    // Store final result
    vector<string> vec_merged_text_;
    ColumnSelectManager column_select_manager_;

  public:
    bool b_premature_flag_;
    // Size of unselected columns
    int n_premature_column_number_;
    // Set b_premature_flag_ = no, n_premature_column_number_ = 0
    TextUnshredder(const string str_in_file, const string str_out_file);
    // Populate vec_text_columns_ from str_in_filename_, with calling text_strip::transpose
    void GetInput(); 
    void DoTextUnshredder();
    // Generate final output file with vec_merged_text_
    void CreateOutput(); 
}

#endif
