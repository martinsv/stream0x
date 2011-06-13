// streamer.hpp // final project // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#ifndef STREAM0X_STREAMER_H
#define STREAM0X_STREAMER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/scoped_array.hpp>

#include "mp3.hpp"
#include "utils.hpp"

namespace stream0x
{
  namespace streamer
  {
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::for_each;
    using std::ifstream;
    using std::ios;
    using std::ostream;
    using std::ofstream;
    using std::pair;
    using std::string;
    using std::stringstream;
    using std::tuple;
    using std::unordered_map;
    using std::vector;

    namespace ba = boost::asio;
    using ba::ip::tcp;
    using boost::lexical_cast;
    using boost::regex;
    using boost::regex_search;
    using boost::scoped_array;
    using boost::shared_ptr;

    using stream0x::utils::http_time;

    class Streamer
    {
      private:
        vector<string> playlist;
        shared_ptr<tcp::socket> socket;   // handle to client socket
        void add(string file);

      public:
        Streamer(shared_ptr<tcp::socket> _socket) 
        : socket(_socket)
        {}
        void headers();
        void play();
        void goodbye();
        void clear(string);
        void status();
        void enqueue(string);
        void err();
    };
  }
}

#endif
