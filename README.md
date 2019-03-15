This package contains two tools - shredder and unshredder
----------------------------------------------------------  
- shredder can shred the text page into text strips in random order. 
- unshredder can re-constuct the original text page with text strips. 
  
>>shredder: full_text.ascii (input file) ==> shredded_text.ascii (output file)  
>>unshredder: shredded_text.ascii (input file) ==> restored_text.ascii (output file)  
    
>>If successfully restored, restored_text.ascii should have the same content with full_text.ascii  

Development Environment: 
---------------------------------------------  
Linux version: ubuntu 16.04.01  

Dependent packages/tools:
----------------  
- g++ - supporting C++ 11 standard (with option -std=gnu++11).  
- Boost - version: 1.68
- lcov - support test coverage data collection and report generation.  

Sub-directories:
----------------  
- design - containing high level designs.  
- dict - containing wordlist used by product unshredder.  
- test - containing unit test code and acceptance test script.  
- ut_coverage - containing unit test code coverage result.  
  
Programme source code statistics:
----------------------------------  

*ubuntu->cloc . --exclude-dir=.git,test,ut_coverage,design,dict    
      21 text files.  
      21 unique files.  
     147 files ignored.*  
    
*http://cloc.sourceforge.net v 1.60  T=0.10 s (197.3 files/s, 17440.1 lines/s)  
\-------------------------------------------------------------------------------  
**Language:                  files:          blank:        comment:           code**  
\-------------------------------------------------------------------------------  
**C++:**                          10:            229:            128:            902  
**C/C++ Header:**                  9:             87:             62:            232  
**make:**                          1:             35:              3:             90  
\-------------------------------------------------------------------------------  
**SUM:**                          20:            351:            193:           1224  
\-------------------------------------------------------------------------------*  
  
UT/Acceptance test code statistics:  
----------------------------------  
*ubuntu->cloc test  
      17 text files.  
      17 unique files.  
      14 files ignored.*  
   
*http://cloc.sourceforge.net v 1.60  T=0.07 s (44.8 files/s, 21621.9 lines/s)  
\-------------------------------------------------------------------------------  
**Language:                     files:          blank:        comment:           code**  
\-------------------------------------------------------------------------------  
**C++:**                              2:            341:            132:            826  
**Bourne Shell:**                     1:             34:              2:            112  
\-------------------------------------------------------------------------------  
**SUM:**                             3:            375:            134:            938  
\-------------------------------------------------------------------------------*  
  
make commands:  
--------------  
Run make commmand in top directory.  
  
make bld: Build the final product  
make ut: Build the UT test product  
make run_ut: Execute UT tests  
make acceptance_test: Execute acceptance test (system test)   
make clean: Clean all generated files (excluding ut test coverage data if exist) except source code   
make GCOV=1 ut_cover: Generate UT test coverage information  
make GCOV=1 clean: Clean all generated files except source code and ut_coverage directory  
  
Unit testing:  
------------------------------   
Unit testing done with Boost Unit test framework. Here is the result:
  
*ubuntu->make run_ut  
./test/UTshredder --run_test=SingletonRandom_test --log_level=warning;echo  
Running 2 test cases...  
test/UTshredder.cpp(49): warning: in "SingletonRandom_test/GenerateRandom_test": condition val[i] != val[i-1] is not satisfied [4 == 4]*      
*** *No errors detected*  
  
*./test/UTshredder --log_level=warning;echo  
Running 11 test cases...*  
  
*** *No errors detected*  
  
*./test/UTunshredder --run_test=TESTSingleton_test --log_level=warning;echo  
Running 1 test case...*  
  
*** *No errors detected*  
  
*./test/UTunshredder --log_level=warning;echo  
Running 25 test cases...*  
  
*** *No errors detected*  
  
    
Unit testing coverage rate:  
--------------------------   
http://htmlpreview.github.io/?https://github.com/jsffeng/Shredder_Reconstruct/blob/master/ut_coverage/index.html  

Acceptance test (System test):    
-----------------------------    

Use 6 text samples (under directory test/STinput) to verify the programme.  
**Note**: Most of above samples got from BBC news for different catagory such as Tech, Secience, Sports, Business, etc.  
Their text row numbers ranged from 8 to 17, and characators in rows ranged from 37 to 48.   
  
*ubuntu->make acceptance_test  
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
TESTCASE30:PASS*  
  
\--------------------------------  
*30 Tests: 30 Successes 0 Failures  
OK  
NOTE: Please find test details under direcotry ./test/SToutput.*  
  
**Example** (i.e. copying ./test/STinput/sampleB to full_text.ascii, run shredder to get the following):  
\=============================================    
*|op|t |r-|ye|sa|  | t|gh| n|is|g |o |ei|Na|es|ea| h|in|br|ar|th|  
| c|he|wi|  |p |  |ab| t|en|n |to|il|th|ga|ap|d |in|y |it|  | a|  
|uc| s|er|ys|tr|ms|on|al|co|w |o | o|ci|in|ti|mm|od|tw|f |te|ne|  
|Dr|ei|ic|ng|th|  |on| b| v|er|no| a|le|- |ag|eh|e | a|nd|  |th|  
|pe|g.| B|  |ve|  |by|in|ia| g|pa| a|oe|de|d |nt|lo|os|er|  |ce|  
|rs|go|he|n |e |  |cr|ra|of|t |li|ew| D|Th|t | t|fi| f|ed|  |gh|  
|oc|ug| a|h |ul|  |r |ho|ul| J|n |as|lt|co|cu|y,|d |oo| s|  |as|  
|ar| i|o |nt|is|  |t |ip|ly|ke|s |da|sl|th|ge| t| t| i|te|o |li|  
|mm|gh| t| t|e |  | a|ou|wo|s |ne|s |hr|th|er|rk|su|gi|en|he|er|  
|li|  |  |  |st|  |t |  |  |  |is|an|  |po|gh|  |-f|ys|al|  |. |  
|do| E| 0|ST|la|  | o|45|bo| a|d |cc|8:|Sp|wn|ut|sh|re|ur|  |at|  
| G|rc|GO|he|3:|  |).|ea|le|al|t,| A| S|(1|MT|d |45|oa| b|r,| c|  
|it|. |ap|Th|s |e |g |le|he| t|ov|to|su|wa|in| c|wa|ec| r|er|er|  
|he|th|ol|e |re|  |s |s |on| c|si|at| a|we|er|tr| c|is| m|  |on|  
|e |  |ic|  |ps|  |nd|  |la|At|th|ed|. |ca|la|nt|ul|n | i|  |e |*  
  
Restored result (i.e. run unshredder for above):  
\==========================   
*Nasa hopes to bring this near-eight year  
gap in capability to an end with the  
introduction of two new commercial systems  
\- the Dragon and another vehicle being  
developed by aerospace giant Boeing.  
The first crewed flight of the Dragon  
could occur as soon as July, although  
this target date is likely to slip into  
the summer as engineers work through the  
post-flight analysis.  
Splashdown occurred at about 08:45 EST  
(13:45 GMT). A boat, called GO Searcher,  
was waiting to recover the capsule. There  
were cheers at mission control as the  
capsule landed in the Atlantic.*  
  
