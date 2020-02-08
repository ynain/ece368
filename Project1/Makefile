GCC = gcc -lm -Werror -Wall -O3
# GCC = gcc -lm -Werror -Wall -g

FILES = sorting.c sorting_main.c

# ITEST = ./proj1 i 1000.txt 1000iseq.txt 1000is.txt
# ITEST = ./proj1 i 10000.txt 10000iseq.txt 10000is.txt
ITEST = ./proj1 i 100000.txt 100000iseq.txt 100000is.txt

# STEST = ./proj1 s 1000.txt 1000sseq.txt 1000ss.txt
# STEST = ./proj1 s 10000.txt 10000sseq.txt 10000ss.txt
STEST = ./proj1 s 100000.txt 100000sseq.txt 100000ss.txt


all: $(OBJECTS)
	$(GCC) $(FILES) -o proj1

test:
	$(ITEST)
	$(STEST)

simple:
	./proj1 i 10.txt 10iseq.txt 10is.txt
	./proj1 s 10.txt 10sseq.txt 10ss.txt
	
simplediff:
	diff 10seq.txt 10iseq.txt
	diff 10s.txt 10is.txt
	diff 10seq.txt 10sseq.txt
	diff 10s.txt 10ss.txt


diff:
	diff 1000seq.txt 1000iseq.txt
	diff 1000s.txt 1000is.txt
	diff 1000seq.txt 1000sseq.txt
	diff 1000s.txt 1000ss.txt

memory:
	valgrind --leak-check=full --track-origins=yes $(ITEST)
	valgrind --leak-check=full --track-origins=yes $(STEST)

simplememory:
	valgrind --leak-check=full --track-origins=yes ./proj1 i 10.txt 10iseq.txt 10is.txt
	valgrind --leak-check=full --track-origins=yes ./proj1 s 10.txt 10sseq.txt 10ss.txt


clean:
	/bin/rm -f *.o
	/bin/rm -f proj1 *proj1
	/bin/rm -f *iseq.txt *is.txt
	/bin/rm -f *sseq.txt *ss.txt
