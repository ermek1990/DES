# Makefile for Separate Compilation

# *****************************************************
# Parameters to control Makefile operation

CPPFLAGS=-c -g -W -Wall

# ****************************************************
# Entries to bring the executable up to date

all: hw5

hw5: main.o helper.o tablechecker.o encryptFx.o utility.o
	g++ -o hw5 main.o helper.o tablechecker.o encryptFx.o utility.o
	
main.o: main.cpp
	g++ $(CPPFLAGS) main.cpp

helper.o: helper.cpp
	g++ $(CPPFLAGS) helper.cpp

tablechecker.o: utility.o
	g++ $(CPPFLAGS) tablechecker.cpp utility.cpp
	
encryptFx.o: tablechecker.o utility.o
	g++ $(CPPFLAGS) encryptFx.cpp tablechecker.cpp utility.cpp
	
utility.o: utility.cpp
	g++ $(CPPFLAGS) utility.cpp

clean:
	rm -f *.o *.gch hw5

clean_output:
	rm -f *.o *.gch

clean_exec:
	rm -f hw5
