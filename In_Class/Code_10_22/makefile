build:
	rm -f ./a.out
	g++ -std=c++20 -g -Wall -pedantic -Werror main.cpp -lm -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++20 -g -Wall -pedantic -Werror main.cpp -lm -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./a.out
