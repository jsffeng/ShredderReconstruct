#define BOOST_TEST_MODULE main_shredder_test
#include <boost/test/included/unit_test.hpp>
#include "../shredder.h"
#include "../singleton.h"
#include<iostream>
#include<fstream>

using namespace std;

// Fixtures Start
struct Fixture_input
{
  
  vector<vector<string>> input;
  vector<vector<string>> input_r;

  Fixture_input() 
  {
    vector<vector<string>> myinput = {{"ab","cd","ef","gh"}, \
                                      {"ij","kl","mn","op"}, \
                                      {"qr","st","uv","wx"}};

    vector<vector<string>> myinput_r = {{"ab", "ij", "qr"},  \
                                        {"cd","kl", "st" },  \
                                        {"ef", "mn", "uv"},  \
                                        {"gh", "op", "wx"}};

    auto iter=myinput.begin();
    while (iter != myinput.end())
    {
      input.push_back(*iter);
      iter++;
    }

    auto iter1=myinput_r.begin();
    while (iter1 != myinput_r.end())
    {
      input_r.push_back(*iter1);
      iter1++;
    }
  }
  
  ~Fixture_input(){}
};

struct file_gen
{
  static void  create_file (const string file_nm, const vector<string> text_lines)
  {
    ofstream file;
  
    file.open (file_nm);
    if(!file)
    {
      throw runtime_error("file cannot open!");
    }
 
    if (text_lines.size() != 0) 
    {
      auto iter=text_lines.begin();
      while (iter != text_lines.end())
      {
  
        file << *iter <<endl;
        iter++;
      }
    }
    file.close();
  }
};

struct Fixture_file
{
  vector<string> orig_lines;
  vector<string> lines;

  Fixture_file() 
  {
    orig_lines.push_back("This is a good start.");
    orig_lines.push_back("Tomorrow will be better");
    orig_lines.push_back("because it will be successful...");

    lines.push_back("This is a good start.           ");
    lines.push_back("Tomorrow will be better         ");
    lines.push_back("because it will be successful...");

    file_gen::create_file("test_input.ascii",orig_lines);
    file_gen::create_file("test_input1.ascii",lines);
   
    vector<string> empty_lines;
    file_gen::create_file("test_input00.ascii",empty_lines);

    file_gen::create_file("test_output.ascii",orig_lines);
    file_gen::create_file("test_output1.ascii",lines);
  }
 
  ~Fixture_file()
  {
    if (0 != remove("test_input.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
    if (0 != remove("test_input1.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
    if (0 != remove("test_input00.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
    if (0 != remove("test_output.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
    if (0 != remove("test_output1.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
  }

};

struct Fixture_source_data
{
  vector<string> lines;
  vector<vector<string>> shredded_text_wid2;
  vector<vector<string>> shredded_text_wid3;


  Fixture_source_data() 
  {
    lines.push_back("This is a good start   ");
    lines.push_back("Tomorrow will be bett  ");
    lines.push_back("because it will be succ");

    vector<vector<string>> myshredded_wid2 = {{"Th","is"," i","s ","a ","go","od"," s","ta","rt","  ","  "}, \
                                          {"To","mo","rr","ow"," w","il","l ","be"," b","et","t ","  "}, \
                                          {"be","ca","us","e ","it"," w","il","l ","be"," s","uc","c "}};


    vector<vector<string>> myshredded_wid3 = {{"Thi","s i","s a"," go","od ","sta","rt ","   "}, \
                                          {"Tom","orr","ow ","wil","l b","e b","ett","   "}, \
                                          {"bec","aus","e i","t w","ill"," be"," su","cc "}};

    auto iter=myshredded_wid2.begin();
    while (iter != myshredded_wid2.end())
    {
      shredded_text_wid2.push_back(*iter);
      iter++;
    }

    auto iter1=myshredded_wid3.begin();
    while (iter1 != myshredded_wid3.end())
    {
      shredded_text_wid3.push_back(*iter1);
      iter1++;
    }

  }

  ~Fixture_source_data() {}
};

struct Fixture_data_file :  Fixture_source_data
{
  vector<string> format_lines_wid2;
  vector<string> format_lines_wid3;

  Fixture_data_file()
  {
    format_lines_wid2.push_back("|Th|is| i|s |a |go|od| s|ta|rt|  |  |");
    format_lines_wid2.push_back("|To|mo|rr|ow| w|il|l |be| b|et|t |  |");
    format_lines_wid2.push_back("|be|ca|us|e |it| w|il|l |be| s|uc|c |");

    format_lines_wid3.push_back("|Thi|s i|s a| go|od |sta|rt |   |");
    format_lines_wid3.push_back("|Tom|orr|ow |wil|l b|e b|ett|   |");
    format_lines_wid3.push_back("|bec|aus|e i|t w|ill| be| su|cc |");

    file_gen::create_file("test_output2.ascii",format_lines_wid2);
    file_gen::create_file("test_output3.ascii",format_lines_wid3);

  }

  ~Fixture_data_file()
  { 
    if (0 != remove("test_output2.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
    if (0 != remove("test_output3.ascii"))
    {
      throw runtime_error("file not exist or cannot be removed!");
    }
  }
};

// Fixtures End


// Class Singleton_rand
BOOST_AUTO_TEST_SUITE (Singleton_rand_test, * boost::unit_test::disabled())
//BOOST_AUTO_TEST_SUITE (Singleton_rand_test)

BOOST_AUTO_TEST_CASE (getInstance_test)
{
 
  // Exception if not setting max_val 
  BOOST_CHECK_THROW(Singleton_rand::getInstance(), exception);   

  Singleton_rand::max_val = 50;
  Singleton_rand& ran0 = Singleton_rand::getInstance();
  Singleton_rand& ran1 = Singleton_rand::getInstance();

  BOOST_CHECK(ran0.instance_num == 1);
  BOOST_CHECK(ran1.instance_num == 1);

}


BOOST_AUTO_TEST_CASE (rand_generator_test)
{
  unsigned val[20];
  Singleton_rand::max_val = 50;
  Singleton_rand& ran = Singleton_rand::getInstance();

  for (int i=0; i<20; i++)
  {
      val[i] = ran.rand_generator();
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

// Class file_operation
BOOST_FIXTURE_TEST_SUITE(file_operation_test, Fixture_file);

BOOST_AUTO_TEST_CASE (read_text_test)
{
  vector<string> lines_t;

  file_operation::read_text("test_input.ascii", lines_t);
  // read_text() will append blank lines to ensure every lines have same charactors
  BOOST_CHECK(lines_t == lines);   
  lines_t.clear();

  // Test file with blanks in the end of lines.
  file_operation::read_text("test_input1.ascii", lines_t);
  BOOST_CHECK(lines_t == lines);   
  lines_t.clear();
  
  // File not exist, throw an exception  
  BOOST_CHECK_THROW(file_operation::read_text("test_input99.ascii", lines_t),exception);
  lines_t.clear();

  // Test empty file 
  file_operation::read_text("test_input00.ascii", lines_t);

  BOOST_CHECK(lines_t.size() == 0);   

  // Test invalid input 
  BOOST_CHECK_THROW(file_operation::read_text("", lines_t), exception);   

}

BOOST_AUTO_TEST_CASE (write_text_test)
{

  // Test invalid input 
  BOOST_CHECK_THROW(file_operation::write_text("", orig_lines), exception);   

  file_operation::write_text("test_output_t.ascii", orig_lines);
  // When cmp return non-zero value, Boost will detect it and quit with fatal error.
  BOOST_CHECK(system ("cmp -l test_output_t.ascii test_output.ascii >/dev/null 2>&1") == 0);   

  file_operation::write_text("test_output_t1.ascii", lines);
  BOOST_CHECK(system ("cmp -l test_output_t1.ascii test_output1.ascii >/dev/null 2>&1") == 0);   
  
  // When file exists, should overwite the file.
  file_operation::write_text("test_output_t1.ascii", lines);
  BOOST_CHECK(system ("cmp -l test_output_t1.ascii test_output1.ascii >/dev/null 2>&1") == 0);   

  system("rm -rf test_output_t*.ascii");

}

BOOST_AUTO_TEST_SUITE_END( )

// Class text_strip_operation 
BOOST_FIXTURE_TEST_SUITE(text_strip_operation_test, Fixture_input);

BOOST_AUTO_TEST_CASE (disorder_test)
{
  vector<vector<string>> input1 = input;
  text_strip_operation::disorder(input1);

//  BOOST_CHECK_NE(input1,input);
//  BOOST_CHECK(input1 != input);
  BOOST_WARN(input1 != input);

  int found = 0; 
 
  for (int i=0; i<3; i++)
  {
     for (int k=0; k<3; k++)
     {
       if (input[i][0] == input1[k][0])
       {
          for (int j=1; j<4; j++)
          {
            BOOST_CHECK(input[i][j] == input1[k][j]);
          }
          found++;
          break;
       }
     } 
  }

  BOOST_CHECK(found == 3);

  vector<vector<string>> input2;
  BOOST_CHECK_THROW(text_strip_operation::disorder(input2), exception);   
  
}


BOOST_AUTO_TEST_CASE (transpose_test)
{
  vector<vector<string>> input1;

  text_strip_operation::transpose(input, input1);
  BOOST_CHECK(input1 == input_r);

  vector<vector<string>> input2 = input;
  text_strip_operation::transpose(input, input2);
  BOOST_CHECK(input2 == input_r);

  // For empty input
  vector<vector<string>> input3,input4;

  BOOST_CHECK_THROW(text_strip_operation::transpose(input3, input4), exception);   

}

BOOST_AUTO_TEST_SUITE_END( )

// Class shredder
BOOST_AUTO_TEST_SUITE (shredder_test)

BOOST_AUTO_TEST_CASE (constructor_test)
{
  shredder shred0;

  BOOST_CHECK(shred0.strip_width == 2);
  BOOST_CHECK(shred0.infilename == "full_text0.ascii");
  BOOST_CHECK(shred0.outfilename == "shredded_text0.ascii");

  shredder shred1(3,"orign_input.ascii","input3.ascii");

  BOOST_CHECK(shred1.strip_width == 3);
  BOOST_CHECK(shred1.infilename == "orign_input.ascii");
  BOOST_CHECK(shred1.outfilename == "input3.ascii");

  shredder shred2(2,"","");
  BOOST_CHECK(shred2.strip_width == 2);
  BOOST_CHECK(shred2.infilename == "");
  BOOST_CHECK(shred2.outfilename == "");

}

BOOST_FIXTURE_TEST_CASE(do_shredder_test, Fixture_source_data)
{
  // strip_width is 2 by default
  shredder shred0;

  // Not populate source_data and call do_shredder directly
  BOOST_CHECK_THROW(shred0.do_shredder(),exception);

  // Populate source_data
  auto iter=lines.begin();
  while (iter != lines.end())
  {
    shred0.source_data.push_back(*iter);
    iter++;
  }

  shred0.do_shredder();

  // Verify shredded_text  
  BOOST_CHECK(shred0.shredded_text == shredded_text_wid2);

  // Initialize with strip_width == 3
  shredder shred1(3,"x","y");
 
  // Populate source_data
  auto iter1=lines.begin();
  while (iter1 != lines.end())
  {
    shred1.source_data.push_back(*iter1);
    iter1++;
  }

  shred1.do_shredder();

  //verify shredded_text  
  BOOST_CHECK(shred1.shredded_text == shredded_text_wid3);
}

BOOST_FIXTURE_TEST_CASE(get_input_test,Fixture_file)
{
  shredder shred0(2,"test_input1.ascii","");
  shred0.get_input();

  BOOST_CHECK(shred0.source_data == lines);

  shredder shred1(2,"NotExist","");
  BOOST_CHECK_THROW(shred1.get_input(),exception);

  shredder shred2(2,"","xy.ascii");
  BOOST_CHECK_THROW(shred2.get_input(),exception);
}

BOOST_FIXTURE_TEST_CASE(create_output_test,Fixture_data_file)
{

  shredder shred0(2,"test_input.out","");
  BOOST_CHECK_THROW(shred0.create_output(),exception);

  shredder shred1(2,"xy.ascii","test_output_t2.ascii");

  // When shredded_text containis no data
  BOOST_CHECK_THROW(shred1.create_output(),exception);
  
  // Populate shredded_text
  auto iter=shredded_text_wid2.begin();
  while (iter != shredded_text_wid2.end())
  {
    shred1.shredded_text.push_back(*iter);
    iter++;
  }

  shred1.create_output();

  // When cmp return non-zero value, Boost will detect it and quit with fatal error.
  BOOST_CHECK(system ("cmp -l test_output2.ascii test_output_t2.ascii >/dev/null 2>&1") == 0);   

  shredder shred2(3,"xy.ascii","test_output_t3.ascii");

  // Populate shredded_text
  auto iter1=shredded_text_wid3.begin();
  while (iter1 != shredded_text_wid3.end())
  {
    shred2.shredded_text.push_back(*iter1);
    iter1++;
  }

  shred2.create_output();

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
