This package contains two tools - shredder and unshredder
-----------------------------------------------------------------  
1. shredder can shred the text page into text strips with random order. 
2. unshredder can re-constuct the original text page with text strips. 

Development Environment: 
---------------------------------------------  
Linux version: ubuntu 16.04.01  
Boost version: 1.68

Dependent packages/tools:
----------------  
1. g++ - supporting C++ 11 standard (with option -std=gnu++11).  
2. lcov - support test coverage data collection and report generation.  

Sub-directories:
----------------  
design - contains high level designs for both two programmes.
dict - contains wordlist used by program unshredder
test - contains unit test code and acceptance test script.
ut_coverage - contains unit test code coverage result.

Source files statistics:
---------------------  
ubuntu->cloc .
      43 text files.
      42 unique files.
     294 files ignored.

http://cloc.sourceforge.net v 1.60  T=0.17 s (134.1 files/s, 18695.1 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             12            570            260           1728
C/C++ Header                     9             87             62            232
Bourne Shell                     1             30              3            107
make                             1             35              3             90
-------------------------------------------------------------------------------
SUM:                            23            722            328           2157
-------------------------------------------------------------------------------

make commands:  
--------------  
Run make commmand in current directory.  
  
make bld: Build the final product  
make ut: Build the UT test product  
make run_ut: Execute UT tests  
make GCOV=1 clean: Clean all generated files except source code and ut_coverage directory.
make clean: Clean all generated files (excluding ut test coverage data if exist) except source code  
make GCOV=1 ut_cover: Generate UT test coverage information  
make acceptance_test: Execute acceptance test (system test)
  
Unit testing:  
------------------------------   
Unit testing was done with Boost. Here is the result:

ubuntu->make run_ut
./test/UTshredder --run_test=SingletonRandom_test --log_level=warning;echo
Running 2 test cases...
test/UTshredder.cpp(49): warning: in "SingletonRandom_test/GenerateRandom_test": condition val[i] != val[i-1] is not satisfied [4 == 4]

*** No errors detected

./test/UTshredder --log_level=warning;echo
Running 11 test cases...

*** No errors detected

./test/UTunshredder --run_test=TESTSingleton_test --log_level=warning;echo
Running 1 test case...

*** No errors detected

./test/UTunshredder --log_level=warning;echo
Running 25 test cases...

*** No errors detected

   
Unit testing coverage:
------------------------------   
http://htmlpreview.github.io/?https://github.com/jsffeng/Shredder_Reconstruct/blob/master/ut_coverage/index.html

Acceptance test (System testing):    
------------------------------------    

Use 6 text samples (under directory test/STinput) to verify the programme.
Note: Most of above got from BBC news for diferent catagory Tech, Secience, Sports, Business, etc.
row numbers range from 8-17, characators in rows ranged from 37 - 48. 

ubuntu->make acceptance_test
/bin/bash ./test/acceptance_test.sh
TESTCASE01:PASS
TESTCASE02:PASS
TESTCASE03:PASS
TESTCASE04:PASS
TESTCASE05:PASS
TESTCASE06:PASS
TESTCASE07:PASS
TESTCASE08:PASS
TESTCASE09:PASS
TESTCASE10:PASS
TESTCASE11:PASS
TESTCASE12:PASS
TESTCASE13:PASS
TESTCASE14:PASS
TESTCASE15:PASS
TESTCASE16:PASS
TESTCASE17:PASS
TESTCASE18:PASS
TESTCASE19:PASS
TESTCASE20:PASS
TESTCASE21:PASS
TESTCASE22:PASS
TESTCASE23:PASS
TESTCASE24:PASS
TESTCASE25:PASS
TESTCASE26:PASS
TESTCASE27:PASS
TESTCASE28:PASS
TESTCASE29:PASS
TESTCASE30:PASS

--------------------------------
30 Tests: 30 Successes 0 Failures
OK
NOTE: Please find test details under direcotry ./test/SToutput.
