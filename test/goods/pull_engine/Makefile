# A sample Makefile for building Google Test and using it in user
# tests.  Please tweak it to suit your environment and project.  You
# may want to move it to your project's root directory.
#
# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.

# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.

LIB_DIR = ../../../../library/mylib
PLG_DIR = ../../../plugins
PUB_DIR = ../../../pub
GTEST_DIR = $(LIB_DIR)/third_party/gtest

# Where to find user code.
USER_DIR = $(LIB_DIR)/base

THIRD_PATH = $(LIB_DIR)/public

CUSTOM_THIRD_PATH = $(LIB_DIR)/third_party

LINK_PATHS = -L./ -L/usr/local/libmemcached-1.0.7/lib/  -L/usr/lib64/mysql

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -D_DEBUG

# Flags passed to the C++ compiler.
CXXFLAGS += -g  -Wextra -pthread

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = test_fx_parser_rule

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

USER_HEADERS =  /usr/include/mysql/*.h \
				/usr/local/freetds/include/*.h \
                $(PLG_DIR)/*.h \
                $(PUB_DIR)/logic/*/*.h \
				$(THIRD_PATH)/*/*.h \
                $(USER_DIR)/*/*.h \
                $(CUSTOM_THIRD_PATH)/hiredis/*.h \

INCLUDE_PATHS = -I/usr/local/libmemcached-1.0.7/include \
				-I$(THIRD_PATH)\
				-I/usr/local/include\
				-I/usr/include/mysql\
				-I$(PLG_DIR)\
				-I$(PUB_DIR)\
				-I$(USER_DIR)\
				-I$(CUSTOM_THIRD_PATH)/hiredis\
				-I$(CUSTOM_THIRD_PATH)\


# House-keeping build targets.

ALL_HEADERS = $(GTEST_HEADERS) $(USER_HEADERS)
all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o *.gch

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

test_fx_parser_rule.o : ./test_fx_parser_rule.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./test_fx_parser_rule.cc $(INCLUDE_PATHS)

fx_pull_engine.o : ./$(PLG_DIR)/goods/fx_pull_engine.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PLG_DIR)/goods/fx_pull_engine.cc $(INCLUDE_PATHS)

jh_pull_engine.o : ./$(PLG_DIR)/goods/jh_pull_engine.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PLG_DIR)/goods/jh_pull_engine.cc $(INCLUDE_PATHS)

pull_engine.o : ./$(PLG_DIR)/goods/pull_engine.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PLG_DIR)/goods/pull_engine.cc $(INCLUDE_PATHS)

logic_unit.o : ./$(PUB_DIR)/logic/logic_unit.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PUB_DIR)/logic/logic_unit.cc $(INCLUDE_PATHS)

quotations_infos.o : ./$(PUB_DIR)/logic/quotations_infos.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PUB_DIR)/logic/quotations_infos.cc $(INCLUDE_PATHS)

http_api.o :./$(PUB_DIR)/net/http_api.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./$(PUB_DIR)/net/http_api.cc $(INCLUDE_PATHS)

test_fx_parser_rule : test_fx_parser_rule.o fx_pull_engine.o pull_engine.o http_api.o jh_pull_engine.o\
	logic_unit.o quotations_infos.o gtest_main.a  $(CUSTOM_THIRD_PATH)/dmg_fp/lib/dmg_fp.a $(LIB_DIR)/base/lib/mybase.a $(LIB_DIR)/third_party/lib/libhiredis.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LINK_PATHS) -lpthread -lexpat -lrt -ldl -lmysqlclient -lcurl \
	$^ -o $@
