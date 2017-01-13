
#
# The PROGRAM macro defines the name of the program or project.  It
# allows the program name to be changed by editing in only one
# location
#

PROGRAM = GENERATOR


#
# The INCLUDEDIRS macro contains a list of include directories
# to pass to the compiler so it can find necessary header files.
#
# The LIBDIRS macro contains a list of library directories
# to pass to the linker so it can find necessary libraries.
#
# The LIBS macro contains a list of libraries that the the
# executable must be linked against.
#
DllSuf        = so
CURRENTDIR = $(shell pwd)
INCLUDEDIRS = \
 	$(shell root-config --cflags)

LIBDIRS = \
	$(shell root-config --libs)

LIBS = -lEGPythia6 -lEG
	 
LD= g++

CXXSOURCES =  pythia_ccbar.cpp
CXXOBJECTS = $(CXXSOURCES:.cpp=.o)
CXXFLAGS =  $(INCLUDEDIRS)
CXX = g++ -g -m64 -O3 -std=c++11

LDFLAGS = $(LIBDIRS) $(LIBS)


all:	$(PROGRAM)

$(PROGRAM): $(CXXOBJECTS)
	$(CXX)  -o $@ $(CXXOBJECTS) $(LDFLAGS) 


clean:
	$(RM) -f $(CXXOBJECTS) $(PROGRAM)


