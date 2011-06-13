// client_main.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "client.hpp"

// stream0x streaming mp3 client
// © 2011 Noah K. Tilton <tilton@cs.uchicago.edu>

using namespace stream0x::client;

int main(int argc, char **argv)
{
  try
  {
    po::options_description opts("Usage");
    opts.add_options()
      // no arg
      ("help,h", "display this message")
      ("kill,k", "stop the server")
      ("status,s", "display the playlist queue")

      // require an arg
      ("clear,c", 
       po::value<string>(),
       "clear the playlist <string match regex>")

      ("enqueue,e", 
       po::value<string>(),
       "enqueue a file into the playlist")

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
    if(vm.count("help") || (argc < 2))  { usage(-1); }
    else
    {
      po::notify(vm);
      unique_ptr<Client> c(new Client(vm["host"].as<string>(),
                                      vm["port"].as<string>() ));
      if(vm.count("kill"))           { c->kill(); }
      else if(vm.count("status"))    { c->status(); }
      else if(vm.count("clear"))     { c->clear(vm["clear"].as<string>()); }
      else if(vm.count("enqueue"))   { c->enqueue(vm["enqueue"].as<string>()); }
      else                           { usage(-1); } // unrecognized argument 
    }
  }
  catch(exception& e)
  {
    cerr << "client exception: " << e.what() << endl;
    return -1;
  }

  return 0;
}
