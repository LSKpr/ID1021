# Define the compiler
CC = gcc

# Define compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Pattern rule to compile a single C file into an executable
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rule to clean up generated files
clean:
	rm -f *.o $(TARGET)
