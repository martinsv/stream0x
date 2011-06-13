// server.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "server.hpp"

namespace stream0x
{
  namespace server
  {
    vector<string> parse_get_path(string data)
    {
      vector<string> http_request, get_request, get_paths;
      // the SHOUTcast protocol uses standard features of HTTP, which I am
      // "extending" here to create something of a poor man's REST.
      split(http_request, data,               
            is_any_of("\r\n"), token_compress_on);
      split(get_request, http_request[0],    
            is_any_of(" "), token_compress_on);
      if(get_request[1] == "/") { get_paths.push_back("/"); } // handle request for /
      else
      {
        split(get_paths, get_request[1], 
              is_any_of("/"), token_compress_on);
        // i.e., [1] of the string "GET /some/request/path HTTP/1.1";
        // split on space
        get_paths.erase(remove_if(get_paths.begin(), get_paths.end(), 
                                  bind(&string::empty, _1)), get_paths.end());
      }
      return get_paths;
    }

    void Server::run()
    {
      try
      {
        tcp::endpoint endpoint(tcp::endpoint(tcp::v4(), boost::lexical_cast<unsigned int>(port)));
        tcp::acceptor acceptor(io_service, endpoint);
        acceptor.set_option(ba::ip::tcp::acceptor::reuse_address(true));
        cout << "server running at " << endpoint << endl;

        for(;;)
        {
          cout << "waiting for a connection" << endl;
          acceptor.accept(*socket); // blocking wait.

          // handle client connection.  first, read request into buffer
          cout << "connection from " << socket->remote_endpoint() << endl;
          boost::system::error_code error;

          stringstream ss;
          ba::streambuf buffer;
          boost::asio::read_until(*socket, buffer, "\n"); // buffer up to the end of the first line
          ss << &buffer;

          // unescape the request, and parse it
          vector<string> get_paths = parse_get_path(ss.str());

          // Now validate and dispatch the client's request
          if(get_paths.size() < 1) { cerr << "Invalid operation." << endl; }
          string command  = get_paths[0];
          string argument = unescape(get_paths[1]);
          cout << command << " (" << argument << ")" << endl;
          if(command == "/")              { streamer->headers(); streamer->play();      }
          else if(command == "kill")      { streamer->goodbye(); break;                 }
          else if(command == "clear")     { streamer->clear(argument);                  }
          else if(command == "status")    { streamer->status();                         }
          else if(command == "enqueue")   { streamer->enqueue(argument);                }
          else                            { streamer->err();                            }

          socket->close();
        }
        cout << "Server shutting down ..." << endl;
      }
      catch(exception& e)
      {
        cerr << e.what() << endl;
      }
    }
  }
}
