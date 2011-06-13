// client.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "client.hpp"

namespace stream0x
{
  namespace client
  {
    void Client::command(string get)
    {
      try
      {
        // Connect to server and issue GET request
        ba::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), hostname, port);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        socket.connect(*iterator);
        ba::write(socket, ba::buffer(get, get.size()));
        boost::system::error_code error;

        ba::streambuf buffer;
        while(ba::read(socket, buffer, ba::transfer_at_least(1), error))
        { 
          cout << &buffer << endl;
        }
      }
      catch(exception& e)
      {
        cerr << "Exception: " << e.what () << "\n";
      }
    }

    void Client::kill()
    {
      command(get("kill"));
    }

    void Client::clear(string pattern)
    {
      command(get("clear", pattern));
    }

    void Client::status()
    {
      command(get("status"));
    }

    void Client::enqueue(string arg_path)
    {
      bf3::path p(arg_path);
      int enqueued = 0;
      try
      {
        if(!bf3::exists(p))
        { 
          cout << "The specified path does not exist." << endl;
          exit(-1);
        }

        if(bf3::is_regular_file(p) && is_mp3(p))
        {
          command(get("enqueue", p.string()));
          ++enqueued;
        }
        else if(bf3::is_directory(p))
        {
          vector<string> filenames;
          transform(bf3::directory_iterator(p), bf3::directory_iterator(), back_inserter(filenames), [](bf3::path _p){ return bf3::absolute(_p).string(); });
          sort(filenames.begin(), filenames.end());
          for_each(filenames.begin(), filenames.end(), [&](string filename){
            if(is_mp3(filename))
            {
              command(get("enqueue", filename));
              ++enqueued;
            }
          });
        }
        else
        {
          cout << p << " exists but is not playable." << endl; 
          exit(-1);
        }
        command(get("status"));
      }
      catch (const bf3::filesystem_error& ex)
      { 
        cout << ex.what() << endl;
      }
      cout << "Enqueued " << lexical_cast<string>(enqueued) << " files." << endl;
    }

  }
}
