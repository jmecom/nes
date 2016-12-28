CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c99
TEST_OBJS = test.o gamepak.o constants.o instructions.o macros.o logging.o cpu.o 

default: test

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o nes_test $(TEST_OBJS)

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

gamepak.o: gamepak.c
	$(CC) $(CFLAGS) -c gamepak.c

instructions.o: instructions.c
	$(CC) $(CFLAGS) -c instructions.c

constants.o: constants.c
	$(CC) $(CFLAGS) -c constants.c

macros.o: macros.c
	$(CC) $(CFLAGS) -c macros.c

logging.o: logging.c
	$(CC) $(CFLAGS) -c logging.c

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c
	
clean: 
	$(RM) nes_test *.o out.log tags *~