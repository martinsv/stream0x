# README :: stream0x :: tilton@cs.uchicago.edu :: cspp 51045

# About stream0x

  This is the readme file for stream0x, an mp3 client/server program
  written in C++ as my final project for CSPP 51045, taken in the Spring
  quarter at the University of Chicago and taught by Professor Mike
  Spertus.

  stream0x uses the SHOUTcast network protocol[1], which is documented
  well--if not exhaustively--at various locations on the web.

  This project takes as its inspiration a SHOUTcast server prototype I
  hacked together in python, called riddim.  riddim can be found at
  github[2].  

# Goals and motivation

  The main goal of this project is to demonstrate my understanding of
  substantial advanced and cutting-edge features of C++0x, as covered in
  CSPP 51045, and as embodied by the GNU C++0x language extensions.  This
  project is being developed on Linux but is intended to be portable to
  other platforms.

  The motivation behind riddim and stream0x is that I spend a lot of time using
  the command line, and I prefer to be able to listen to my music collection
  remotely.

# TODO (limitations)

  * handles mp3s only for now
  * single-threaded only for now, only one connection at a time
  * no persistence

# Dependencies

  * Boost (I used 1.46.1)
  * A modern C++ compiler.
  * libcurl

# Building stream0x

  The following instructions are for linux.  Building on windows is untested,
  but should also work:

    % cd ./stream0x
    % make

# Running stream0x

## Quickstart for the impatient

  First, start the server.  Hostname and port are both required arguments:

      % ./server -p 31616 -n localhost
      server running at 0.0.0.0:31616
      waiting for a connection

  Second, enqueue some tracks.  The client is essentially a playlist manager.
  It can be used to enqueue, clear, and query the playlist, as follows:

     % ./client -p 31616 -n localhost -e /path

  Where path is either a single mp3 file or a directory containing mp3s.  The
  directory search is not recursive.

  Once you have enqueued tracks, you can query the playlist status:

     % ./client -p 31616 -n localhost -s

  And you can clear tracks based upon a regular expression match.  For example:

     % ./client -p 31616 -n localhost -c dylan  # clear all tracks matching "dylan"
     % ./client -p 31616 -n localhost -c .      # clear all tracks

  Finally, once the playlist is as you like it, open your favorite audio player
  and open a stream connection to:

    http://server:port 

## Extended usage

  stream0x is comprised by two parts, a server and client, each of which
  is a command line binary program.  Both programs contain a help menu:

      % ./server -h 
      Usage:
        -h [ --help ]         display this message
        -p [ --port ] arg     port number
        -n [ --host ] arg     hostname or IP address

      % ./client -h
      Usage:
        -h [ --help ]         display this message
        -k [ --kill ]         stop the server
        -s [ --status ]       display the playlist queue
        -c [ --clear ] arg    clear the playlist <string match regex>
        -e [ --enqueue ] arg  enqueue a file into the playlist
        -p [ --port ] arg     port number
        -n [ --host ] arg     hostname or IP address

# References

[1]     "Shoutcast Metadata Protocol", 
                http://www.smackfu.com/stuff/programming/shoutcast.html;

        "Practical Common Lisp", Ch 28. "A Shoutcast Server", 
                http://www.gigamonkeys.com/book/practical-a-shoutcast-server.html

[2]     riddim SHOUTcast server, https://github.com/noah/riddim
