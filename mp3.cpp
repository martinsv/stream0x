// mp3.cpp // final project // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "mp3.hpp"

/*
 * The Shoutcast Protocol
 *
 * The Shoutcast protocol was invented by the folks at Nullsoft, the
 * makers of the Winamp MP3 software. It was designed to support
 * Internet audio broadcasting--Shoutcast DJs send audio data from their
 * personal computers to a central Shoutcast server that then turns
 * around and streams it out to any connected listeners.
 *
 * [...]
 *
 * The initial request from the MP3 client to the Shoutcast server is
 * formatted as a normal HTTP request. In response, the Shoutcast server
 * sends an ICY response that looks like an HTTP response except with
 * the string "ICY"2 in place of the normal HTTP version string and with
 * different headers. After sending the headers and a blank line, the
 * server streams a potentially endless amount of MP3 data.
 *
 * The only tricky thing about the Shoutcast protocol is the way
 * metadata about the songs being streamed is embedded in the data sent
 * to the client. The problem facing the Shoutcast designers was to
 * provide a way for the Shoutcast server to communicate new title
 * information to the client each time it started playing a new song so
 * the client could display it in its UI [because] the MP3 format
 * doesn't make any provision for encoding metadata.
 *
 * While one of the design goals of ID3v2 had been to make it better
 * suited for use when streaming MP3s, the Nullsoft folks decided to go
 * their own route and invent a new scheme that's fairly easy to
 * implement on both the client side and the server side. That, of
 * course, was ideal for them since they were also the authors of their
 * own MP3 client.
 *
 * Their scheme was to simply ignore the structure of MP3 data and embed
 * a chunk of self-delimiting metadata every n bytes. The client would
 * then be responsible for stripping out this metadata so it wasn't
 * treated as MP3 data. Since metadata sent to a client that isn't ready
 * for it will cause glitches in the sound, the server is supposed to
 * send metadata only if the client's original request contains a
 * special Icy-Metadata header. And in order for the client to know how
 * often to expect metadata, the server must send back a header
 * Icy-Metaint whose value is the number of bytes of MP3 data that will
 * be sent between each chunk of metadata.
 *
 * The basic content of the metadata is a string of the form
 * "StreamTitle='title';" where title is the title of the current song
 * and can't contain single quote marks. This payload is encoded as a
 * length-delimited array of bytes: a single byte is sent indicating how
 * many 16-byte blocks follow, and then that many blocks are sent. They
 * contain the string payload as an ASCII string, with the final block
 * padded out with null bytes as necessary.
 *
 * Thus, the smallest legal metadata chunk is a single byte, zero,
 * indicating zero subsequent blocks. If the server doesn't need to
 * update the metadata, it can send such an empty chunk, but it must
 * send at least the one byte so the client doesn't throw away actual
 * MP3 data.
 *      
 *      Peter Siebel, ``Practical, A Shoutcast Server'' from the book
 *      Practical Common Lisp.
 *      http://www.gigamonkeys.com/book/practical-a-shoutcast-server.html
 *
 * */

namespace stream0x
{
  namespace mp3
  {
    using std::endl;
    using std::cout;
    bool is_mp3(boost::filesystem3::path candidate)
    {
      // Using file(1) would be appropriate here.  However, in my experience
      // mp3 files often have mimetype application/octet rather than
      // audio/mpeg.  Nevertheless, there may still be more complicated,
      // better ways of doing this.
      return candidate.has_extension() && (candidate.extension() == ".mp3");
    }

    //
    // FIXME It would be nice to use auto here, but I got a complier
    // error about late binding. 
    tuple<unsigned int, string> info(string mp3_path)
    {
      // warning, here be some ugly I/O code...
      //
      ifstream mp3;
      mp3.open(mp3_path);
      if(!mp3.is_open()) { cerr << "Couldn't open " << mp3_path << endl; }

      // Figure out where in the file the mp3 data starts
      unsigned int start = 0;
      scoped_array<char> id3_data(new char[3]);
      mp3.read(id3_data.get(), 3);
      string id3 = id3_data.get();
      if(id3.substr(0, 3) == "ID3")
      { 
        // 
        scoped_array<char> L_data(new char[4]);
        mp3.seekg(6);
        mp3.read(L_data.get(), 4);
        string sd = L_data.get();
        vector<unsigned char> suc;
        suc.resize(4);
        for(int i=0; i < 4; ++i) { suc[i] = *(L_data.get()+i); }
        vector<unsigned int> b2b = bytes2bin(suc, 7);
        start = bin2dec(b2b) + 10;
      } // i.e., start is 0 if the first three chars aren't "ID3"


      // metadata calculation
      string padding = string("\x00", 16);
      // 28 is the number of static characters in the metadata string
      string title = mp3_path;
      string url = "http://cs.uchicago.edu/~tilton";

      unsigned int length = title.size() + url.size() + 28;
      unsigned int pad    = 16 - (length % 16);
      stringstream metadata;
      metadata  << (char) ((length+pad)/16)
                << "StreamTitle='" << title << "';"
                << "StreamUrl='"   << url  << "';" 
                << padding.substr(0, pad);

      return tuple<unsigned int, string>{ start, metadata.str() };
    }
  }
}
