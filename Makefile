# Object files location. Object files will be placed in this directory during compilation
OBJDIR := objects

# Full names of object files
OBJECTS	:= $(addprefix $(OBJDIR)/, misc.o)

# ================================================================ #

# C compiler
CC := gcc

# We are only interested in object files without linking them into an executable `-c`
# Debugging included `-g`
CFLAGS := -g -c

# Extra layer of protection
ALL_CFLAGS := $(CFLAGS) -Wall -Wextra -pedantic-errors -O2

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Setting the value of the variable MISC to the path of the `misc.c`
MISC := $(addprefix misc/, misc.c)

TARGET := $(OBJDIR)/misc.o

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Define a target
all: $(OBJECTS)

# Building the `misc.o` object file from the MISC
$(OBJDIR)/misc.o: $(MISC)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Creating a directory if it does not exist. *nix
$(shell mkdir -p $(OBJDIR))

# ================================ #
# Executing the commands regardless of whether a file named clean exists or not
.PHONY: clean

clean:
	rm -rf $(OBJS) $(OBJDIR)