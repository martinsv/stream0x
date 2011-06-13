// streamer.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "streamer.hpp"

namespace stream0x
{
  namespace streamer
  {
    void Streamer::clear(string pattern) // clear tracks from the playlist by regular expression pattern
    {
      regex re(pattern);
      size_t size = playlist.size();
      playlist.erase(
          remove_if(playlist.begin(), playlist.end(),
            [&re](string track) {
                return regex_search(track, re); // prune track if it matches the user's pattern
            }), 
            playlist.end());
      size_t after_size = playlist.size();
      ba::write(*socket, ba::buffer((
                            (size == after_size)
                            ? "no matches"
                            : "cleared " + lexical_cast<string>(size-after_size)) + "\n"));
    }

    void Streamer::status() // display the playlist
    {
      stringstream ss;
      int n_tracks = playlist.size();
      if(n_tracks < 1)
      {
        ss << ">>> Playlist empty." << "\n";
      }
      else
      {
        ss << ">>> " << n_tracks << " tracks" << endl;
        int i=0;
        for_each(playlist.begin(), playlist.end(), [&](string entry) {
            ss << ++i << " " << entry << "\n";
        });
      }
      ba::write(*socket, ba::buffer(ss.str()));
    }

    void Streamer::enqueue(string file) // enqueue tracks onto the playlist
    {
      playlist.push_back(file);
      ba::write(*socket, ba::buffer("enqueued " + file));
    }

    /* State notification */

    void Streamer::err()
    {
      ba::write(*socket, ba::buffer("Not understood."));
    }

    void Streamer::goodbye()
    {
      ba::write(*socket, ba::buffer("Goodbye!\n"));
    }

    /* The big bad mp3 streaming part */

    void Streamer::headers()
    {
      typedef unordered_map<string, string> string_map;
      ba::streambuf headers_buf;
      ostream headers_s(&headers_buf);
      headers_s << "HTTP/1.0 200 ICY\r\n";
      string_map headers;
      headers["icy-notice1"]    = "stream0x";
      headers["icy-notice2"]    = "stream0x mp3 server";
      headers["icy-name"]       = "stream0x mp3 server";
      headers["icy-pub"]        = "False";
      headers["icy-genre"]      = "no genre";
      headers["icy-url"]        = "http://cs.uchicago.edu/~tilton";
      headers["icy-metaint"]    = "16384";
      headers["content-type"]   = "audio/mpeg";
      headers["Server"]         = "stream0x/0.1 C++0x";
      headers["Date"]           = http_time(); //e.g., "Fri, 03 Jun 2011 00:05:59 GMT"
      for(string_map::iterator it = headers.begin(); it != headers.end(); ++it)
      {
          headers_s << it->first << ": " << it->second << "\r\n";
      }
      headers_s << "\r\n";
      ba::write(*socket, headers_buf);
    }

    void Streamer::play()
    {
      const unsigned int metadata_interval      = 16384;
      unsigned int byte_count                   = 0;

      for_each(playlist.begin(), playlist.end(), [&](string track) {

        cout << "> " << track << endl;
        auto mp3_info                   = mp3::info(track);
        unsigned int audio_start        = std::get<0>(mp3_info);
        string audio_metadata           = std::get<1>(mp3_info);

        bool metadata_sent              = false;
        unsigned int bytes_until_meta = 0;

        boost::system::error_code error;
        boost::array<char, 4096> buf;
        ifstream mp3;
        mp3.open(track);
        mp3.seekg(audio_start);

        while(!mp3.eof())
        {
          bytes_until_meta = (metadata_interval - byte_count);
          if(bytes_until_meta == 0)
          {
            ba::streambuf metadata_request;
            ostream metadata_stream(&metadata_request);
            if(metadata_sent) { metadata_stream << '\x00'; }
            else
            {
              metadata_stream << audio_metadata << std::flush;
              metadata_sent = true;
            }
            ba::write(*socket, metadata_request, ba::transfer_all(), error);
            if(error) { std::cout << "send error:" << error << std::endl; }
            byte_count = 0;
          }
          else
          {
            size_t buffer_size = buf.size();
            unsigned int n_bytes = (bytes_until_meta < buffer_size)
              ? bytes_until_meta
              : buffer_size;
            
            mp3.read(buf.c_array(), (std::streamsize)n_bytes);
            if(!mp3.gcount()) { cout << "read file error" << endl; }
            std::size_t bytes_written = ba::write(*socket, ba::buffer(buf.c_array(), 
                                                                      mp3.gcount()), 
                                                                      ba::transfer_all(),
                                                                      error);
            if(error) { std::cout << "send error:" << error << std::endl; }
            byte_count += bytes_written;
          }
        }
        metadata_sent = false;
      });

    }
  }
}
