GCC = g++ -ggdb3 -Wall -std=c++0x

SRC = 			\
	mode.cpp	\

HEADER = mode.h

OBJECT = $(SRC:%.cpp=%.o)

LIBS = 				\
	-lboost_program_options	\
	-lboost_serialization	\

all : libs tsm test TAGS

%.o : %.cpp %.h
	$(GCC) -c -fpic -o $@ $<

%.o : %.cpp
	$(GCC) -c -fpic -o $@ $<

tsm : main.o $(HEADER) $(OBJECT) Makefile
	$(GCC) -o tsm main.o $(OBJECT) $(LIBS)

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
	$(GCC) -o tsm-from-archive main.o libtsm.a $(LIBS)
#
# To build tsm from tsmlib.so:
tsm-from-so :
	$(GCC) -o tsm-from-so main.o -L. -ltsm $(LIBS)
#
############################################################

python.o : python.cpp
	$(GCC) -c -fpic -o $@ -I/usr/include/python2.6 $<

libpytsm.so : $(OBJECT) python.o
	$(GCC) -shared -Wl,-soname,libpytsm.so -o pytsm.so	\
		python.o					\
		$(OBJECT) -lpython2.6 -lboost_python $(LIBS)

TESTS = 			\
	mode_test 		\

test : $(TESTS)
	./test.sh

%_test : %_test.o mode.o $(OBJECT)
	$(GCC) -o $@ $^ $(LIBS)
	-./$@

clean :
	rm -f $(OBJECT) *.o *~ tsm TAGS *_test libtsm*

clean-test :
	rm -rf tsm-test-*/ test_[0-9]*\.[0-9]*

TAGS : *cpp *h
	etags *cpp *h


