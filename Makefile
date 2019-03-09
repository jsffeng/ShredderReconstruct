CC = g++
RM = rm -rf

#GCOV = 1, currently invoked by command line
DBG_ENABLE = 1

SRC_PATH := .
TEST_DIR := ./test

TARGET_SHRED := shredder
TARGET_UNSHRED := unshredder

ALLTARGETS := $(TARGET_SHRED) $(TARGET_UNSHRED)
#ALLTARGETS := $(TARGET_SHRED)

TEST_TARGET_SHRED := $(TEST_DIR)/UT$(TARGET_SHRED)
TEST_TARGET_UNSHRED := $(TEST_DIR)/UT$(TARGET_UNSHRED)

TEST_ALLTARGETS := $(TEST_TARGET_SHRED) $(TEST_TARGET_UNSHRED)

MULTI_THREAD_OBJ_NAME = singleton_random

ALLSRCS := $(wildcard $(SRC_PATH)/*.cpp)
ALLOBJS := $(ALLSRCS:.cpp=.o)
ALLOBJS += $(MULTI_THREAD_OBJ_NAME)_multi.o

OBJS_SHRED := common_classes.o singleton_random.o text_shredder.o shred_main.o
OBJS_UNSHRED := common_classes.o singleton_random_multi.o singleton_diction.o column_match_manager.o column_select_manager.o text_unshredder.o thread_controller.o unshred_main.o


# UT test code
UT_TEST_ALLSRCS := $(wildcard $(TEST_DIR)/*.cpp)
UT_TEST_SRCS_SRHED := $(TEST_TARGET_SHRED).cpp
UT_TEST_SRCS_UNSRHED := $(TEST_TARGET_UNSHRED).cpp

UT_TEST_ALLOBJS := $(UT_TEST_ALLSRCS:.cpp=_utest.o)
UT_TEST_OBJS_SHRED := $(UT_TEST_SRCS_SRHED:.cpp=_utest.o)
UT_TEST_OBJS_UNSHRED := $(UT_TEST_SRCS_UNSRHED:.cpp=_utest.o)

TEST_ALLOBJS := $(ALLOBJS:.o=_test.o)
TEST_OBJS_SHRED := $(OBJS_SHRED:.o=_test.o)
TEST_OBJS_UNSHRED := $(OBJS_UNSHRED:.o=_test.o) 

INCLUDE_PATH += /usr/include
INCLUDE_PATH += /usr/local/include

LIBRARY_PATH += /usr/lib
LIBRARY_PATH += /usr/local/lib

CFLAGS += -std=gnu++11
DUTFLAG := -DUTFLAG
MULTI_THREAD_FLAG := -DMULTI_THREAD_FLAG

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))

# For executables using multi-threads
LIBS += pthread
LIBS += boost_regex
LIBFLAGS += $(foreach lib, $(LIBS), -l$(lib))

ifeq (${DBG_ENABLE}, 1)
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

ifeq (${GCOV}, 1)
        COV_FILE = coverage.info
	GCDA_FILES = $(TEST_ALLOBJS:.o=.gcda)
	GCNO_FILES = $(TEST_ALLOBJS:.o=.gcno)

	DIR_NAME =  $(shell pwd )
	WORK_DIR =  $(shell basename $(DIR_NAME) )
	GCOV_RESULT =  ut_coverage

	GCOV_CFLAGS += -fprofile-arcs
	GCOV_CFLAGS += -ftest-coverage
	GCOV_LDFLAGS += -fprofile-arcs
	GCOV_LDFLAGS += -ftest-coverage
	GCOV_LDFLAGS += -lgcov
endif

.PHONY: bld ut clean

all: bld ut
ut_cover: clean ut run_ut collect_cov

bld: $(ALLTARGETS)
ut:  $(TEST_ALLTARGETS)

$(TARGET_SHRED): $(OBJS_SHRED)
#	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBFLAGS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET_UNSHRED): $(OBJS_UNSHRED)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBFLAGS)

$(TEST_TARGET_SHRED): $(TEST_OBJS_SHRED) $(UT_TEST_OBJS_SHRED) 
#	$(CC) $(CFLAGS) $(LDFLAGS) $(GCOV_LDFLAGS) $^ -o $@ $(LIBFLAGS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(GCOV_LDFLAGS) $^ -o $@ 

$(TEST_TARGET_UNSHRED): $(TEST_OBJS_UNSHRED) $(UT_TEST_OBJS_UNSHRED) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(GCOV_LDFLAGS) $^ -o $@ $(LIBFLAGS)

%.o:%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< 

$(MULTI_THREAD_OBJ_NAME)_multi.o: $(MULTI_THREAD_OBJ_NAME).cpp
	$(CC) $(CFLAGS) $(MULTI_THREAD_FLAG) -o $@ -c $<

%_test.o:%.cpp
	$(CC) $(DUTFLAG) $(CFLAGS) $(GCOV_CFLAGS) -o $@ -c $<

$(MULTI_THREAD_OBJ_NAME)_multi_test.o: $(MULTI_THREAD_OBJ_NAME).cpp
	$(CC) $(DUTFLAG) $(CFLAGS) $(MULTI_THREAD_FLAG) $(GCOV_CFLAGS) -o $@ -c $<

%_utest.o:%.cpp
	$(CC) $(DUTFLAG) $(CFLAGS) -o $@ -c $<

# Need multi-thread support for this objective 
$(TEST_DIR)/UT$(TARGET_UNSHRED)_utest.o:$(TEST_DIR)/UT$(TARGET_UNSHRED).cpp
	$(CC) $(DUTFLAG) $(CFLAGS) $(MULTI_THREAD_FLAG) -o $@ -c $<

run_ut: ut
	$(TEST_TARGET_SHRED) --run_test=SingletonRandom_test --log_level=warning;echo
	$(TEST_TARGET_SHRED) --log_level=warning;echo
	$(TEST_TARGET_UNSHRED) --run_test=TESTSingleton_test --log_level=warning;echo
	$(TEST_TARGET_UNSHRED) --log_level=warning;echo
collect_cov:
	lcov --rc lcov_branch_coverage=1 -d . -b . -c -o $(COV_FILE)
	lcov --rc lcov_branch_coverage=1 -e $(COV_FILE) "*/$(WORK_DIR)*" -o  $(COV_FILE)
	genhtml --rc lcov_branch_coverage=1 -o $(GCOV_RESULT) $(COV_FILE)
clean:
	$(RM) $(ALLOBJS) $(TEST_ALLOBJS) $(UT_TEST_ALLOBJS) $(ALLTARGETS) $(TEST_ALLTARGETS) $(GCDA_FILES) $(GCNO_FILES) $(COV_FILE) 
