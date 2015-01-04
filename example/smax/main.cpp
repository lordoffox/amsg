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

  struct person2
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

AMSG(usr::person, (name&smax(5))(age)(married));
AMSG(usr::person2, (name)(age)(married));

#define ENOUGH_SIZE 4096

int main()
{
  try
  {
    unsigned char buf[ENOUGH_SIZE];

    // serialize
    usr::person src;
    src.name = "lordoffox";
    src.age = 33;
    src.married = true;

    amsg::zerocopy_buffer_t writer(buf, ENOUGH_SIZE);
    amsg::write(writer, src);
    BOOST_ASSERT(writer.bad());

    writer.clear();
    usr::person2 src2;
    src2.name = "lordoffox";
    src2.age = 33;
    src2.married = true;
    amsg::write(writer, src2);
    BOOST_ASSERT(!writer.bad());

    usr::person des;

    amsg::zerocopy_buffer_t reader(buf, ENOUGH_SIZE);
    amsg::read(reader, des);
    BOOST_ASSERT(reader.bad());
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
