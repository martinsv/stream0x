// server_main.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "server.hpp"

// stream0x streaming mp3 server
// © 2011 Noah K. Tilton <tilton@cs.uchicago.edu>

namespace po = boost::program_options;
using namespace stream0x::server;

int main(int argc, char **argv)
{
  try
  {
    po::options_description opts("Usage");
    opts.add_options()
      // no arg
      ("help,h", "display this message")
      // required (not really "options")
      ("port,p",    
       po::value<string>()->required(), 
       "port number")

      ("host,n",
       po::value<string>()->required(), 
       "hostname or IP address")
      ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);

    auto usage = [&](int STATUS)
    { 
      cerr << opts << endl;
      exit(STATUS);
    };
    // dispatch command line arguments to the server
    if(vm.count("help")) { usage(-1); }
    else
    {
      po::notify(vm);
      unique_ptr<Server> s(new Server(vm["host"].as<string>(),
                                      vm["port"].as<string>() ));
      s->run();
    }
  }
  catch(exception& e)
  {
    cerr << "server exception: " << e.what() << endl;
    return -1;
  }

  return 0;
}
