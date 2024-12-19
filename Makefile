GCC=gcc -g
CFALGS=-Wall -Werror -Wextra -std=c11 
SOURCES=*.c
TESTS_SOURCES=tests/*.c
OBJECTS=$(SOURCES:.c=.o)
CLANG=../materials/linters/.clang-format 

ifeq ($(shell uname), Linux)
	CHECKFLAGS=-lcheck -lm -lpthread -lrt -lsubunit
else
	CHECKFLAGS=-lcheck -lm -lpthread
endif

all: clean s21_matrix.a

s21_matrix.a:
	$(GCC) $(CFALGS) $(SOURCES) -c
	ar rcs s21_matrix.a $(OBJECTS)

test: clean s21_matrix.a
	$(GCC) $(CFALGS) $(TESTS_SOURCES) s21_matrix.a $(CHECKFLAGS) -o s21_matrix
	./s21_matrix

clean:
	rm -rf s21_matrix *.a $(OBJECTS) test *.gcno *.gcda report report.info *.dSYM

gcov_report: clean s21_matrix.a
	$(GCC) $(CFALGS) $(TESTS_SOURCES) $(SOURCES) s21_matrix.a $(CHECKFLAGS) -fprofile-arcs -ftest-coverage -lgcov -o s21_matrix
	./s21_matrix
	lcov -t "./s21_matrix" -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html

clang_checking:
	cp $(CLANG) ./
	cp $(CLANG) ./tests
	clang-format -n *.c *.h
	clang-format -n tests/*.c
	rm .clang-format
	rm ./tests/.clang-format
	
clang_format:
	cp $(CLANG) ./
	cp $(CLANG) ./tests
	clang-format -i -style=google *.c *.h
	clang-format -i -style=google tests/*.c
	rm .clang-format
	rm ./tests/.clang-format

valgrind: clean test
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./s21_matrix