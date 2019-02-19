// Unit Testing by using BOOST
#define BOOST_TEST_MODULE main_shredder_test
#include <boost/test/included/unit_test.hpp>
#include "../common_class.h"
#include "../shredder.h"
#include "../singleton_random.h"

using namespace std;

// Include Fixtures
#include "fixture.in"

// Class SingletonRandom
// Disable this test_suite by default as the test of SingletonRandom has conflicts with the rest 
// testings which may use SingletonRandom class.
// It can be invoked by option --run_test=SingletonRandom_test seperately with the rest tests.
BOOST_AUTO_TEST_SUITE (SingletonRandom_test, * boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE (GetInstance_test)
{
 
  // Exception if not setting s_max_val_ 
  BOOST_CHECK_THROW(SingletonRandom::GetInstance(), exception);   

  SingletonRandom::s_max_val_ = 50;
  SingletonRandom & ran0 = SingletonRandom::GetInstance();
  SingletonRandom & ran1 = SingletonRandom::GetInstance();

  BOOST_CHECK(ran0.s_instance_numbers_ == 1);
  BOOST_CHECK(ran1.s_instance_numbers_ == 1);

}


BOOST_AUTO_TEST_CASE (GenerateRandom_test)
{
  unsigned val[20];
  SingletonRandom::s_max_val_ = 50;
  SingletonRandom & ran = SingletonRandom::GetInstance();

  for (int i = 0; i<20; ++i)
  {
      val[i] = ran.GenerateRandom();
      BOOST_CHECK_LE(val[i],50);
      BOOST_CHECK_GE(val[i],0);

      if (i > 3)
      {
        BOOST_WARN_NE(val[i],val[i-1]);
        BOOST_WARN_NE(val[i],val[i-2]);
        BOOST_WARN_NE(val[i],val[i-3]);
        BOOST_WARN_NE(val[i],val[i-4]);
      }
  }
}

BOOST_AUTO_TEST_SUITE_END( )

// Class TextFileOperation
BOOST_FIXTURE_TEST_SUITE(TextFileOperation_test, Fixture_file);

BOOST_AUTO_TEST_CASE (ReadText_test)
{
  vector<string> lines_t;

  TextFileOperation::ReadText("test_input.ascii", lines_t);
  // ReadText() will append blank charactors to ensure all lines have the same number of charactors
  BOOST_CHECK(lines_t == Fix_lines);   
  lines_t.clear();

  // Test file with blanks at the end of lines.
  TextFileOperation::ReadText("test_input1.ascii", lines_t);
  BOOST_CHECK(lines_t == Fix_lines);   
  lines_t.clear();
  
  // File not exist, throw an exception  
  BOOST_CHECK_THROW(TextFileOperation::ReadText("test_input99.ascii", lines_t),exception);
  lines_t.clear();

  // Test empty file 
  TextFileOperation::ReadText("test_input00.ascii", lines_t);

  BOOST_CHECK(lines_t.size() == 0);   

  // Test invalid input 
  BOOST_CHECK_THROW(TextFileOperation::ReadText("", lines_t), exception);   

}

BOOST_AUTO_TEST_CASE (WriteText_test)
{

  // Test invalid input 
  BOOST_CHECK_THROW(TextFileOperation::WriteText("", Fix_orig_lines), exception);   

  TextFileOperation::WriteText("test_output_t.ascii", Fix_orig_lines);
  // When cmp return non-zero value, Boost will detect it and quit with fatal error.
  BOOST_CHECK(system ("cmp -l test_output_t.ascii test_output.ascii >/dev/null 2>&1") == 0);   

  TextFileOperation::WriteText("test_output_t1.ascii", Fix_lines);
  BOOST_CHECK(system ("cmp -l test_output_t1.ascii test_output1.ascii >/dev/null 2>&1") == 0);   
  
  // When file exists, should overwite the file.
  TextFileOperation::WriteText("test_output_t1.ascii", Fix_lines);
  BOOST_CHECK(system ("cmp -l test_output_t1.ascii test_output1.ascii >/dev/null 2>&1") == 0);   

  system("rm -rf test_output_t*.ascii");

}

BOOST_AUTO_TEST_SUITE_END( )

// Class TextStripOperation 
BOOST_FIXTURE_TEST_SUITE(TextStripOperation_test, Fixture_input);

BOOST_AUTO_TEST_CASE (Disorder_test)
{
  vector<vector<string>> input1 = Fix_input;
  TextStripOperation::Disorder(input1);

//  BOOST_CHECK_NE(input1,input);
//  BOOST_CHECK(input1 != input);
  BOOST_WARN(input1 != Fix_input);

  int found = 0; 
 
  for (int i=0; i<3; ++i)
  {
     for (int k=0; k<3; ++k)
     {
       if (Fix_input[i][0] == input1[k][0])
       {
          for (int j=1; j<4; ++j)
          {
            BOOST_CHECK(Fix_input[i][j] == input1[k][j]);
          }
          ++found;
          break;
       }
     } 
  }

  BOOST_CHECK(found == 3);

  vector<vector<string>> input2;
  BOOST_CHECK_THROW(TextStripOperation::Disorder(input2), exception);   
  
}


BOOST_AUTO_TEST_CASE (Transpose_test)
{
  vector<vector<string>> input1;

  TextStripOperation::Transpose(Fix_input, input1);
  BOOST_CHECK(input1 == Fix_input_r);

  vector<vector<string>> input2 = Fix_input;
  TextStripOperation::Transpose(Fix_input, input2);
  BOOST_CHECK(input2 == Fix_input_r);

  // For empty input
  vector<vector<string>> input3,input4;

  BOOST_CHECK_THROW(TextStripOperation::Transpose(input3, input4), exception);   

}

BOOST_AUTO_TEST_SUITE_END( )

// Class TextShredder
BOOST_AUTO_TEST_SUITE (TextShredder_test)

BOOST_AUTO_TEST_CASE (constructor_test)
{
  TextShredder shred0;

  BOOST_CHECK(shred0.n_strip_width_ == 2);
  BOOST_CHECK(shred0.str_in_filename_ == "full_text0.ascii");
  BOOST_CHECK(shred0.str_out_filename_ == "shredded_text0.ascii");

  TextShredder shred1(3,"orign_input.ascii","input3.ascii");

  BOOST_CHECK(shred1.n_strip_width_ == 3);
  BOOST_CHECK(shred1.str_in_filename_ == "orign_input.ascii");
  BOOST_CHECK(shred1.str_out_filename_ == "input3.ascii");

  TextShredder shred2(2,"","");
  BOOST_CHECK(shred2.n_strip_width_ == 2);
  BOOST_CHECK(shred2.str_in_filename_ == "");
  BOOST_CHECK(shred2.str_out_filename_ == "");

}

BOOST_FIXTURE_TEST_CASE(DoTextShredder_test, Fixture_source_data)
{
  // n_strip_width_ is 2 by default
  TextShredder shred0;

  // Not populate vec_str_source_data_ and call DoTextShredder directly
  BOOST_CHECK_THROW(shred0.DoTextShredder(),exception);

  // Populate vec_str_source_data_
  auto iter = Fix_lines.begin();
  while (iter != Fix_lines.end())
  {
    shred0.vec_str_source_data_.push_back(*iter);
    ++iter;
  }

  shred0.DoTextShredder();

  // Verify vec_str_shredded_text_  
  BOOST_CHECK(shred0.vec_str_shredded_text_ == Fix_shredded_text_wid2);

  // Initialize with n_strip_width_ = 3
  TextShredder shred1(3,"x","y");
 
  // Populate vec_str_source_data_
  auto iter1 = Fix_lines.begin();
  while (iter1 != Fix_lines.end())
  {
    shred1.vec_str_source_data_.push_back(*iter1);
    ++iter1;
  }

  shred1.DoTextShredder();

  //verify vec_str_shredded_text_  
  BOOST_CHECK(shred1.vec_str_shredded_text_ == Fix_shredded_text_wid3);
}

BOOST_FIXTURE_TEST_CASE(GetInput_test,Fixture_file)
{
  TextShredder shred0(2,"test_input1.ascii","");
  shred0.GetInput();

  BOOST_CHECK(shred0.vec_str_source_data_ == Fix_lines);

  TextShredder shred1(2,"NotExist","");
  BOOST_CHECK_THROW(shred1.GetInput(),exception);

  TextShredder shred2(2,"","xy.ascii");
  BOOST_CHECK_THROW(shred2.GetInput(),exception);
}

BOOST_FIXTURE_TEST_CASE(CreateOutput_test,Fixture_data_file)
{

  TextShredder shred0(2,"test_input.out","");
  BOOST_CHECK_THROW(shred0.CreateOutput(),exception);

  TextShredder shred1(2,"xy.ascii","test_output_t2.ascii");

  // When vec_str_shredded_text_ containis no data
  BOOST_CHECK_THROW(shred1.CreateOutput(),exception);
  
  // Populate vec_str_shredded_text_
  auto iter = Fix_shredded_text_wid2.begin();
  while (iter != Fix_shredded_text_wid2.end())
  {
    shred1.vec_str_shredded_text_.push_back(*iter);
    ++iter;
  }

  shred1.CreateOutput();

  // When cmp return non-zero value, Boost will detect it and quit with fatal error.
  BOOST_CHECK(system ("cmp -l test_output2.ascii test_output_t2.ascii >/dev/null 2>&1") == 0);   

  TextShredder shred2(3,"xy.ascii","test_output_t3.ascii");

  // Populate vec_str_shredded_text_
  auto iter1 = Fix_shredded_text_wid3.begin();
  while (iter1 != Fix_shredded_text_wid3.end())
  {
    shred2.vec_str_shredded_text_.push_back(*iter1);
    ++iter1;
  }

  shred2.CreateOutput();

  // When cmp return non-zero value, Boost will detect it and quit with fatal error.
  BOOST_CHECK(system ("cmp -l test_output3.ascii test_output_t3.ascii >/dev/null 2>&1") == 0);   

  system("rm -rf test_output_t*.ascii");
}

BOOST_AUTO_TEST_SUITE_END( )

// Mainly focus on  parameter interface test for main() function.
// Funcationality test will be fully covered by system level test.
BOOST_FIXTURE_TEST_SUITE(UTmain_test, Fixture_file);

BOOST_AUTO_TEST_CASE (UTmain_test)
{
  BOOST_CHECK(UTmain(2,"test_input.ascii", "test_output_t1.ascii") == 0);

  BOOST_CHECK(UTmain(100,"test_input.ascii", "test_output_t2.ascii") == 0);

  BOOST_CHECK(UTmain(0,"test_input.ascii", "test_output_t.ascii") == 1);

  BOOST_CHECK(UTmain(-2,"test_input.ascii", "test_output_t.ascii") == 1);

  BOOST_CHECK(UTmain(2,"", "test_output_t.ascii") == 1);

  BOOST_CHECK(UTmain(3,"test_input.ascii", "") == 1);

  system("rm -rf test_output_t*.ascii");
}

BOOST_AUTO_TEST_SUITE_END( )
