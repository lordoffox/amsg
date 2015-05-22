///
/// Copyright (c) 2012 Ning Ding (lordoffox@gmail.com)
///
/// Distributed under the Boost Software License, Version 1.0. (See accompanying
/// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///
/// See https://github.com/lordoffox/amsg for latest version.
///

#include <amsg/all.hpp>
#include <boost/assert.hpp>
#include <iostream>

namespace usr
{
  struct person
  {
    std::string name;
    boost::int32_t age;
    bool married;

    bool operator==(person const& rhs) const
    {
      return name == rhs.name && age == rhs.age && married == rhs.married;
    }
  };
}

AMSG(usr::person, (v.name)(v.age)(v.married));

int main()
{
  try
  {
    usr::person src;
    src.name = "my name";
    src.age = 33;
    src.married = true;

    std::size_t size = amsg::size_of(src);
    std::cout << size << std::endl;
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
