# GCC = gcc -std=c99 -Wall -Wshadow -Wvla -O3 -pedantic
GCC = gcc -std=c99 -Wall -Wshadow -Wvla -g -pedantic
# TEST = map5x5.txt query5x5.txt
# TEST = map2_5x5.txt query5x5.txt
# TEST = usa.txt usa1.txt
# TEST = usa.txt usa5.txt
# TEST = usa.txt usa10.txt
TEST = usa.txt usa100.txt

mapping.o: mapping.c
	$(GCC) -c mapping.c -o mapping.o

adjacent: adjacent.c
	$(GCC) adjacent.c -o adjacent -lm

proj3: mapping.o
	$(GCC) mapping.o -o proj3 -lm

test:
	./proj3 $(TEST)
	

# diff:

memory:
	valgrind --leak-check=full --track-origins=yes ./proj3 $(TEST)

clean:
	/bin/rm -f *.o
	/bin/rm -f proj3 *proj3 adjacent
