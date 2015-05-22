///
/// Copyright (c) 2012 Ning Ding (lordoffox@gmail.com)
///
/// Distributed under the Boost Software License, Version 1.0. (See accompanying
/// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///
/// See https://github.com/lordoffox/amsg for latest version.
///

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

AMSG(usr::person, (v.name&smax(30))(v.age&sfix)(v.married));

#define ENOUGH_SIZE 4096

namespace amsg
{
class base_ut
{
public:
  static void run()
  {
    std::cout << "base_ut begin." << std::endl;
    for (std::size_t i=0; i<test_count; ++i)
    {
      test_base();
      test_common();
      if (test_count > 1) std::cout << "\r" << i;
    }
    if (test_count > 1) std::cout << std::endl;
    std::cout << "base_ut end." << std::endl;
  }

private:
  template <typename T>
  static void test_basic(T src)
  {
    unsigned char buf[ENOUGH_SIZE];
    amsg::zero_copy_buffer buffer;
    buffer.set_write(buf, ENOUGH_SIZE);

    // serialize
    amsg::write(buffer, src);
    BOOST_ASSERT(!buffer.bad());

    // deserialize
    buffer.set_read(buf, ENOUGH_SIZE);
    T des = T();

    amsg::read(buffer, des);
    BOOST_ASSERT(!buffer.bad());

    BOOST_ASSERT(src == des);
  }

  static void test_base()
  {
    try
    {
      test_basic(true);
      test_basic(boost::int8_t(1));
      test_basic(boost::uint8_t(2));
      test_basic(boost::int16_t(3));
      test_basic(boost::uint16_t(4));
      test_basic(boost::int32_t(5));
      test_basic(boost::uint32_t(6));
      test_basic(boost::int64_t(-1003));
      test_basic(boost::uint64_t(9321));
      test_basic(float(3.1f));
      test_basic(double(323.232f));

      test_basic(std::string("hello world!"));
      test_basic(std::vector<boost::int32_t>(5, boost::int32_t(11)));
      test_basic(std::deque<boost::int32_t>(5, boost::int32_t(11)));
      test_basic(std::list<boost::int32_t>(5, boost::int32_t(11)));

      boost::array<boost::int32_t, 2> arr;
      arr.assign(23);
      test_basic(arr);

#ifdef AMSG_STD_CXX11
      std::array<boost::int32_t, 2> std_arr;
      std_arr[0] = 23;
      std_arr[1] = 23;
      test_basic(std_arr);
#endif

#ifdef AMSG_STD_CXX11
      test_basic(std::forward_list<boost::int32_t>(5, boost::int32_t(11)));
#endif

      std::map<boost::int32_t, std::string> m;
      m.insert(std::make_pair(boost::int32_t(121234), std::string("string")));
      test_basic(m);

      std::multimap<boost::int32_t, std::string> mm;
      mm.insert(std::make_pair(boost::int32_t(121234), std::string("string1")));
      mm.insert(std::make_pair(boost::int32_t(121234), std::string("string2")));
      test_basic(mm);

      boost::unordered_map<boost::int32_t, std::string> um;
      um.insert(std::make_pair(boost::int32_t(121234), std::string("string")));
      test_basic(um);

#ifdef AMSG_STD_CXX11
      std::unordered_map<boost::int32_t, std::string> std_um;
      std_um.insert(std::make_pair(boost::int32_t(121234), std::string("string")));
      test_basic(std_um);

      std::unordered_multimap<boost::int32_t, std::string> std_umm;
      std_umm.insert(std::make_pair(boost::int32_t(121234), std::string("string1")));
      std_umm.insert(std::make_pair(boost::int32_t(121234), std::string("string2")));
      test_basic(std_umm);
#endif

      std::set<std::string> s;
      s.insert(std::string("string"));
      test_basic(s);

      std::multiset<std::string> ms;
      ms.insert(std::string("string"));
      ms.insert(std::string("string"));
      test_basic(ms);

      boost::unordered_set<std::string> us;
      us.insert(std::string("string"));
      test_basic(us);

#ifdef AMSG_STD_CXX11
      std::unordered_set<std::string> std_us;
      std_us.insert(std::string("string"));
      test_basic(std_us);

      std::unordered_multiset<std::string> std_ums;
      std_ums.insert(std::string("string"));
      std_ums.insert(std::string("string"));
      test_basic(std_ums);
#endif
    }
    catch (std::exception& ex)
    {
      std::cerr << "test_base: " << ex.what() << std::endl;
    }
  }

  static void test_common()
  {
    try
    {
      unsigned char buf[ENOUGH_SIZE];

      // serialize
      usr::person src;
      src.name = "lordoffox";
      src.age = 33;
      src.married = true;

      amsg::zero_copy_buffer writer;
      writer.set_write(buf, ENOUGH_SIZE);
      amsg::write(writer, src);
      BOOST_ASSERT(!writer.bad());

      // deserialize
      usr::person des;

      amsg::zero_copy_buffer reader;
      reader.set_read(buf, ENOUGH_SIZE);
      amsg::read(reader, des);
      BOOST_ASSERT(!reader.bad());

      BOOST_ASSERT(src == des);
    }
    catch (std::exception& ex)
    {
      std::cerr << "test_common: " << ex.what() << std::endl;
    }
  }
};
}
