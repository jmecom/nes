CC = gcc
CFLAGS = -g -Wall
TEST_OBJS = test.o gamepak.o instructions.o memory.o error.o cpu.o 

default: test

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o nes_test $(TEST_OBJS)

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

gamepak.o: gamepak.c
	$(CC) $(CFLAGS) -c gamepak.c

instructions.o: instructions.c
	$(CC) $(CFLAGS) -c instructions.c

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c

error.o: error.c
	$(CC) $(CFLAGS) -c error.c

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c
	
clean: 
	$(RM) nes_test *.o *~