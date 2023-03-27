tests: src/option_i32.c test/munit.c
	clang -D__OPTION_TESTS -Wall -fblocks -lBlocksRuntime src/option_i32.c test/munit.c -o test/option_tests.out

clean: 
	rm test/option_tests.out
