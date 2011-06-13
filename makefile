CXX=g++
CXXFLAGS=-Wall -g -O2 -std=gnu++0x -pipe -march=native
MAKEFLAGS=-j3

programs=server client

## aggregate targets

all: $(programs)
clean:
	-rm -rfv $(programs) *.o

## the object targets are implicit.  further, they don't need to be
## linked to external libraries -- only the executables which use my
## objects require linkage to boost, cURL, etc.

## program targets

client_objs=utils.o mp3.o client.o client_main.o
client: $(client_objs)
	$(CXX) $(CXXFLAGS) -o client \
	$(client_objs) \
	-lboost_program_options -lboost_filesystem -lboost_system -lboost_date_time \
	-lcurl

server_objs=utils.o mp3.o streamer.o server.o server_main.o
server: $(server_objs)
	$(CXX) $(CXXFLAGS) -o server \
	$(server_objs) \
	-lboost_program_options -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time \
	-lcurl 
