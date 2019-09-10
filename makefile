# $@ The file name of the target.
# $< The name of the first dependency.
# $* The part of a filename which matched a suffix rule.
# $?  The names of all the dependencies newer than the target separated by spaces.
# $^ The names of all the dependencies separated by spaces, but with duplicate names removed.
# $+ The names of all the dependencies separated by spaces with duplicate names included and in the same order as in the rule.

cc = gcc
CFLAGS = -g -Wuninitialized -Wreturn-type \
		 -Wunused-label -Wunused-value -Wunused-variable \
		 -Wimplicit-function-declaration -Wimplicit-int -Wmissing-braces \
		 -Wnonnull -Wparentheses

INC_DIRS = -I./include

bitmap_test.o: bitmap.o ./test/bitmap_test.c
	$(cc) $(CFLAGS) $(INC_DIRS) -o bitmaptest $^ -lcunit

test: bitmaptest

testall: bitmap_sa bitmaptest bitmaptest_mem


bitmaptest: bitmap_test.o
	./bitmaptest

bitmaptest_mem: bitmap_test.o
	valgrind -v --tool=memcheck --leak-check=full --show-leak-kinds=all --error-exitcode=9 ./bitmaptest || (echo "Leak detected"; exit 1)

bitmap_sa:
	cppcheck .

bitmap.o: ./src/bitmap.c
	$(cc) $(CFLAGS) $(INC_DIRS) -c $^

.PHONY: clean

clean:
	rm -f *.o bitmaptest
