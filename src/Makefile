CC=gcc
PKGCONFIG = $(shell which pkg-config) 
CFLAGS=-Wall -Wextra -Werror #$(shell $(PKGCONFIG) --cflags gtk4) #-fsanitize=address
SOURCES=calc_functions/*.c
TEST_SOURCES=calc_functions/*.c tests/*.c 
UNAME_S=$(shell uname -s)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
OPEN := open
ifeq ($(UNAME_S),Linux)
LDFLAGS= -lm -lcheck -lpthread 
OPEN := xdg-open
endif
ifeq ($(UNAME_S),Darwin)
LDFLAGS= -lm -lcheck -lpthread
endif

all: install

test: clean
	$(CC) $(CFLAGS) $(TEST_SOURCES) $(LDFLAGS) $(GCOV_FLAGS) -o ./tests/test
	./tests/test

# gcov_report: test
# 	./test
# 	lcov --exclude *calc_functions.c -t "report" -o report.info -c -d .
# 	genhtml -o "html_report" report.info
# 	open html_report/index.html

gcov_report: clean test
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	rm -rf gcovreport gcovreport.info *.gcda *.gcno 
	$(OPEN) ./report/index.html

install:
	rm -rf s21_smart_calc
	mkdir s21_smart_calc
	qmake6 qt/s21_smart_calc.pro -o s21_smart_calc
	cd s21_smart_calc && make && cd ../
	#chmod +x s21_smart_calc/s21_smart_calc
	#ln -s ${PWD}/s21_smart_calc/qt_smart_calc.app ${HOME}/Desktop/Smart_Calc\_v1.0

uninstall:
	rm -rf s21_smart_calc
	#rm ${HOME}/Desktop/Smart\ Calc\ v1.0

val: install 
	CK_FORK=no valgrind --tool=memcheck --leak-check=yes  ./tests/test

dvi:
	$(OPEN) Readme.html

dist: install
	tar -czf s21_smart_calc.tar.gz --directory=s21_smart_calc/ .

clang-format:
	clang-format -style=google -i calc_functions/*.c calc_functions/*.h
	clang-format -style=google -i qt/*.h qt/*.cpp
	clang-format -style=google -n calc_functions/*.c calc_functions/*.h
	clang-format -style=google -n qt/*.h qt/*.cpp

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem calc_functions/*.c calc_functions/*.h qt/*.h qt/*.cpp

clean:
	rm -rf obj *.a test html_report report.info program *.o *.gch calc_functions/*.gch *.gcno *.gcda s21_smart_calc.tar.gz .qmake.stash
	rm -rf ./tests/*.gcda ./tests/*.gcno ./tests/test ./calc_functions/*.o  ./calc_functions/*.a
	rm -rf ./report

lib:
	$(CC) -c $(SOURCES) (LDFLAGS)
	ar -rc libcalc.a *.o
	ranlib libcalc.a

.PHONY: all test clean lib gcov_report
