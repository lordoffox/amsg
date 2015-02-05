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
  struct msg_header_fix
  {
    boost::int32_t size;
    boost::int32_t type;
  };

  struct msg_header
  {
    boost::int32_t size;
    boost::int32_t type;
  };
}

AMSG(usr::msg_header_fix, (size&sfix)(type&sfix));
AMSG(usr::msg_header, (size)(type));

int main()
{
  try
  {
    usr::msg_header_fix src_fix;
    src_fix.size = 4;
    src_fix.type = 1;

    std::size_t size = amsg::size_of(src_fix);
    BOOST_ASSERT(size == sizeof(boost::int32_t) * 2);

    usr::msg_header src;
    src.size = 4;
    src.type = 1;

    size = amsg::size_of(src);
    BOOST_ASSERT(size == sizeof(boost::int8_t) * 2);
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
