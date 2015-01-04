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

static std::size_t const test_count = 1;

#include "test_base.hpp"

int main()
{
  try
  {
    amsg::base_ut::run();
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
