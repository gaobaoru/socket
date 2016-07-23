#Makefile for WebServer
# godfrey 2016

CC := g++
SRCDIR := source
BINDIR := libs
BUILDDIR := makeprojects
TARGET := httpserver
UNAME := $(shell uname)

DeBUGFLAGS := -g3 -O0 -Wall
RTCHECKS := -fmudflap -fstack-check -gnato

PRODFLAGS := -Wall -O2

ifeq ($(UNAME), Linux)
CFLAGS := -std=c++11 -Iinclude/ $(DEBUGFLAGS)
LINK := -lpthread -lkqueue $(DEBUGFLAGS)
else
CFLAGS := -std=c++11 -stdlib=libc++ -Iinclude/ $(DEBUGFLAGS)
LINK := -stdlib=libc++ $(DEBUGFLAGS)
endif

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@echo " Linking..."$(LINK); $(CC) $^ -o $(BINDIR)/$(TARGET) $(LINK)
 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<
 
clean:
	@echo " Cleaning..."; rm -r $(BUILDDIR) $(BINDIR)/$(TARGET)*
 
.PHONY: clean

