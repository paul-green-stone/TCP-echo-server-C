# Object files location. Object files will be placed in this directory during compilation
OBJDIR := objects

# Full names of object files
OBJECTS	:= $(addprefix $(OBJDIR)/, main.o)

# ================================================================ #

# C compiler
CC := gcc

# We are only interested in object files without linking them into an executable `-c`
# Debugging included `-g`
CFLAGS := -g -c

# Extra layer of protection
ALL_CFLAGS := $(CFLAGS) -Wall -Wextra -pedantic-errors -O2

# Linker flags
LDFLAGS := -g

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Setting the value of the variable MAIN to the path of the `main.c`
MAIN := $(addprefix ./, main.c)

# Setting the value of the variable MISC to the path of the `misc.o`
MISC := $(addprefix ../objects/, misc.o)

# Executable name
BIN := client

# ================================================================ #
# ================================================================ #
# ================================================================ #

all: $(BIN)

# Building the `main.o` object file from the MAIN
$(OBJDIR)/main.o: $(MAIN)
	$(CC) $(ALL_CFLAGS) -o $@ $<

# Building the executable
$(BIN): $(OBJECTS) ../$(OBJDIR)/misc.o
	$(CC) $(LDFLAGS) -o $(BIN).out $(OBJECTS) ../$(OBJDIR)/misc.o

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Creating a directory if it does not exist. *nix
$(shell mkdir -p $(OBJDIR))

# ================================ #
# Executing the commands regardless of whether a file named clean exists or not
.PHONY: clean

clean:
	rm -rf $(OBJS) $(OBJDIR) $(BIN).out $(BIN)