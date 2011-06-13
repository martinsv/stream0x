// client.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#ifndef STREAM0X_CLIENT_H
#define STREAM0X_CLIENT_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "utils.hpp"    // get()
#include "mp3.hpp"      // is_mp3()

namespace stream0x
{
  namespace client
  {
    using std::back_inserter;
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::exception;
    using std::for_each;
    using std::sort;
    using std::string;
    using std::transform;
    using std::unique_ptr;
    using std::vector;

    using boost::lexical_cast;
    namespace bf3 = boost::filesystem3;
    namespace ba = boost::asio;
    using ba::ip::tcp;
    namespace po = boost::program_options;

    using stream0x::utils::get;
    using stream0x::mp3::is_mp3;

    class Client
    {
      private:
        string hostname, port;

      public:
        Client(string _hostname, string _port) 
        : hostname(_hostname), 
          port(_port) 
        {}
        void kill();
        void status();
        void clear(string pattern);
        void enqueue(string path);
        void command(string get);
    };
  }
}

#endif
