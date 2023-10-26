#FLAGS = -Wall -Wextra -Werror -std=c11
FLAGS = -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
TST_LIBS = -lcheck
LIB_NAME = s21_smartcalc
SRCS=s21_stack.c s21_smart_calc.c
OBJS=$(SRCS:.c=.o)
OPEN := open
ifeq ($(shell uname), Linux)
	TST_LIBS += -lpthread -lrt -lm
	OPEN := xdg-open
endif

all: test

lol: 
	gcc s21_smart_calc.c s21_stack.c -o lol
	./lol

dbg:
	gcc -g huila.c
	gdb a.out

run:
	gcc s21_stack.c -o main
	./main

$(LIB_NAME).a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a

test: clean $(LIB_NAME).a
	$(CC) $(FLAGS) test.c $(LIB_NAME).a $(TST_LIBS) -o test
	./test

gcov_report: clean test
	gcc $(CFLAGS) $(SRCS)  test.c  $(TST_LIBS) $(GCOVFLAGS) -o gcovreport
	./gcovreport
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	rm -rf gcovreport gcovreport.info *.gcda *.gcno 
	$(OPEN) ./report/index.html


clean:
	rm -rf *.o test *.a *.gcno *.gcda *.gcov *.html *.css *.info ./report *.out ./lol

cln:
	clang-format -style=Google -n *.c *.h

cli:
	clang-format -style=Google -i *.c *.h

val: test
	CK_FORK=no valgrind --tool=memcheck --leak-check=yes  ./test
