CC = g++
RM = rm -rf

#GCOV = 1, currently invoked by command line
DBG_ENABLE = 1

SRC_PATH := .
TEST_DIR := ./test

TARGET := shredder
TEST_TARGET := UTshredder


SRCS := $(wildcard $(SRC_PATH)/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# UT test code
UT_TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
UT_TEST_OBJS := $(UT_TEST_SRCS:.cpp=_ut.o)
TEST_OBJS += $(SRCS:.cpp=_test.o)

INCLUDE_PATH += /usr/include
INCLUDE_PATH += /usr/local/include

LIBRARY_PATH += /usr/lib
LIBRARY_PATH += /usr/local/lib

CFLAGS += -std=gnu++11
DUTFLAG += -DUTFLAG

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))

# if multi-threads,
#LIBS=pthread
#LIBFLAGS += $(foreach lib, $(LIBS), -l$(lib))

ifeq (${DBG_ENABLE}, 1)
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

ifeq (${GCOV}, 1)
        COV_FILE = $(TARGET).info
	GCDA_FILES = $(TEST_OBJS:.o=.gcda)
	GCNO_FILES = $(TEST_OBJS:.o=.gcno)

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

bld: $(TARGET)
ut:  $(TEST_DIR)/$(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

$(TEST_DIR)/$(TEST_TARGET): $(TEST_OBJS) $(UT_TEST_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(GCOV_LDFLAGS) $^ -o $@ 

%.o:%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

%_test.o:%.cpp
	$(CC) $(DUTFLAG) $(CFLAGS) $(GCOV_CFLAGS) -o $@ -c $<

%_ut.o:%.cpp
	$(CC) $(DUTFLAG) $(CFLAGS) -o $@ -c $<
run_ut: ut
	$(TEST_DIR)/$(TEST_TARGET) --run_test=SingletonRandom_test --log_level=warning;echo
	$(TEST_DIR)/$(TEST_TARGET) --log_level=warning;echo
collect_cov:
	lcov --rc lcov_branch_coverage=1 -d . -b . -c -o $(COV_FILE)
	lcov --rc lcov_branch_coverage=1 -e $(COV_FILE) "*/$(WORK_DIR)*" -o  $(COV_FILE)
	genhtml --rc lcov_branch_coverage=1 -o $(GCOV_RESULT) $(COV_FILE)
clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(UT_TEST_OBJS) $(TARGET) $(TEST_DIR)/$(TEST_TARGET) $(GCDA_FILES) $(GCNO_FILES) $(COV_FILE) 
