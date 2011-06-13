// mp3.hpp // final project // cspp 51045 // tilton@cs.uchicago.edu // 20110528

#ifndef STREAM0X_MP3_H
#define STREAM0X_MP3_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>

#include "utils.hpp" // bin2dec(), bytes2bin()

namespace stream0x
{
  namespace mp3
  {
    using std::cerr;
    using std::endl;
    using std::ifstream;
    using std::ios;
    using std::string;
    using std::stringstream;
    using std::tuple;
    using std::vector;

    using boost::lexical_cast;
    using boost::scoped_array;

    using stream0x::utils::bin2dec;
    using stream0x::utils::bytes2bin;

    bool is_mp3(boost::filesystem3::path candidate);
    tuple<unsigned int, string> info(string mp3_path);
  }
}

#endif
