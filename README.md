This package contains two tools - shredder and unshredder
-----------------------------------------------------------------  
1. shredder can shred the text page into text strips with random order - completed.  
2. unshredder can re-constuct the original text page with text strips - to be worked on.  

Development Environment: 
---------------------------------------------  
Linux version: ubuntu 16.04.01  
Boost version: 1.68

Dependent packages/tools:
----------------  
1. g++ - supporting C++ 11 standard (with option -std=gnu++11).  
2. lcov - support test coverage data collection and report generation.  

Source list:  
------------  
TBD

make commands:  
--------------  
Run make commmand in current directory.  
  
make bld: Build the final product  
make ut: Build the UT test product  
make run_ut: Execute UT tests  
make clean: Clean all generated files (excluding ut test coverage data if exist) but leave source code  
make GCOV=1 ut_cover: Generate UT test coverage information  
make GCOV=1 clean: Clean all generated files including UT test coverage data but leave source code  
  
Unit testing:  
------------------------------   
Unit testing was done with Boost.


   
Unit testing coverage:
------------------------------   
http://htmlpreview.github.io/?https://github.com/jsffeng/Shredder_Reconstruct/blob/master/ut_coverage/index.html

System testing (Acceptance testing):    
------------------------------------    
TBD

