CC=gcc

default: 
	$(CC) -o chatclient chatclient.c

clean:
	rm chatclient