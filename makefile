tests: test2 test3

test2:
	gcc -Wall -g -o test2 test2.c -lpthread

test3:
	gcc -Wall -g -o test3 test3.c -lpthread

clean:
	rm -f test2 test3
