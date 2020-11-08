# project name (generate executable with this name)
TARGET=main

EXEC=main*
# (The wildcard is used so that this file can be used both in linux and windows systems extension for executable files)

# Compiler
CC=g++ --std=c++11

# compiling flags here
CFLAGS   = -c -Wall -I.

# Linker
LINKER   = g++

# linking flags here
LFLAGS   = -Wall -I. -lsqlite3 -o

# directories
OBJDIR=obj
LIBDIR=lib
INCDIR=include
SRCDIR=./
BINDIR=bin

LIBS=-lm

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
LIBRARIES:= $(wildcard $(LIBDIR)/*.cpp)
LIBOBJS	 := $(LIBRARIES:$(LIBDIR)/%.cpp=$(OBJDIR)/%.o)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

rm       = rm -f
rmdir		 = rm -rf

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): DIR $(LIBOBJS) $(OBJECTS)
	@$(LINKER) $(LIBOBJS) $(OBJECTS) $(LFLAGS) $@
	@echo "Linking complete!"

DIR: | $(OBJDIR) $(BINDIR)

$(LIBOBJS): $(OBJDIR)/%.o : $(LIBDIR)/%.cpp
	@$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<" dependency successfully!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<" object successfully!"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

.PHONY: clean run remove delete

clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(LIBOBJS)
	@echo "All object files cleaned!"

remove: clean
	@$(rm) $(BINDIR)/$(EXEC)
	@echo "Executable removed!"

delete: remove clean
	@$(rmdir) $(BINDIR)
	@$(rmdir) $(OBJDIR)
	@echo "obj and bin directories deleted!"

run: #all
	@echo "Starting program..."
	@./$(BINDIR)/$(TARGET)
