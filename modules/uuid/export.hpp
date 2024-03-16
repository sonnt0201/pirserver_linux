#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

#pragma once

// ID as UUID version 4 std::string
typedef std::string ID;

inline std::string uuid_v4() {
    boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();

  // Convert the UUID to a string in the standard format
  std::string uuid_str = boost::uuids::to_string(uuid);

  return uuid_str;
}