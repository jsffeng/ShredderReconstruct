// Unit Testing by using BOOST
#define BOOST_TEST_MODULE main_unshredder_test
#include <boost/test/included/unit_test.hpp>
#include "../common_classes.h"
#include "../text_unshredder.h"
#include "../singleton_diction.h"
#include "../singleton_random.h"
#include "../column_match_manager.h"

using namespace std;

// Include Fixtures
#include "fixture.in"

// Class StringWordOperation
BOOST_AUTO_TEST_SUITE (StringWordOperation_test);

BOOST_AUTO_TEST_CASE (RemoveWordSuffix)
{
  vector<string> sample = {"goods", " goods", "goods ", "uses", "being", "he's", "added", "us", "ding", "bed", "ed"};
  vector<string> sample_suffix_removed = {"good"," good", "goods ","us", "be", "he'", "add", "us", "ding", "bed", "ed"};
  
  for (int i = 0; i < sample.size(); ++i)
     StringWordOperation::RemoveWordSuffix(sample[i]); 

  BOOST_CHECK(sample == sample_suffix_removed);

  vector<string> sample1 = {"goods", "good", "bed", "goods ", " goods"};
  BOOST_CHECK(true == StringWordOperation::RemoveWordSuffix(sample1[0]));
  BOOST_CHECK(false == StringWordOperation::RemoveWordSuffix(sample1[1]));
  BOOST_CHECK(false == StringWordOperation::RemoveWordSuffix(sample1[2]));
  BOOST_CHECK(false == StringWordOperation::RemoveWordSuffix(sample1[3]));
  BOOST_CHECK(true == StringWordOperation::RemoveWordSuffix(sample1[4]));
  
  string sample2;
  BOOST_CHECK_THROW(StringWordOperation::RemoveWordSuffix(sample2), exception);   

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

  vector<string> vec_str_key_wid2 = {"a1a2a3a4a5", \
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
    StringWordOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid6);

  width = 2;
  for (int i = 0; i < vec_str_line.size(); ++i)
    StringWordOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid2);

  width = 8;
  for (int i = 0; i < vec_str_line.size(); ++i)
    StringWordOperation::FindLookupWordLeft(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid8);
  
  string str_key_t;
  StringWordOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);

  // str_key_t is note empty
  StringWordOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);
  str_key_t.clear();

  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, 0), exception); 
  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordLeft(vec_str_line[0], str_key_t, -1), exception); 
  
  string str_line;
  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordLeft(str_line, str_key_t, 8), exception);   
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

  vector<string> vec_str_key_wid2 = {"a5a6a7a8a9", \
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
    StringWordOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid6);

  width = 2;
  for (int i = 0; i < vec_str_line.size(); ++i)
    StringWordOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid2);

  width = 8;
  for (int i = 0; i < vec_str_line.size(); ++i)
    StringWordOperation::FindLookupWordRight(vec_str_line[i], vec_str_key_t[i], width);
  BOOST_CHECK(vec_str_key_t == vec_str_key_wid8);
  
  string str_key_t;
  StringWordOperation::FindLookupWordRight(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);

  // str_key_t is note empty
  StringWordOperation::FindLookupWordRight(vec_str_line[0], str_key_t, width);
  BOOST_CHECK(str_key_t == vec_str_key_wid8[0]);
  str_key_t.clear();

  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordRight(vec_str_line[0], str_key_t, 0), exception); 
  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordRight(vec_str_line[0], str_key_t, -1), exception); 
  
  string str_line;
  BOOST_CHECK_THROW(StringWordOperation::FindLookupWordRight(str_line, str_key_t, 8), exception);   
}

BOOST_AUTO_TEST_SUITE_END ()

// Class SingletonDiction
BOOST_AUTO_TEST_SUITE (SingletonDiction_test, * boost::unit_test::disabled())

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
  BOOST_CHECK(17640 == dict.uset_dictionary_.size());
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
  ColumnMatchManager column_match_mgr0;
  ColumnMatchManager column_match_mgr1(Fix_columns_wid2, Fix_columnX_wid2);
  BOOST_CHECK (Fix_columns_wid2 == column_match_mgr1.vec_text_columns_);
  BOOST_CHECK (Fix_columnX_wid2 == column_match_mgr1.vec_new_column_);

  ColumnMatchManager column_match_mgr2 = column_match_mgr1;

  ColumnMatchManager column_match_mgr3 = ColumnMatchManager(Fix_columns_wid2, Fix_columnX_wid2);
  BOOST_CHECK (Fix_columns_wid2 == column_match_mgr3.vec_text_columns_);
  BOOST_CHECK (Fix_columnX_wid2 == column_match_mgr3.vec_new_column_);
}

BOOST_AUTO_TEST_CASE (BuildLookupKey_test)
{
  vector<string> vec_keys;
  ColumnMatchManager column_match_mgr1(Fix_columns_wid2, Fix_columnX_wid2);

  vector<string> vec_keys_right1 = {"good", "will", "wil"};
  column_match_mgr1.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right1 == vec_keys);

  vector<string> vec_keys_left1 = {"0", "0", "iluse"};
  column_match_mgr1.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left1 == vec_keys);

  ColumnMatchManager column_match_mgr2(Fix_columns_wid3, Fix_columnY_wid3);

  vector<string> vec_keys_right2 = {"stas", "borr", "beaus"};
  vec_keys.clear();
  column_match_mgr2.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right2 == vec_keys);

  vector<string> vec_keys_left2 = {"0", "orrow", "ause"};
  vec_keys.clear();
  column_match_mgr2.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left2 == vec_keys);

  // vec_keys is not empty
  column_match_mgr2.BuildLookupKey(vec_keys, LEFT);
  BOOST_CHECK (vec_keys_left2 == vec_keys);

  ColumnMatchManager column_match_mgr0;
  BOOST_CHECK_THROW(column_match_mgr0.BuildLookupKey(vec_keys, LEFT), exception);   

  // Verify special charactors correctly converted into blank delimiter
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

  // Verify the handling on 's, 'd, 't
  vector<string> column_spec_s_d_t1 =  {"abcdefg", "abc'd,g","abc's fg"};
  ColumnMatchManager column_match_mgr5(columns_special, column_spec_s_d_t1);

  vector<string> vec_keys_right5 = {"0", "nowayfabc", "okabc"};
  vec_keys.clear();
  column_match_mgr5.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right5 == vec_keys);

  vector<string> column_spec_s_d_t2 =  {"abcdefg", "abc fg","'s abcd"};
  ColumnMatchManager column_match_mgr6(columns_special, column_spec_s_d_t2);

  vector<string> vec_keys_right6 = {"0", "nowayfabc", "0"};
  vec_keys.clear();
  column_match_mgr6.BuildLookupKey(vec_keys, RIGHT);
  BOOST_CHECK (vec_keys_right6 == vec_keys);

  // Verify the handling on capital letters or non alphabetic charactors
  vector<string> column_spec_cap =  {"abcdefg", "abC,efg","abc'h fg"};
  ColumnMatchManager column_match_mgr7(columns_special, column_spec_cap);

  vector<string> vec_keys_right7 = {"0", "0", "0"};
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

BOOST_AUTO_TEST_SUITE_END ()

/*
// Class  xxxx
BOOST_AUTO_TEST_SUITE ();

BOOST_AUTO_TEST_CASE ()
{

}

BOOST_AUTO_TEST_CASE ()
{

}

BOOST_AUTO_TEST_SUITE_END ()

*/
