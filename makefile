CC=gcc
CFLAGS=-Wall -ansi

fleschIndex: A4.o
	$(CC) -o A4 A4.o

enterStudents.o: A4.c
	$(CC) $(CFLAGS) -c A4.c
clean:
	rm A4 A4.o myContactList.db