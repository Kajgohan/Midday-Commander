CC = gcc

CFLAGS = -Wall

TARGET2 = mc2
TARGET1 = mc1
TARGET0 = mc0

all:
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET0) $(TARGET0).c

clean:
	$(RM) $(TARGET2) *.o*~
	$(RM) $(TARGET1) *.o*~
	$(RM) $(TARGET0) *.o*~
