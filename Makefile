CFLAGS=-Wall -g

clean_MACOSX: 
	rm -f hw1-MACOSX

clean_MINIX: 
	rm -f ex1-MINIX

MINIX:
	gcc -D MINIX hw1.c -o hw1-MINIX

MACOSX:
	gcc -D MACOSX hw1.c -o hw1-MACOSX


