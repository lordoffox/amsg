AMSG v1.1
=======

AMSG is an C++ serialization library.

Features Overview
---------------

* Lightweight, fast and efficient serialization implementations
* Only one macro to register C++ struct, one line code that's all
* Header only, no need to build library, just three hpp files
* Support C++03 and C++11 (Need AMSG_STD_CXX11 macro)
* Default support C++ built-in types and all std containters (include C++11's)

What is AMSG?
---------------

```cpp
struct person
{
  std::string  name;
  int          age;

  bool operator==(person const& rhs) const
  {
    return name == rhs.name && age == rhs.age;
  }
};

AMSG(person, (name)(age));

#define ENOUGH_SIZE 4096
unsigned char buf[ENOUGH_SIZE];

// serialize
person src;
src.name = "lordoffox"
src.age = 33

amsg::zero_copy_buffer writer;
writer.set_write(buf, ENOUGH_SIZE);
amsg::write(writer, src);
assert(!writer.bad());

// deserialize
person des;

amsg::zero_copy_buffer reader;
reader.set_read(buf, ENOUGH_SIZE);
amsg::read(reader, des);
assert(!reader.bad());

assert(src == des);
```

Dependencies
------------

* CMake 2.8 and newer
* Boost 1.55.0 and newer (Header only)

Supported Compilers
-------------------

* GCC >= 4.6
* VC >= 9.0 (sp1)

Support C++11
-------------------

Define AMSG_STD_CXX11 in your project or just before include amsg hpps

```cpp
#define AMSG_STD_CXX11
#include <amsg/all.hpp>

// C++11's forward_list
#include <forward_list>

std::forward_list<int> fwd_list = {1,2,3,4,5};
unsigned char buf[4096];
amsg::zero_copy_buffer writer;
writer.set_write(buf, 4096);
amsg::write(writer, fwd_list);
assert(!writer.bad());
```

amsg::size_of
-------------------

Using amsg::size_of to get object's serialize size

```cpp
struct person
{
  std::string  name;
  int          age;

  bool operator==(person const& rhs) const
  {
    return name == rhs.name && age == rhs.age;
  }
};

AMSG(person, (name)(age));

person obj;
std::size_t size = amsg::size_of(obj);
std::cout << "person's serialize size: " << size << std::endl;
```

smax
-------------------

Sometimes you want limit max size of an array of string:

```cpp
struct person
{
  std::string  name;
  int          age;

  bool operator==(person const& rhs) const
  {
    return name == rhs.name && age == rhs.age;
  }
};

AMSG(person, (name&smax(30))(age)); // smax(30) limit name string max size is 30 bytes
```

sfix
-------------------

Sometimes you want serialization size to be fixed:

```cpp
struct person
{
  std::string    name;
  boost::int32_t age;

  bool operator==(person const& rhs) const
  {
    return name == rhs.name && age == rhs.age;
  }
};

AMSG(person, (name)(age&sfix)); // sfix enforce age (int32_t) to be 4bytes after serialization
```

If no sfix, age will dependence its value:
  -128 to 127                     --> 1byte
  -32,768 to 32,767               --> 2byte
  -2,147,483,648 to 2,147,483,647 --> 4byte

note: sfix only effect built-in types(int, short, long, char, float, double and so on).

delete
------------------------
Time flies. Sometimes you want delete some field from your struct, you can do like this.
struct person
{
  boost::int32_t age;

  bool operator==(person const& rhs) const
  {
    return age == rhs.age;
  }
};

AMSG(person, (del_string_t)(age&sfix)); // now field name is gone but we still can access old version data and got right value for field age.
and also:
del_int_t
del_enum_t
del_fix_int8_t
del_fix_int16_t
del_fix_int32_t
del_fix_int64_t
del_float_t
del_double_t
del_string_t
del_t<list_t<T>>
del_t<map_t<K,V>>
do there job for you.

Change list: V1.1:

  add forwards and backwards compatibility.

  add delete field.