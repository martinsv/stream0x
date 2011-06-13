// utils.cpp // stream0x // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#include "utils.hpp"

namespace stream0x
{
  namespace utils
  {
    string escape(const string data)
    {
      // curl_escape is now deprecated, but the cluster curl is too old,
      // so we'll continue to use it here.
      CURL *handle = curl_easy_init();
      char *tmp = curl_easy_escape(handle, data.c_str(), 0);
      string escaped_data = string(tmp);
      curl_free(tmp);
      return escaped_data;
    }

    string unescape(const string data)
    {
      CURL *handle = curl_easy_init();
      char *tmp = curl_easy_unescape(handle, data.c_str(), 0, NULL);
      string escaped_data = string(tmp);
      curl_free(tmp);
      return escaped_data;
    }

    // convert array of bits to decimal value
    unsigned int bin2dec(vector<unsigned int>& input)
    {
      int multi = 1;
      reverse(input.begin(), input.end());
      unsigned long value = accumulate(input.begin(), input.end(), 0, [&](unsigned int total, unsigned int b){
          unsigned retval = total += b * multi;
          multi = multi * 2;
          return retval;
      });
      return value;
    }

    /* 
     * Accepts a string of bytes (chars) and returns an array of bits
     * representing the bytes in big endian byte (Most significant byte/bit
     * first) order.  Each byte can have its higher bits ignored by passing
     * an sz arg.
     *
     */
    vector<unsigned int> bytes2bin(vector<unsigned char>& vb, unsigned int sz = 8)
    {
      if((sz < 1) || (sz > 8))
      {
        cerr << "Invalid sz value " << lexical_cast<string>(sz) << endl;
      }

      vector<unsigned int> rs;
      for_each(vb.begin(), vb.end(), [&](unsigned int b) {
          vector<unsigned int> bits;
          int ord = (int) b;
          while(ord > 0)
          { 
            bits.push_back(ord & 1);
            ord >>= 1;
          }
          size_t bs = bits.size();
          if(bs < sz)
          {
            for(int i=0; i < (sz - bs); ++i)
            {
              bits.push_back(0);
            }
          }
          else if(bs > sz)
          {
            bits.resize(8);
          }
          // big endian
          reverse(bits.begin(), bits.end());
          rs.insert(rs.end(), bits.begin(), bits.end());
      });

      if(rs.size() == 0) { rs.push_back(0); }
      return rs;
    }

    string http_time()
    {
      static char const* const fmt = "%a, %d %b %Y %H:%M:%S GMT";
      ostringstream ss;
      ss.imbue(locale(cout.getloc(), new time_facet(fmt)));
      ss << second_clock::universal_time();
      return ss.str();
    }

  }
}
