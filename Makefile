CC_SOURCE:=${sort $(shell ls *.cc) parser.tab.cc lexer.cc}
OBJS:=$(CC_SOURCE:.cc=.o)
DEPS:=$(CC_SOURCE:.cc=.d)
CXXFLAGS=-ggdb3 -std=c++17
LIBS=-lfl -lboost_regex

infomysql: ${OBJS} parser.y lexer.l
	g++ -o $@ ${OBJS} ${LIBS}

clean:
	rm *.d *.o lexer.cc parser.tab.h parser.tab.cc

include ${DEPS}

%.d: %.cc
	g++ ${CXXFLAGS} -MM $< -MF$@

lexer.cc: lexer.l parser.tab.h
	flex -o lexer.cc lexer.l

parser.tab.cc: parser.y
	bison -o parser.tab.cc --defines=parser.tab.h parser.y
