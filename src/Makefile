#CC = clang++ -ggdb3 -Wall -std=c++0x
CC = g++ -ggdb3 -Wall -std=c++0x

SRC = 			\
	mode.cpp	\
	store.cpp	\

HEADER = 		\
	mode.h		\
	series.h	\
	series_collection.h \
	store.h		\

OBJECT = $(SRC:%.cpp=%.o)

LIBS = 				\
	-lboost_program_options	\
	-lboost_serialization	\
	-lleveldb		\

all : libs tsm test TAGS

%.o : %.cpp %.h
	$(CC) -c -fpic -o $@ $<

%.o : %.cpp
	$(CC) -c -fpic -o $@ $<

tsm : main.o $(HEADER) $(OBJECT) Makefile
	$(CC) -o tsm main.o $(OBJECT) $(LIBS)

libs : libtsm.so libtsm.a

libtsm.a : $(OBJECT)
	ar rcs $@ $^

libtsm.so : $(OBJECT)
	gcc -shared -Wl,-soname,$@.1 -o $@.1.0.1 $^


############################################################
# These rules aren't executed by default, they're here to
# document how to build the app from libraries.  The point
# is to make it easier to build other apps from libtsm.
#
# To build tsm from tsmlib.a:
tsm-from-archive :
	$(CC) -o tsm-from-archive main.o libtsm.a $(LIBS)
#
# To build tsm from tsmlib.so:
tsm-from-so :
	$(CC) -o tsm-from-so main.o -L. -ltsm $(LIBS)
#
############################################################

python.o : python.cpp
	$(CC) -c -fpic -o $@ -I/usr/include/python2.6 $<

libpytsm.so : $(OBJECT) python.o
	$(CC) -shared -Wl,-soname,libpytsm.so -o pytsm.so	\
		python.o					\
		$(OBJECT) -lpython2.6 -lboost_python $(LIBS)

TESTS = 			\
	mode_test 		\
	header_test		\

test : $(TESTS)
	./test.sh

%_test : %_test.o mode.o $(OBJECT)
	$(CC) -o $@ $^ $(LIBS)
	-./$@

header_test :
	./build-test-header

clean :
	rm -f $(OBJECT) *.o *~ tsm TAGS *_test libtsm*

clean-test :
	rm -rf tsm-test-*/ test_[0-9]*\.[0-9]*

TAGS : *cpp *h
	etags *cpp *h



