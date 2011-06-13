// utils.hpp // final project // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#ifndef STREAM0X_UTILS_H
#define STREAM0X_UTILS_H

#include <algorithm>
#include <locale>
#include <numeric>
#include <vector>
#include <sstream>
#include <string>

#include <curl/curl.h> // FIXME
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/join.hpp>

namespace stream0x
{
  namespace utils
  {
    using std::accumulate;
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::for_each;
    using std::locale;
    using std::ostringstream;
    using std::reverse;
    using std::string;
    using std::vector;

    using boost::lexical_cast;
    using boost::algorithm::join;
    namespace bpt = boost::posix_time;
    using bpt::time_facet;
    using bpt::second_clock;


    string escape(const string data);
    string unescape(const string data);
    unsigned int bin2dec(std::vector<unsigned int>& input);
    vector<unsigned int> bytes2bin(vector<unsigned char>& vb, unsigned int sz);
    string http_time();

    // Build GET strings out of variadic arguments.
    template <typename Head>
    string _build_get_path(Head& head)
    {
      return string(head);
    }

    template <typename Head, typename... Tail>
    string _build_get_path(Head& head, Tail&... tail)
    {
      // must enqueue list[1..] because it may include slashes, which are
      // a delimiter.
      return string(head) + "/" + escape(_build_get_path(tail...));
    }

    template <typename... Args>
    string build_get_path(Args&... args)
    {
      return "/" + _build_get_path(args...);
    }

    template <typename... Args>
    string get(Args&... args)
    {
      vector<string> s = {
        "GET",
        build_get_path(args...), 
        "HTTP/1.1", 
        "\n"
      };
      return string(join(s, " "));
    }
  }

}

#endif
