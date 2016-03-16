########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : Reyham Soenaso
# Student Number   : s3496982
# Course Code      : COSC1076_1550
# Program Code     : BP094
# Start up code provided by Paul Miller
########################################################################

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
README=ppd_readme
MAKEFILE=Makefile
CC=gcc
CFLAGS=-ansi -Wall -pedantic -c
EXECUTABLE=ppd
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o $(EXECUTABLE)

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)