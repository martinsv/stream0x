// server.hpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#ifndef STREAM0X_SERVER_H
#define STREAM0X_SERVER_H

#include <iostream>
#include <algorithm>
#include <string>
#include <memory> // unique_ptr
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include "utils.hpp"    // unescape()
#include "streamer.hpp" // Streamer()

namespace stream0x
{
  namespace server
  {
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::exception;
    using std::for_each;
    using std::remove_if;
    using std::string;
    using std::stringstream;
    using std::unique_ptr;
    using std::vector;

    using boost::bind;
    using boost::is_any_of;
    using boost::shared_ptr;
    using boost::split;
    using boost::token_compress_on;
    namespace ba = boost::asio;
    using ba::ip::tcp;

    using stream0x::utils::unescape;
    using namespace stream0x::streamer;

    class Server
    {
      private:
        string hostname, port;
        ba::io_service io_service;
        /* 
         * Create a socket that can be passed around (asio sockets are
         * by default non-copyable).  Note:  I tried to use move
         * semantics for this, but I could not get it to work.
         *
         * "The shared_ptr class template stores a pointer to a
         * dynamically allocated object, typically with a C++
         * new-expression. The object pointed to is guaranteed to be
         * deleted when the last shared_ptr pointing to it is destroyed
         * or reset." -- boost shared_ptr docs
         * */
        shared_ptr<tcp::socket> socket;
        unique_ptr<Streamer> streamer;

      public:
        Server(string _hostname, string _port) 
        : hostname(_hostname), 
          port(_port), 
          socket(new tcp::socket(io_service)),
          streamer(new Streamer(socket))
        {}
        void run();
    };
  }
}

#endif
