//////////////////////////////////////////////////////////////////////////////////////////  
//
//  Naming conventions used in source files and header files. 
//
//  Let's assume the following text content to be unshredded by this programme.
//  
//  a1|a2|a3
//  b1|b2|b3
//  c1|c2|c3
//  d1|d2|d3
// 
//  In above example,, a1, a2, b1, etc. is a string with same width, "|" is the delimiter
//  used by programme.
//
//  In the programme, above data will be stored into following 2 dimensional vector:
//  {
//    {a1,b1,c1,d1},
//    {a2,b2,c2,d2},
//    {a3,b3,c3,d3}
//  }
//  
//  A text strip refers to {a1,b1,c1,d1}, or {a2,b2,c2,d2}, etc. 
//  A "column" is often used to refer a text strip in data members or variables in the programme.
//  
//////////////////////////////////////////////////////////////////////////////////////////  


// Unit Testing by using BOOST
#define BOOST_TEST_MODULE main_unshredder_test
#include <boost/test/included/unit_test.hpp>
#include <thread>
#include "../common_classes.h"
#include "../text_unshredder.h"
#include "../singleton_diction.h"
#include "../singleton_random.h"
#include "../column_match_manager.h"
#include "../thread_controller.h"

using namespace std;

// Include Fixtures
#include "fixture.in"
#include "fixture_unshredder.in"

// Class TESTSingleton
// Test class SingletonRandom::GetInsance() under multi-threads
// Disable this test_suite by default as the invoke of SingletonRandom has conflicts with the rest
// testing which may use SingletonRandom class.
// It can be invoked by option --run_test=SingletonRandom_test seperately with the rest tests.
BOOST_FIXTURE_TEST_SUITE (TESTSingleton_test,TESTSingleton ,* boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE (TESTGetInstance_test)
{
  int n_number = 10; 
  thread test_thread[n_number]; 

  for (int i = 0; i < n_number; ++i) 
    test_thread[i] = thread(TESTGetInstance);

  for (int i = 0; i < n_number; ++i)
    test_thread[i].join();

  BOOST_CHECK(b_instance_equal == true);
  BOOST_CHECK(b_insance_update == n_number);
}

BOOST_AUTO_TEST_SUITE_END ()

// Class WordStringOperation
BOOST_AUTO_TEST_SUITE (WordStringOperation_test);

BOOST_AUTO_TEST_CASE (RemoveWordSuffix)
{
  vector<string> sample = {"goods", " goods", "goods ", "uses", "being", "he's", "added", "us", "ding", "bed", "ed"};
  vector<string> sample_suffix_removed = {"good"," good", "goods ","us", "be", "he'", "add", "us", "ding", "bed", "ed"};
  
  for (int i = 0; i < sample.size(); ++i)
     WordStringOperation::RemoveWordSuffix(sample[i]); 

  BOOST_CHECK(sample == sample_suffix_removed);

  vector<string> sample1 = {"goods", "good", "bed", "goods ", " goods"};
  BOOST_CHECK(true == WordStringOperation::RemoveWordSuffix(sample1[0]));
  BOOST_CHECK(false == WordStringOperation::RemoveWordSuffix(sample1[1]));
  BOOST_CHECK(false == WordStringOperation::RemoveWordSuffix(sample1[2]));
  BOOST_CHECK(false == WordStringOperation::RemoveWordSuffix(sample1[3]));
  BOOST_CHECK(true == WordStringOperation::RemoveWordSuffix(sample1[4]));
  
  string sample2;
  BOOST_CHECK_THROW(WordStringOperation::RemoveWordSuffix(sample2), exception);   
}

BOOST_AUTO_TEST_CASE (FindLookupWordLeft_test)
{
   
  vector<string> vec_str_line =   {"a1a2a3a4a5  a7a8a9", \
                                   "    a3a4a5a6a7  a9  a11a12 ", \
                                   "      a4a5a6a7a8a9", \
                                   "        a5a6a7a8", \
                                   "a1  a3a4  a6a7a8a9a10a11", \
                                   "a1  a3  a5a6a7a8  " \
                                 };

  vector<string> vec_str_key_wid6 = {"a1a2a3a4a5", \
                                   "a3a4a5a6a7", \
                                   "", \
                                   "", \
                                   "a3a4", \
                                   "" \
                                };

  vector<string> vec_str_key_wid_2 = {"a1a2a3a4a5", \
                                   "", \
                                   "", \
                                   "", \
                                   "", \
                                   "" \
                                 };

  vector<string> vec_str_key_wid8 = {"a1a2a3a4a5", \
                                   "a3a4a5a6a7", \
                                   "a4a5a6a7a8a9", \
                                   "", \
                                   "", \
                                   "" \
                                 };

  vector<string> vec_str_key_t = {"","","","","",""}; 

  int width = 6;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid6);

  width = 2;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid_2);

  width = 8;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid8);
  
  string str_key_t;
  WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);

  // str_key_t is not empty
  WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);
  str_key_t.clear();

  // Throw exceptions for invalid input
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, 0), exception);
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, -1), exception); 
  BOOST_CHECK_NO_THROW(WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, vec_str_line[0].size() - 1)); 
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, vec_str_line[0].size()), exception); 
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, vec_str_line[0].size() + 1), exception); 
  
  string str_line;
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordLeft(str_line, str_key_t, 8), exception);   
}

BOOST_AUTO_TEST_CASE (FindLookupWordRight_test)
{
   
  vector<string> vec_str_line =   {"a1a2a3  a5a6a7a8a9", \
                                     "a2a3a4a5a6a7    ", \
                                   "  a2a3a4a5a6      ", \
                                   "a1  a3a4a5        ", \
                               "abaca1a2a3a4  a6a7  a9", \
                                     "a2a3a4a5  a7  a9" \
                                 };

  vector<string> vec_str_key_wid6 = {"a5a6a7a8a9", \
                                   "a2a3a4a5a6a7", \
                                   "", \
                                   "", \
                                   "a6a7", \
                                   "" \
                                };

  vector<string> vec_str_key_wid_2 = {"a5a6a7a8a9", \
                                   "", \
                                   "", \
                                   "", \
                                   "", \
                                   "" \
                                 };

  vector<string> vec_str_key_wid8 = {"a5a6a7a8a9", \
                                   "a2a3a4a5a6a7", \
                                   "a2a3a4a5a6", \
                                   "", \
                                   "", \
                                   "" \
                                 };

  vector<string> vec_str_key_t = {"","","","","",""}; 

  int width = 6;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid6);

  width = 2;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid_2);

  width = 8;
  for (int i = 0; i < vec_str_line.size(); ++i)
    WordStringOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid8);
  
  string str_key_t;
  WordStringOperation::FindLookupWordRight(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);

  // str_key_t is not empty
  WordStringOperation::FindLookupWordRight(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);
  str_key_t.clear();

  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordRight(vec_str_line[0], str_key_t, 0), exception); 
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordRight(vec_str_line[0], str_key_t, -1), exception); 
  
  string str_line;
  BOOST_CHECK_THROW(WordStringOperation::FindLookupWordRight(str_line, str_key_t, 8), exception);   
}

BOOST_AUTO_TEST_SUITE_END ()

// Class SingletonDiction
BOOST_AUTO_TEST_SUITE (SingletonDiction_test)

BOOST_AUTO_TEST_CASE (GetInstance_test)
{
  SingletonDiction & dict0 = SingletonDiction::GetInstance();
  SingletonDiction & dict1 = SingletonDiction::GetInstance();

  BOOST_CHECK (&dict0 == &dict1);

  SingletonDiction & dict2 = SingletonDiction::GetInstance();
  BOOST_CHECK (&dict0 == &dict2);
}

BOOST_AUTO_TEST_CASE (Init_test)
{
  SingletonDiction & dict = SingletonDiction::GetInstance();
  BOOST_CHECK(0 == dict.uset_dictionary_.size());
  dict.Init();
  BOOST_CHECK(17641 == dict.uset_dictionary_.size());
  BOOST_CHECK (dict.uset_dictionary_.find("zucchini") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("Zulu") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("a") != (dict.uset_dictionary_.end()));
}

BOOST_AUTO_TEST_CASE (BuildWordPiece_test)
{
  SingletonDiction & dict = SingletonDiction::GetInstance();
  BOOST_CHECK(0 != dict.uset_dictionary_.size());

  if (dict.uset_dictionary_.empty())
    dict.Init();

  dict.BuildWordPiece();
  BOOST_WARN_GE(dict.uset_dictionary_.size(),17640*5);
  BOOST_CHECK (dict.uset_dictionary_.find("zucchini") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("ucch") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("zucchi") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("cch") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("Zulu") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("ulu") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("Zu") != (dict.uset_dictionary_.end()));
  BOOST_CHECK (dict.uset_dictionary_.find("a") != (dict.uset_dictionary_.end()));

  dict.uset_dictionary_.clear();
  BOOST_CHECK_THROW(dict.BuildWordPiece(), exception);   
}

BOOST_AUTO_TEST_CASE (LookupDict_test)
{
  SingletonDiction & dict = SingletonDiction::GetInstance();
  if (dict.uset_dictionary_.empty())
  {
    dict.Init();
    dict.BuildWordPiece();
  }

  BOOST_CHECK (true == dict.LookupDict("Zu"));
  BOOST_CHECK (true == dict.LookupDict("ucch"));
  BOOST_CHECK (true == dict.LookupDict("cch"));
  BOOST_CHECK (true == dict.LookupDict("a"));
  BOOST_CHECK (true == dict.LookupDict("good"));
  BOOST_CHECK (true == dict.LookupDict("birth"));

  BOOST_CHECK (false == dict.LookupDict("wz"));
  BOOST_CHECK (false == dict.LookupDict("z"));
  BOOST_CHECK (false == dict.LookupDict("goood"));
  BOOST_CHECK (false == dict.LookupDict("Birth"));
  BOOST_CHECK (false == dict.LookupDict("Birth"));

  BOOST_CHECK (false == dict.LookupDict("!"));

  string key = "good";
  BOOST_CHECK (true == dict.LookupDict(key));

  key.clear();
  BOOST_CHECK_THROW(dict.LookupDict(key), exception);   

  BOOST_CHECK_THROW(dict.LookupDict(""), exception);   
}

BOOST_AUTO_TEST_SUITE_END ()

// Class ColumnMatchManager
BOOST_FIXTURE_TEST_SUITE(ColumnMatchManager_test, Fixture_columns);

BOOST_AUTO_TEST_CASE (constructor_test)
{
  ColumnMatchManager column_match_mgr1(Fix_columns_wid_2, Fix_columnX_wid_2);
  BOOST_CHECK (Fix_columns_wid_2 == column_match_mgr1.vec_text_columns_);
  BOOST_CHECK (Fix_columnX_wid_2 == column_match_mgr1.vec_new_column_);

  ColumnMatchManager column_match_mgr2 = column_match_mgr1;

  ColumnMatchManager column_match_mgr3 = ColumnMatchManager(Fix_columns_wid_2, Fix_columnX_wid_2);
  BOOST_CHECK (Fix_columns_wid_2 == column_match_mgr3.vec_text_columns_);
  BOOST_CHECK (Fix_columnX_wid_2 == column_match_mgr3.vec_new_column_);

  vector<vector<string>> columns_A = { {" ", "","GOOD"},{"OK ","", "YES"}};

  vector<string> column_A =  {"abcd", "abcd","abcd"};
  BOOST_CHECK_NO_THROW(ColumnMatchManager(columns_A, column_A));   

  // When strings in the new column didn't have the same length.
  vector<string> column_A1 =  {"abcd", "abcd","abc"};
  BOOST_CHECK_THROW(ColumnMatchManager(columns_A, column_A1), exception);   
 
  // When size of new column is not equal to the size of other columns
  vector<string> column_A2 =  {"abcd", "abcd"};
  BOOST_CHECK_THROW(ColumnMatchManager(columns_A, column_A2), exception);   

  // When columns is empty
  columns_A.clear();
  BOOST_CHECK_THROW(ColumnMatchManager(columns_A, column_A), exception);   

  // When the new column is empty
  vector<vector<string>> columns_B = { {" ", "","GOOD"},{"OK ","", "YES"}};
  vector<string> column_B;
  BOOST_CHECK_THROW(ColumnMatchManager(columns_B, column_B), exception);   
}

BOOST_AUTO_TEST_CASE (BuildLookupKey_test)
{
  vector<string> vec_keys;
  ColumnMatchManager column_match_mgr1(Fix_columns_wid_2, Fix_columnX_wid_2);

  vector<string> vec_keys_right1 = {"good", "will", "wil"};
  column_match_mgr1.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right1 == vec_keys);

  vector<string> vec_keys_left1 = {"0", "0", "iluse"};
  column_match_mgr1.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left1 == vec_keys);

  ColumnMatchManager column_match_mgr2(Fix_columns_wid_3, Fix_columnY_wid_3);

  vector<string> vec_keys_right2 = {"stas", "borr", "beaus"};
  vec_keys.clear();
  column_match_mgr2.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right2 == vec_keys);

  vector<string> vec_keys_left2 = {"is", "orrow", "ause"};
  vec_keys.clear();
  column_match_mgr2.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left2 == vec_keys);

  // vec_keys is not empty
  column_match_mgr2.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left2 == vec_keys);

  // Verify special characters correctly converted into blank delimiter
  vector<vector<string>> columns_special = { {"go?yes!", "[no],he","i won't"},{"my-bed ","(nowayf", "said:ok"}};
  vector<string> column_letter =  {"abcdefg", "abcdefg","abcdefg"};

  ColumnMatchManager column_match_mgr3(columns_special, column_letter);

  vector<string> vec_keys_right3 = {"0", "nowayfabcdefg", "okabcdefg"};
  vec_keys.clear();
  column_match_mgr3.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right3 == vec_keys);

  vector<string> vec_keys_left3 = {"abcdefggo", "0", "abcdefgi"};
  vec_keys.clear();
  column_match_mgr3.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left3 == vec_keys);

  vector<string> column_spec =  {"abc,efg", "abc.efg","abc:efg"};

  ColumnMatchManager column_match_mgr4(columns_special, column_spec);

  vector<string> vec_keys_right4 = {"0", "nowayfabc", "okabc"};
  vec_keys.clear();
  column_match_mgr4.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right4 == vec_keys);

  vector<string> vec_keys_left4 = {"efggo", "0", "efgi"};
  vec_keys.clear();
  column_match_mgr4.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left4 == vec_keys);

  // Verify the handling on 's, 'd
  vector<string> column_spec_s_d1 =  {"abcdefg", "abc'd,g","abc's g"};
  ColumnMatchManager column_match_mgr5(columns_special, column_spec_s_d1);

  vector<string> vec_keys_right5 = {"0", "nowayfabc", "okabc"};
  vec_keys.clear();
  column_match_mgr5.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right5 == vec_keys);

  vector<string> column_spec_s_d2 =  {"abcdefg", "abc  fg","'s abcd"};
  ColumnMatchManager column_match_mgr6(columns_special, column_spec_s_d2);

  vector<string> vec_keys_right6 = {"0", "nowayfabc", "ok"};
  vec_keys.clear();
  column_match_mgr6.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right6 == vec_keys);

  // Verify the handling on capital letters and non alphabetic characters
  vector<string> column_spec_cap =  {"abcdefg", "abC,efg","abc'h g"};
  ColumnMatchManager column_match_mgr7(columns_special, column_spec_cap);

  vector<string> vec_keys_right7 = {"0", "nowayfabc", "0"};
  vec_keys.clear();
  column_match_mgr7.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right7 == vec_keys);

  vector<string> column_spec_digit =  {"abcdefg", "abc\\efg","abc3 fg"};
  ColumnMatchManager column_match_mgr8(columns_special, column_spec_digit);

  vector<string> vec_keys_right8 = {"0", "0", "0"};
  vec_keys.clear();
  column_match_mgr8.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right8 == vec_keys);
}

BOOST_AUTO_TEST_CASE (CalculateMatchRate_test)
{
  SingletonDiction & dict = SingletonDiction::GetInstance();
  if (dict.uset_dictionary_.empty())
  {
    dict.Init();
    dict.BuildWordPiece();
  }

  // For columns with strip width = 2
  ColumnMatchManager column_match_mgrX(Fix_columns_wid_2, Fix_columnX_wid_2);
  column_match_mgrX.CalculateMatchRate();
  
  BOOST_CHECK (column_match_mgrX.column_match_rate_[0].f_match_rate == 0.5);
  BOOST_CHECK_LT (column_match_mgrX.column_match_rate_[0].f_notmatch_rate, 0.3333334);
  BOOST_CHECK (column_match_mgrX.column_match_rate_[1].f_match_rate == 4);
  BOOST_CHECK_LT (column_match_mgrX.column_match_rate_[1].f_notmatch_rate, 0.0000001);
  
  ColumnMatchManager column_match_mgrY(Fix_columns_wid_2, Fix_columnY_wid_2);
  column_match_mgrY.CalculateMatchRate();
  
  BOOST_CHECK_LT (abs(column_match_mgrY.column_match_rate_[0].f_match_rate - 3), 0.0000001);
  BOOST_CHECK_LT (column_match_mgrY.column_match_rate_[0].f_notmatch_rate, 0.0000001);
  BOOST_CHECK (column_match_mgrY.column_match_rate_[1].f_match_rate == 1.5);
  BOOST_CHECK_LT (column_match_mgrY.column_match_rate_[1].f_notmatch_rate, 0.3333334);
  
  ColumnMatchManager column_match_mgrZ(Fix_columns_wid_2, Fix_columnZ_wid_2);
  column_match_mgrZ.CalculateMatchRate();
  
  BOOST_CHECK (column_match_mgrZ.column_match_rate_[0].f_match_rate == 0.5);
  BOOST_CHECK_LT (column_match_mgrZ.column_match_rate_[0].f_notmatch_rate, 0.3333334);
  BOOST_CHECK_LT (abs(column_match_mgrZ.column_match_rate_[1].f_match_rate - 1), 0.0000001);
  BOOST_CHECK_LT (column_match_mgrZ.column_match_rate_[1].f_notmatch_rate, 0.3333334);
  
  // For columns with strip width = 3
  ColumnMatchManager column_match_mgrX1(Fix_columns_wid_3, Fix_columnX_wid_3);
  column_match_mgrX1.CalculateMatchRate();
  
  BOOST_CHECK (column_match_mgrX1.column_match_rate_[0].f_match_rate == 1);
  BOOST_CHECK_LT (column_match_mgrX1.column_match_rate_[0].f_notmatch_rate, 0.3333334);
  BOOST_CHECK (column_match_mgrX1.column_match_rate_[1].f_match_rate == 3);
  BOOST_CHECK_LT (column_match_mgrX1.column_match_rate_[1].f_notmatch_rate, 0.0000001);
  
  ColumnMatchManager column_match_mgrY1(Fix_columns_wid_3, Fix_columnY_wid_3);
  column_match_mgrY1.CalculateMatchRate();
  
  BOOST_CHECK_LT (abs(column_match_mgrY1.column_match_rate_[0].f_match_rate - 4), 0.0000001);
  BOOST_CHECK_LT (column_match_mgrY1.column_match_rate_[0].f_notmatch_rate, 0.0000001);
  BOOST_CHECK (column_match_mgrY1.column_match_rate_[1].f_match_rate == 4 );
  BOOST_CHECK_LT (column_match_mgrY1.column_match_rate_[1].f_notmatch_rate, 0.0000001);
  
  ColumnMatchManager column_match_mgrZ1(Fix_columns_wid_3, Fix_columnZ_wid_3);
  column_match_mgrZ1.CalculateMatchRate();
  
  BOOST_CHECK (column_match_mgrZ1.column_match_rate_[0].f_match_rate == 1);
  BOOST_CHECK_LT (column_match_mgrZ1.column_match_rate_[0].f_notmatch_rate, 0.0000001);
  BOOST_CHECK_LT (abs(column_match_mgrZ1.column_match_rate_[1].f_match_rate - 0.5), 0.0000001);
  BOOST_CHECK_LT (column_match_mgrZ1.column_match_rate_[1].f_notmatch_rate, 0.3333334);
}

BOOST_AUTO_TEST_SUITE_END ()

// Class  ColumnSelectManager
BOOST_FIXTURE_TEST_SUITE (ColumnSelectManager_test, Fixture_columns);

BOOST_AUTO_TEST_CASE (constructor_test)
{
  ColumnSelectManager column_sel_mgr1;
  BOOST_CHECK(-1 == column_sel_mgr1.best_match_column_.n_number_in_pool);
  BOOST_CHECK(LEFT == column_sel_mgr1.best_match_column_.enum_best_match_direct);
  BOOST_CHECK(false == column_sel_mgr1.b_failure_flag_);
}

BOOST_AUTO_TEST_CASE (Init_test)
{
  ColumnSelectManager column_sel_mgr1;
  column_sel_mgr1.Init(Fix_columns_wid_3);

  vector<vector<string>> vec_selected_wid_3;
  vector<vector<string>> vec_pool_wid_3;
  vec_selected_wid_3.emplace_back(Fix_columns_wid_3[0]); 
  vec_pool_wid_3.assign(Fix_columns_wid_3.begin()+1, Fix_columns_wid_3.end()); 

  BOOST_CHECK(column_sel_mgr1.vec_selected_columns_ == vec_selected_wid_3);
  BOOST_CHECK(column_sel_mgr1.vec_column_pool_ == vec_pool_wid_3);

  ColumnSelectManager column_sel_mgr2;
  column_sel_mgr2.Init(Fix_columns_wid_2);

  vector<vector<string>> vec_selected_wid_2;
  vector<vector<string>> vec_pool_wid_2;
  vec_selected_wid_2.emplace_back(*Fix_columns_wid_2.begin()); 
  vec_pool_wid_2.assign(Fix_columns_wid_2.begin()+1, Fix_columns_wid_2.end()); 

  BOOST_CHECK(column_sel_mgr2.vec_selected_columns_ == vec_selected_wid_2);
  BOOST_CHECK(column_sel_mgr2.vec_column_pool_ == vec_pool_wid_2);

  // Verify invalid input to constructor
  vector<vector<string>> vec_init1; 
  ColumnSelectManager column_sel_mgr3;
  BOOST_CHECK_THROW(column_sel_mgr3.Init(vec_init1), exception);   

  vector<vector<string>> vec_init2 = {{"abc", "def", "ghi"}}; 
  ColumnSelectManager column_sel_mgr4;
  BOOST_CHECK_THROW(column_sel_mgr4.Init(vec_init2), exception);   

  vector<vector<string>> vec_init3 = {{"abc", "def", "ghi"}, {"abc", "def", "ghi"}}; 
  ColumnSelectManager column_sel_mgr5;
  BOOST_CHECK_NO_THROW(column_sel_mgr5.Init(vec_init3));   
}

BOOST_AUTO_TEST_CASE (FindBestMatch_test)
{
  ColumnSelectManager column_sel_mgr1;
  column_sel_mgr1.Init(Fix_columns_wid_2);

  column_sel_mgr1.FindBestMatch();

  BOOST_CHECK(column_sel_mgr1.best_match_column_.n_number_in_pool == 0);
  BOOST_CHECK(column_sel_mgr1.best_match_column_.enum_best_match_direct == RIGHT);
  BOOST_CHECK(column_sel_mgr1.b_failure_flag_ == false);

  vector<vector<string>> vec_columns_wid_3;
  vec_columns_wid_3.assign(Fix_columns_wid_3.begin()+1, Fix_columns_wid_3.end());
 
  ColumnSelectManager column_sel_mgr2;
  column_sel_mgr2.Init(vec_columns_wid_3);

  column_sel_mgr2.FindBestMatch();

  BOOST_CHECK(column_sel_mgr2.best_match_column_.n_number_in_pool == 0);
  BOOST_CHECK(column_sel_mgr2.best_match_column_.enum_best_match_direct == RIGHT);
  BOOST_CHECK(column_sel_mgr2.b_failure_flag_ == false);

  // Throw exceptions because best_match_column_.n_number_in_pool != -1 
  BOOST_CHECK_THROW(column_sel_mgr2.FindBestMatch(), exception);   

  // Won't depend on the initial value of b_failure_flag_,
  // which means FindBestMatch() can be re-run even if failed last time.
  column_sel_mgr2.b_failure_flag_ = true;
  column_sel_mgr2.best_match_column_.n_number_in_pool = -1;
  column_sel_mgr2.FindBestMatch();  
  BOOST_CHECK(column_sel_mgr2.best_match_column_.n_number_in_pool == 0);
  BOOST_CHECK(column_sel_mgr2.best_match_column_.enum_best_match_direct == RIGHT);
  BOOST_CHECK(column_sel_mgr2.b_failure_flag_ == false);

  // Throw exceptions 
  ColumnSelectManager column_sel_mgr3;
  BOOST_CHECK_THROW(column_sel_mgr3.FindBestMatch(), exception);   
}

BOOST_AUTO_TEST_CASE (AddToSelectedColumns_test)
{
  ColumnSelectManager column_sel_mgr1;
  column_sel_mgr1.Init(Fix_columns_wid_3);

  // If BestMatchColumn.n_number_in_pool is -1 (default value), throw exception
  BOOST_CHECK_THROW(column_sel_mgr1.AddToSelectedColumns(), exception);   
  
  // Adding from Right
  column_sel_mgr1.best_match_column_.n_number_in_pool = 2;
  column_sel_mgr1.best_match_column_.enum_best_match_direct = RIGHT;

  vector<vector<string>> vec_temp;

  vec_temp.emplace_back(Fix_columns_wid_3[0]);
  vec_temp.emplace_back(Fix_columns_wid_3[3]);

  column_sel_mgr1.AddToSelectedColumns();
  BOOST_CHECK(column_sel_mgr1.vec_selected_columns_ == vec_temp);

  // Adding from Left
  ColumnSelectManager column_sel_mgr2;
  column_sel_mgr2.Init(Fix_columns_wid_2);

  column_sel_mgr2.best_match_column_.n_number_in_pool = 1;
  column_sel_mgr2.best_match_column_.enum_best_match_direct = LEFT;

  vector<vector<string>> vec_temp1;

  vec_temp1.emplace_back(Fix_columns_wid_2[2]);
  vec_temp1.emplace_back(Fix_columns_wid_2[0]);

  column_sel_mgr2.AddToSelectedColumns();
  BOOST_CHECK(column_sel_mgr2.vec_selected_columns_ == vec_temp1);

  // Throw exceptions if n_number_in_pool out of range
  column_sel_mgr2.best_match_column_.n_number_in_pool = column_sel_mgr2.vec_column_pool_.size();
  BOOST_CHECK_THROW(column_sel_mgr2.AddToSelectedColumns(), exception);   
}

BOOST_AUTO_TEST_CASE (DeleteFromColumnPool_test)
{
  ColumnSelectManager column_sel_mgr1;
  column_sel_mgr1.Init(Fix_columns_wid_3);

  // If BestMatchColumn.n_number_in_pool is -1 (default value), throw exception
  BOOST_CHECK_THROW(column_sel_mgr1.DeleteFromColumnPool(), exception);   

  column_sel_mgr1.best_match_column_.n_number_in_pool = 2;
  column_sel_mgr1.best_match_column_.enum_best_match_direct = RIGHT;

  column_sel_mgr1.DeleteFromColumnPool();

  vector<vector<string>> vec_temp;

  // Fix_columns_wid_3[0] will be assigned to vec_selected_columns_ by Init()
  vec_temp.emplace_back(Fix_columns_wid_3[1]);
  vec_temp.emplace_back(Fix_columns_wid_3[2]);

  BOOST_CHECK(column_sel_mgr1.vec_column_pool_ == vec_temp);
  BOOST_CHECK(column_sel_mgr1.best_match_column_.n_number_in_pool == -1);
  BOOST_CHECK(column_sel_mgr1.best_match_column_.enum_best_match_direct == RIGHT);

  // Throw exceptions if n_number_in_pool out of range
  column_sel_mgr1.best_match_column_.n_number_in_pool = column_sel_mgr1.vec_column_pool_.size();
  BOOST_CHECK_THROW(column_sel_mgr1.DeleteFromColumnPool(), exception);   
}

BOOST_AUTO_TEST_CASE (RebuildColumnsByBestMatch)
{
  ColumnSelectManager column_sel_mgr1;
  column_sel_mgr1.Init(Fix_columns_wid_2);

  column_sel_mgr1.RebuildColumnsByBestMatch();

  vector<vector<string>> vec_temp;
  vec_temp.emplace_back(Fix_columns_wid_2[0]);
  vec_temp.emplace_back(Fix_columns_wid_2[1]);

  // Fix_columns_wid_3[0] will be assigned to vec_selected_columns_ by Init()
  BOOST_CHECK(column_sel_mgr1.b_failure_flag_ == false);
  BOOST_CHECK(column_sel_mgr1.vec_selected_columns_ == vec_temp);
  BOOST_CHECK(column_sel_mgr1.best_match_column_.n_number_in_pool == -1);

  column_sel_mgr1.RebuildColumnsByBestMatch();

  BOOST_CHECK(column_sel_mgr1.b_failure_flag_ == true);
  BOOST_CHECK(column_sel_mgr1.vec_selected_columns_ == vec_temp);
  BOOST_CHECK(column_sel_mgr1.best_match_column_.n_number_in_pool == -1);

  // Throw exception if b_failure_flag_ is not false
  BOOST_CHECK_THROW(column_sel_mgr1.RebuildColumnsByBestMatch(), exception);
  
  // Throw exception if best_match_column_.n_number_in_pool is not -1
  column_sel_mgr1.best_match_column_.n_number_in_pool = 1;
  column_sel_mgr1.b_failure_flag_ = false;
  BOOST_CHECK_THROW(column_sel_mgr1.RebuildColumnsByBestMatch(), exception);

  column_sel_mgr1.best_match_column_.n_number_in_pool = -1;
  column_sel_mgr1.b_failure_flag_ = false;
  BOOST_CHECK_NO_THROW(column_sel_mgr1.RebuildColumnsByBestMatch());

  // Throw exceptions if vec_selected_columns_ or vec_column_pool_ is empty
  ColumnSelectManager column_sel_mgr2;
  BOOST_CHECK_THROW(column_sel_mgr2.RebuildColumnsByBestMatch(), exception);

  // PrintVectorString (column_sel_mgr1.vec_selected_columns_, "vec_selected_columns_");
}

BOOST_AUTO_TEST_SUITE_END ()

// Class TextUnshredder
BOOST_FIXTURE_TEST_SUITE (TextUnshredder_test, Fixture_columns);

BOOST_AUTO_TEST_CASE (constructor_test)
{
  TextUnshredder unshred1;

  BOOST_CHECK(false == unshred1.b_premature_flag_);
  BOOST_CHECK(0 == unshred1.n_premature_column_count_);
}

BOOST_FIXTURE_TEST_CASE (GetInput_test, Fixture_data_file)
{
  TextUnshredder unshred1;
  unshred1.GetInput("test_output2.ascii");
  vector<vector<string>> vec_text_columns_wid_2;
  TextStripOperation::Transpose(Fix_shredded_text_wid_2, vec_text_columns_wid_2);
 
  BOOST_CHECK(unshred1.vec_text_columns_ == vec_text_columns_wid_2);

  // PrintVectorString (unshred1.vec_text_columns_, "vec_text_columns_");

  // Throw exceptions 
  TextUnshredder unshred2;
  BOOST_CHECK_THROW(unshred2.GetInput("test/UTinput/test_input.ascii"), exception);   

  BOOST_CHECK_NO_THROW(unshred2.GetInput("test/UTinput/test_shredded_text.ascii"));   

  // With blank character at the end of fist 2 rows.
  BOOST_CHECK_NO_THROW(unshred2.GetInput("test/UTinput/test_shredded_text1.ascii"));   

  // Unexpected format
  BOOST_CHECK_THROW(unshred2.GetInput("test/UTinput/test_shredded_text2.ascii"), exception);   
  BOOST_CHECK_THROW(unshred2.GetInput("test/UTinput/test_shredded_text3.ascii"), exception);   
  BOOST_CHECK_THROW(unshred2.GetInput("test/UTinput/test_shredded_text4.ascii"), exception);   
}


BOOST_FIXTURE_TEST_CASE (CreateOutput_test, Fixture_file_vec)
{
  TextUnshredder unshred1;

  // Won't throw exception if vec_merged_text_ is empty, just write nothing to disk
  BOOST_CHECK_NO_THROW(unshred1.CreateOutput("test/UTinput/test_output.ascii"));   

  // With blanks at the end of lines
  unshred1.vec_merged_text_.assign(Fix_lines.begin(), Fix_lines.end());

  unshred1.CreateOutput("test/UTinput/test_output.ascii");
  BOOST_CHECK(system("diff test_output.ascii test/UTinput/test_output.ascii >/dev/null 2>&1") == 0);

  system("rm -rf test/UTinput/test_output.ascii");
}


BOOST_AUTO_TEST_CASE (DoTextUnshredder_test)
{
  SingletonDiction & dict = SingletonDiction::GetInstance();
  if (dict.uset_dictionary_.empty())
  {
    dict.Init();
    dict.BuildWordPiece();
  }

  TextUnshredder unshred1;

  // This data file can be successfully restored 
  unshred1.GetInput("test/UTinput/test_shredded_text0.ascii");

  unshred1.DoTextUnshredder();
 
  vector<string> vec_merged_text = 
  {
    "Claude Shannon founded information    ",
    "theory, which is the basis of         ", 
    "probabilistic language models and     ",
    "of the code breaking methods that     ",
    "you would use to solve this problem,  ",
    "with the paper titled \"A Mathematical ",
    "Theory of Communication,\" published   ",
    "in this year.                         "
  };

  BOOST_CHECK(unshred1.b_premature_flag_ == false);
  BOOST_CHECK(unshred1.vec_merged_text_ == vec_merged_text);

  TextUnshredder unshred2;

  // This data file can be partially restored - reason why it cannot be fully restored during UT
  // is that vec_selected_columns_ are fixed, which caused each thread generate the same result.
  unshred2.GetInput("test/UTinput/test_shredded_text.ascii");

  unshred2.DoTextUnshredder();
  BOOST_CHECK(unshred2.b_premature_flag_ == true);

  // PrintVectorString (unshred2.vec_merged_text_, "vec_merged_text_");
}

BOOST_AUTO_TEST_SUITE_END ()

// Class ThreadController
BOOST_FIXTURE_TEST_SUITE (ThreadController_test, Fixture_thread_reset);

BOOST_AUTO_TEST_CASE (RecordThreadAbnormals_test)
{
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 0);

  ThreadController::RecordThreadAbnormals();
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 1);

  ThreadController::RecordThreadAbnormals();
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 2);

  // Re-set the static variable so as not to impact the rest tests.
  ThreadStatusDataReset();
}

BOOST_AUTO_TEST_CASE (RecordThreadResult_test)
{
  BOOST_CHECK(ThreadController::vec_final_merged_text_.size() == 0);
  BOOST_CHECK(ThreadController::n_premature_column_count_ == 0);
  BOOST_CHECK(ThreadController::b_premature_flag_ == false);
  
  vector<string> vec_text;
  int n_count = 5;
  bool b_flag = true;
  ThreadController::RecordThreadResult(vec_text, n_count, b_flag);
  
  BOOST_CHECK(ThreadController::vec_final_merged_text_.size() == 0);
  BOOST_CHECK(ThreadController::n_premature_column_count_ == n_count);
  BOOST_CHECK(ThreadController::b_premature_flag_ == b_flag);
  
  vector<string> vec_text1 = {"good day", "better results", "best version"};
  ThreadController::RecordThreadResult(vec_text1, 0, false);
  
  BOOST_CHECK(ThreadController::vec_final_merged_text_ == vec_text1);
  BOOST_CHECK(ThreadController::n_premature_column_count_ == 0);
  BOOST_CHECK(ThreadController::b_premature_flag_ == false);

  BOOST_CHECK_THROW(ThreadController::RecordThreadResult(vec_text1, -1, false), exception); 

  // vec_final_merged_text_ is not empty
  ThreadController::RecordThreadResult(vec_text, 2, true); 
  BOOST_CHECK(ThreadController::vec_final_merged_text_.size() == 0);

  // Re-set the static variable so as not to impact the rest tests.
  ThreadStatusDataReset();
}

BOOST_AUTO_TEST_CASE (UpdateThreadStatus_test)
{
  BOOST_CHECK(ThreadController::thread_status_ == NOTSTART);

  ThreadController::UpdateThreadStatus(PARTICIAL);
  BOOST_CHECK(ThreadController::thread_status_ == PARTICIAL);

  ThreadController::UpdateThreadStatus(SUCCESS);
  BOOST_CHECK(ThreadController::thread_status_ == SUCCESS);

  BOOST_CHECK_THROW(ThreadController::UpdateThreadStatus(NOTSTART), exception); 
}

BOOST_AUTO_TEST_CASE (DoTextUnshredderInThread_test)
{
  // Test exceptions from thread
  TextUnshredder test_unshred;
  ThreadController::DoTextUnshredderInThread(test_unshred);
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 1);
  
  // Test exceptions from thread
  TextUnshredder test_unshred0;
  test_unshred0.vec_text_columns_ = {{"123","45678"},{" ","abcdefg"}};
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 1);

  // This data file can be partially restored - reason why it cannot be fully restored during UT
  // is that vec_selected_columns_ are fixed, which caused each thread generate the same result.

  TextUnshredder test_unshred1;
  test_unshred1.GetInput("test/UTinput/test_shredded_text.ascii");

  // Re-set the static variable so as not to impact the rest tests.
  ThreadStatusDataReset();

  ThreadController::DoTextUnshredderInThread(test_unshred1);

  BOOST_CHECK(ThreadController::vec_final_merged_text_.size() == 8);
  BOOST_CHECK(ThreadController::vec_final_merged_text_.begin()->size() == 38);

  BOOST_CHECK(ThreadController::thread_status_ != NOTSTART);
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 0);
 
  TextUnshredder test_unshred2;

  // This data file can be successfully restored 
  test_unshred2.GetInput("test/UTinput/test_shredded_text0.ascii");

  // Re-set the static variable so as not to impact the rest tests.
  ThreadStatusDataReset();

  int n_number = 10; 
  thread test_thread[n_number]; 

  for (int i = 0; i < n_number; ++i) 
    test_thread[i] = thread(ThreadController::DoTextUnshredderInThread, test_unshred2);

  for (int i = 0; i < n_number; ++i)
    test_thread[i].join();

  BOOST_CHECK(ThreadController::vec_final_merged_text_.size() == 8);
  BOOST_CHECK(ThreadController::vec_final_merged_text_.begin()->size() == 38);

  BOOST_CHECK(ThreadController::thread_status_ != NOTSTART);
  BOOST_CHECK(ThreadController::n_thread_abnormals_ == 0);

  // Re-set the static variable so as not to impact the rest tests.
  ThreadStatusDataReset();
}

BOOST_AUTO_TEST_SUITE_END ()

// Test main logic for main() function
BOOST_AUTO_TEST_SUITE (UTmain_test);

BOOST_AUTO_TEST_CASE (UTmain_test)
{
  // Partially restored
  BOOST_CHECK(UTmain("test/UTinput/test_shredded_text.ascii", "test/UTinput/test_output.ascii", false) == 1);

  // Successfully restored
  BOOST_CHECK(UTmain("test/UTinput/test_shredded_text0.ascii", "test/UTinput/test_output0.ascii", false) == 0);

  // Threads throw exception
  BOOST_CHECK(UTmain("test/UTinput/test_shredded_text0.ascii", "test/UTinput/test_output0.ascii", true) == 2);

  // main() function throw exception as data input file not exist
  BOOST_CHECK(UTmain("test/UTinput/test_input.ascii", "test/UTinput/test_output1.ascii", false) == 2);

  system("rm -rf test/UTinput/test_output*.ascii");
}

BOOST_AUTO_TEST_SUITE_END ()
