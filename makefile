CC = gcc
CFLAGS = -Wall -g
TARGET = main
INCLUDES = -I./utils -I./lexer -I./parser # Include both lexer and utils directories
SRCS = main.c utils/utils.c lexer/lexer.c parser/parser.c # Source files list
OBJS = $(SRCS:.c=.o)  # Object files list

all: $(TARGET)

# This target specifies how to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

# This target specifies how to build the object files from the source files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# This target specifies how to run the program
run: $(TARGET)
	./$(TARGET)

# This target specifies how to clean up the build
clean:
	rm -f $(OBJS)

# This target specifies how to clean up all files generated by the build
clean_all: clean
	rm -f $(TARGET)

.PHONY: all clean clean_all run
