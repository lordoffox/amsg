///
/// Copyright (c) 2012 - 2015 Ning Ding (lordoffox@gmail.com)
///
/// Distributed under the Boost Software License, Version 1.0. (See accompanying
/// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///
/// See https://github.com/lordoffox/amsg for latest version.
///

#ifndef AMSG_AMSG_HPP
#define AMSG_AMSG_HPP

#include <stdint.h>
#include <string>
#include <deque>
#include <list>
#include <vector>
#include <map>
#include <limits>
#include <type_traits>
#include <array>
#include <forward_list>
#include <unordered_map>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/facilities/empty.hpp>

#ifndef AMSG_INLINE
# ifdef _MSC_VER
#  pragma inline_recursion(on)
#  define AMSG_INLINE __forceinline
# elif defined(_GUNC_)
#  define AMSG_INLINE inline __attribute__((always_inline))
# else
#  define AMSG_INLINE inline
# endif
#endif

namespace amsg
{
#if _MSC_VER > 1300
#include <intrin.h>

#define byte_swap_16(x) _byteswap_ushort(x)
#define byte_swap_32(x) _byteswap_ulong(x)
#define byte_swap_64(x) _byteswap_uint64(x)

#elif __GNUC__ >= 3
#include <byteswap.h>

#define byte_swap_16(x) bswap_16(x)
#define byte_swap_32(x) bswap_32(x)
#define byte_swap_64(x) bswap_64(x)
#else
  template<typename _ty>
  AMSG_INLINE _ty byte_swap_16(_ty value)
  {
    value = (value >> 8) | (value << 8);
    return value;
  }

  template<typename _ty>
  AMSG_INLINE _ty byte_swap_32(_ty value)
  {
    value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0x00FF00FF);
    value = (value >> 16) | (value << 16);
    return value;
  }

  template<typename _ty>
  AMSG_INLINE _ty byte_swap_64(_ty value)
  {
    value = ((value << 8) & 0xFF00FF00FF00FF00ULL) | ((value >> 8) & 0x00FF00FF00FF00FFULL);
    value = ((value << 16) & 0xFFFF0000FFFF0000ULL) | ((value >> 16) & 0x0000FFFF0000FFFFULL);
    return (value >> 32) | (value << 32);
  }

#endif

  //define endian check macro

#ifdef _WIN32
#else
#include <arpa/inet.h>  /* __BYTE_ORDER */
#endif

#	if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#		if __BYTE_ORDER == __LITTLE_ENDIAN
#			define __LITTLE_ENDIAN__
#		elif __BYTE_ORDER == __BIG_ENDIAN
#			define __BIG_ENDIAN__
#		elif _WIN32
#			define __LITTLE_ENDIAN__
#		endif
#	endif

#if defined(__LITTLE_ENDIAN__)
#define host_to_little_endian16(value) (value)
#define host_to_little_endian32(value) (value)
#define host_to_little_endian64(value) (value)
#define little_endian_to_host16(value) (value)
#define little_endian_to_host32(value) (value)
#define little_endian_to_host64(value) (value)
#else
#define host_to_little_endian16(value) byte_swap_16(value)
#define host_to_little_endian32(value) byte_swap_32(value)
#define host_to_little_endian64(value) byte_swap_64(value)
#define little_endian_to_host16(value) byte_swap_16(value)
#define little_endian_to_host32(value) byte_swap_32(value)
#define little_endian_to_host64(value) byte_swap_64(value)
#endif

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<sizeof(value_type) == 1, value_type>::type
    le_to_host(const value_type& value)
  {
    return value;
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<sizeof(value_type) == 2, value_type>::type
    le_to_host(const value_type& value)
  {
    return (value_type)little_endian_to_host16(value);
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<sizeof(value_type) == 4, value_type>::type
    le_to_host(const value_type& value)
  {
    return (value_type)little_endian_to_host32((uint32_t)value);
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<sizeof(value_type) == 8, value_type>::type
    le_to_host(const value_type& value)
  {
    return (value_type)little_endian_to_host64((uint64_t)value);
  }

  AMSG_INLINE float le_to_host(const float& value)
  {
    union { uint32_t uv; float fv; }u;
    u.fv = value;
    u.uv = little_endian_to_host32(u.uv);
    return u.fv;
  }

  AMSG_INLINE double le_to_host(const double& value)
  {
    union { uint32_t uv; double fv; }u;
    u.fv = value;
    u.uv = little_endian_to_host64(u.uv);
    return u.fv;
  }

  template<typename value_type,
    typename ::std::enable_if<sizeof(value_type) == 1, int >::type = 0
  >
  AMSG_INLINE value_type host_to_le(const value_type& value)
  {
    return value;
  }

  template<typename value_type,
    typename ::std::enable_if<sizeof(value_type) == 2, int >::type = 0
  >
  AMSG_INLINE value_type host_to_le(const value_type& value)
  {
    return (value_type)host_to_little_endian16(value);
  }

  template<typename value_type,
    typename ::std::enable_if<sizeof(value_type) == 4, int >::type = 0
  >
  AMSG_INLINE value_type host_to_le(const value_type& value)
  {
    return (value_type)host_to_little_endian32((uint32_t)value);
  }

  template<typename value_type,
    typename ::std::enable_if<sizeof(value_type) == 8, int >::type = 0
  >
  AMSG_INLINE value_type host_to_le(const value_type& value)
  {
    return (value_type)host_to_little_endian64((uint64_t)value);
  }

  AMSG_INLINE float host_to_le(const float& value)
  {
    union { uint32_t uv; float fv; }u;
    u.fv = value;
    u.uv = host_to_little_endian32(u.uv);
    return u.fv;
  }

  AMSG_INLINE double host_to_le(const double& value)
  {
    union { uint32_t uv; double fv; }u;
    u.fv = value;
    u.uv = host_to_little_endian64(u.uv);
    return u.fv;
  }

  static size_t to_str(const uint32_t& Value, char * resultbuffer, size_t len)
  {
    uint32_t temp = Value;
    resultbuffer[len - 1] = 0;
    size_t pos = len - 2;
    if (temp == 0)
    {
      resultbuffer[pos--] = '0';
    }
    while (temp)
    {
      resultbuffer[pos--] = (char)((temp % 10) + '0');
      temp = temp / 10;
    }
    ++pos;
    memmove(resultbuffer, resultbuffer + pos, (len - pos));
    return len - pos;
  }

  enum error_code_t
  {
    success = 0,
    negative_assign_to_unsigned_integer_number,
    value_too_large_to_integer_number,
    sequence_length_overflow,
    stream_buffer_overflow,
    number_of_element_not_macth
  };

  struct basic_store
  {

    basic_store()
      : m_error_code(success)
    {

    }

    AMSG_INLINE error_code_t error_code() const
    {
      return m_error_code;
    }

    AMSG_INLINE void	set_error_code(error_code_t value)
    {
      m_error_code = value;
    }

    AMSG_INLINE const char * message()
    {
      switch (m_error_code)
      {
      case success:
        break;
      case negative_assign_to_unsigned_integer_number:
        return "can't assign negative number to unsigned integer number.";
      case value_too_large_to_integer_number:
        return "value too large to integer number";
      case sequence_length_overflow:
        return "sequence length overflow";
      case stream_buffer_overflow:
        return "stream buffer overflow";
      case number_of_element_not_macth:
        return "number of element not macth";
      default:
        break;
      }
      return "";
    }

    AMSG_INLINE bool error() const // true if error
    {
      return m_error_code != success;
    }

    AMSG_INLINE void clear()
    {
      m_error_code = success;
    }

    error_code_t	m_error_code;
  };

  template< typename stream_ty, typename error_string_ty = ::std::string >
  struct store : public basic_store
  {
  public:
    store(stream_ty& stream)
      : basic_store()
      , m_stream(stream)
    {
    }

    AMSG_INLINE const error_string_ty& info() const { return this->m_error_info; }

    AMSG_INLINE void append_debug_info(const char * info)
    {
      this->m_error_info.append(info);
    }

    AMSG_INLINE bool bad()const { return this->m_stream.bad(); }

    AMSG_INLINE ::std::size_t read(char * buffer, ::std::size_t len)
    {
      return this->m_stream.read(buffer, len);
    }

    AMSG_INLINE ::std::size_t write(const char * buffer, ::std::size_t len)
    {
      return this->m_stream.write(buffer, len);
    }

    AMSG_INLINE void skip_read(::std::size_t len)
    {
      this->m_stream.seekg(len);
    }

    AMSG_INLINE ::std::size_t read_length()
    {
      return this->m_stream.tellg();
    }

    AMSG_INLINE ::std::size_t write_length()
    {
      return this->m_stream.tellp();
    }

    AMSG_INLINE void clear()
    {
      basic_store::clear();
      this->m_stream.clear();
      this->m_error_info.clear();
    }

  private:
    stream_ty&		m_stream;
    error_string_ty		m_error_info;
  };

  template<typename stream_ty>
  AMSG_INLINE store<stream_ty> make_store(stream_ty& stream)
  {
    return store<stream_ty>(stream);
  }

  enum
  {
    const_interger_byte_msak = 0x3f,
    const_negative_bit_value = 0x40,
    const_tag_as_value = 0x7f,
    const_tag_as_type,
    const_store_postive_integer_byte_mask = 0x80 - 2,
    const_store_negative_integer_byte_mask = 0x80 + const_negative_bit_value - 2
  };

  template <typename value_type>
  struct sfix_op
  {
    value_type& val;
    sfix_op(value_type& value)
      :val(value)
    {}
    sfix_op(const sfix_op& rv)
      :val(rv.val)
    {}

    sfix_op& operator = (const sfix_op& rv)
    {
      val = rv.val;
    }
  };

  struct sfix_def{};

  namespace
  {
    static sfix_def sfix = sfix_def();
  }

  template<typename ty>
  AMSG_INLINE sfix_op<ty> operator & (ty& value, const sfix_def&)
  {
    sfix_op<ty> op(value);
    return op;
  }

  template <typename value_type>
  struct smax_valid
  {
    uint32_t size;
    value_type & val;
    smax_valid(uint32_t max, value_type& value)
      :size(max), val(value)
    {}
    smax_valid(smax_valid& rv)
      :size(rv.size), val(rv.val)
    {}

    smax_valid& operator = (const smax_valid& rv)
    {
      size = rv.size;
      val = rv.val;
    }
  };

  struct smax
  {
    uint32_t size;
    smax(uint32_t max = 0)
      :size(max)
    {}
  };

  template<typename ty>
  AMSG_INLINE smax_valid<ty> operator & (ty& value, const smax& sm)
  {
    smax_valid<ty> valid(sm.size, value);
    return valid;
  }

  template<typename value_type>
  AMSG_INLINE bool can_skip(const value_type&)
  {
    return false;
  }

  AMSG_INLINE uint32_t size_of(bool)
  {
    return 1;
  }

  template<typename store_ty>
  AMSG_INLINE void skip_read(store_ty& store_data, bool*)
  {
    store_data.skip_read(1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    return;
  }

  template<typename store_ty>
  AMSG_INLINE void read(store_ty& store_data, bool& value)
  {
    store_data.read((char*)&value, 1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    return;
  }

  template<typename store_ty>
  AMSG_INLINE void write(store_ty& store_data, const bool& value)
  {
    uint8_t val = value ? 1 : 0;
    store_data.write((char*)&value, 1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    return;
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_signed<value_type>::value && ::std::is_integral<value_type>::value, uint32_t>::type
    size_of(const value_type& value)
  {
    if (0 <= value && value < const_tag_as_type)
    {
      return 1;
    }
    else
    {
      value_type temp = value;
      if (value < 0)
      {
        temp = -value;
      }
      if (temp < 0x100)
      {
        return 2;
      }
      else if (temp < 0x10000)
      {
        return 3;
      }
      else if (temp < 0x1000000)
      {
        return 4;
      }
      else if (temp < 0x100000000)
      {
        return 5;
      }
      else if (temp < 0x10000000000LL)
      {
        return 6;
      }
      else if (temp < 0x1000000000000LL)
      {
        return 7;
      }
      else if (temp < 0x100000000000000LL)
      {
        return 8;
      }
    }
    return 9;
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    skip_read(store_ty& store_data, value_type *)
  {
    uint8_t tag;
    store_data.read((char*)&tag, 1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    if (tag > const_tag_as_value)
    {
      int read_bytes = (tag & const_interger_byte_msak) + 1;
      store_data.skip_read(read_bytes);
      if (store_data.bad())
      {
        store_data.set_error_code(stream_buffer_overflow);
        return;
      }
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_signed<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value)
  {
    const int bytes = sizeof(value_type);
    value_type read_value[2] = { 0 };
    uint8_t * ptr = (uint8_t *)read_value;
    store_data.read((char*)ptr, 1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    value = *ptr;
    if (value > const_tag_as_value)
    {
      int sign = 1;
      if ((long)value & const_negative_bit_value)
      {
        sign = -1;
      }
      int read_bytes = (value & const_interger_byte_msak) + 1;
      if (bytes < read_bytes)
      {
        store_data.set_error_code(value_too_large_to_integer_number);
        return;
      }
      ptr = (uint8_t *)&read_value[1];
      store_data.read((char*)ptr, read_bytes);
      if (store_data.bad())
      {
        store_data.set_error_code(stream_buffer_overflow);
        return;
      }
      if (sign < 0)
      {
        value = -(value_type)le_to_host(read_value[1]);
      }
      else
      {
        value = le_to_host(read_value[1]);
      }
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_signed<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value)
  {
    value_type write_value[2] = { 0 };
    int write_bytes = 0;
    uint8_t * ptr = (uint8_t *)write_value;
    uint8_t tag = static_cast<uint8_t>(value);
    write_bytes = 1;
    if (0 <= value && value < const_tag_as_type)
    {
      write_value[0] = tag;
    }
    else
    {
      uint8_t negative_bit = 0;
      value_type temp = value;
      if (value < 0)
      {
        negative_bit = const_negative_bit_value;
        temp = -value;
      }
      write_value[1] = host_to_le(temp);
      ptr = (uint8_t *)(&write_value[1]) - 1;
      if (temp < 0x100)
      {
        write_bytes = 2;
      }
      else if (temp < 0x10000)
      {
        write_bytes = 3;
      }
      else if (temp < 0x1000000)
      {
        write_bytes = 4;
      }
      else if (temp < 0x100000000)
      {
        write_bytes = 5;
      }
      else if (temp < 0x10000000000LL)
      {
        write_bytes = 6;
      }
      else if (temp < 0x1000000000000LL)
      {
        write_bytes = 7;
      }
      else if (temp < 0x100000000000000LL)
      {
        write_bytes = 8;
      }
      else
      {
        write_bytes = 9;
      }
      *ptr = const_store_postive_integer_byte_mask + negative_bit + write_bytes;
    }
    store_data.write((const char *)ptr, write_bytes);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_unsigned<value_type>::value && ::std::is_integral<value_type>::value, uint32_t>::type
    size_of(const value_type& value)
  {
    if (value < const_tag_as_type)
    {
      return 1;
    }
    else
    {
      if (value < 0x100)
      {
        return 2;
      }
      else if (value < 0x10000)
      {
        return 3;
      }
      else if (value < 0x1000000)
      {
        return 4;
      }
      else if (value < 0x100000000)
      {
        return 5;
      }
      else if (value < 0x10000000000LL)
      {
        return 6;
      }
      else if (value < 0x1000000000000LL)
      {
        return 7;
      }
      else if (value < 0x100000000000000LL)
      {
        return 8;
      }
    }
    return 9;
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_unsigned<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value)
  {
    const int bytes = sizeof(value_type);
    value_type read_value[2] = { 0 };
    uint8_t * ptr = (uint8_t *)read_value;
    store_data.read((char*)ptr, 1);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    value = *ptr;
    if (value > const_tag_as_value)
    {
      if ((long)value & const_negative_bit_value)
      {
        store_data.set_error_code(negative_assign_to_unsigned_integer_number);
        return;

      }
      int read_bytes = int(value & const_interger_byte_msak) + 1;
      if (bytes < read_bytes)
      {
        store_data.set_error_code(value_too_large_to_integer_number);
        return;
      }
      ptr = (uint8_t *)&read_value[1];
      store_data.read((char*)ptr, read_bytes);
      if (store_data.bad())
      {
        store_data.set_error_code(stream_buffer_overflow);
        return;
      }
      value = le_to_host(read_value[1]);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_unsigned<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value)
  {
    value_type write_value[2] = { 0 };
    int write_bytes = 0;
    uint8_t * ptr = (uint8_t *)write_value;
    uint8_t tag = static_cast<uint8_t>(value);
    write_bytes = 1;
    if (value < const_tag_as_type)
    {
      write_value[0] = tag;
    }
    else
    {
      write_value[1] = host_to_le(value);
      ptr = (uint8_t *)(&write_value[1]) - 1;
      if (value < 0x100)
      {
        write_bytes = 2;
      }
      else if (value < 0x10000)
      {
        write_bytes = 3;
      }
      else if (value < 0x1000000)
      {
        write_bytes = 4;
      }
      else if (value < 0x100000000)
      {
        write_bytes = 5;
      }
      else if (value < 0x10000000000LL)
      {
        write_bytes = 6;
      }
      else if (value < 0x1000000000000LL)
      {
        write_bytes = 7;
      }
      else if (value < 0x100000000000000LL)
      {
        write_bytes = 8;
      }
      else
      {
        write_bytes = 9;
      }
      *ptr = (uint8_t)(const_store_postive_integer_byte_mask + write_bytes);
    }
    store_data.write((const char *)ptr, write_bytes);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_enum<value_type>::value, uint32_t>::type
    size_of(const value_type& value)
  {
    return size_of((int64_t)value);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_enum<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value)
  {
    int64_t data;
    read(store_data, data);
    value = static_cast<value_type>(data);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_enum<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value)
  {
    int64_t data = static_cast<int64_t>(value);
    write(store_data, data);
  }

  AMSG_INLINE uint32_t size_of(const float&)
  {
    return sizeof(float);
  }

  template<typename store_ty>
  AMSG_INLINE void skip_read(store_ty& store_data, float *)
  {
    store_data.skip_read(sizeof(float));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename store_ty>
  AMSG_INLINE void read(store_ty& store_data, float& value)
  {
    store_data.read((char*)&value, sizeof(float));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
    value = le_to_host(value);
  }

  template<typename store_ty>
  AMSG_INLINE void write(store_ty& store_data, const float& value)
  {
    float data = host_to_le(value);
    store_data.write((char*)&data, sizeof(double));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  AMSG_INLINE uint32_t size_of(const double&)
  {
    return sizeof(double);
  }

  template<typename store_ty>
  AMSG_INLINE void skip_read(store_ty& store_data, double *)
  {
    store_data.skip_read(sizeof(double));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename store_ty>
  AMSG_INLINE void read(store_ty& store_data, double& value)
  {
    store_data.read((char*)&value, sizeof(double));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
    value = le_to_host(value);
  }

  template<typename store_ty>
  AMSG_INLINE void write(store_ty& store_data, const double& value)
  {
    double data = host_to_le(value);
    store_data.write((char*)&data, sizeof(double));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename alloc_ty>
  AMSG_INLINE uint32_t size_of(const ::std::basic_string<char, ::std::char_traits<char>, alloc_ty>& value, uint32_t max = 0)
  {
    (max);
    uint32_t len = (uint32_t)value.length();
    return size_of(len) + len;
  }

  template<typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::basic_string<char, ::std::char_traits<char>, alloc_ty>& value)
  {
    return value.empty();
  }
  
  template<typename store_ty, typename alloc_ty>
  AMSG_INLINE void skip_read(store_ty& store_data, ::std::basic_string<char, ::std::char_traits<char>, alloc_ty>*, uint32_t max = 0)
  {
    (max);
    uint32_t len;
    read(store_data, len);
    if (store_data.error())
    {
      return;
    }
    store_data.skip_read(len);
  }

  template<typename store_ty, typename alloc_ty>
  AMSG_INLINE void read(store_ty& store_data, ::std::basic_string<char, ::std::char_traits<char>, alloc_ty>& value, uint32_t max = 0)
  {
    uint32_t len;
    read(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    value.resize(len);
    store_data.read((char*)value.data(), len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
  }

  template<typename store_ty, typename alloc_ty>
  void write(store_ty& store_data, const ::std::basic_string<char, ::std::char_traits<char>, alloc_ty>& value, uint32_t max = 0)
  {
    uint32_t len = (uint32_t)value.length();
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    write(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    store_data.write(value.data(), len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
  }

  template<typename type>
  struct is_sequence_container : public ::std::false_type{};

  template<typename type, typename alloc_type>
  struct is_sequence_container< ::std::deque<type, alloc_type> > : public ::std::true_type{};

  template<typename type, typename alloc_type>
  struct is_sequence_container< ::std::list<type, alloc_type> > : public ::std::true_type{};

  template<typename type, typename alloc_type>
  struct is_sequence_container< ::std::vector<type, alloc_type> > : public ::std::true_type{};

  template<typename type, typename alloc_type>
  struct is_sequence_container< ::std::forward_list<type, alloc_type> > : public ::std::true_type{};

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_sequence_container<value_type>::value, uint32_t>::type
    size_of(const value_type& value, uint32_t max = 0)
  {
    (max);
    uint32_t len = 0;
    uint32_t size = 0;
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i, ++len)
    {
      size += size_of(*i);
    }
    return size + size_of(len);
  }

//   template<typename value_type>
//   AMSG_INLINE
//     typename ::std::enable_if<is_sequence_container<value_type>::value, bool>::type
//     can_skip(const value_type& value)
//   {
//     return value.empty();
//   }

  template<typename value_type , typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::deque<value_type, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename value_type, typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::list<value_type, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename value_type, typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::vector<value_type, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename value_type, typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::forward_list<value_type, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_sequence_container<value_type>::value, void>::type
    skip_read(store_ty& store_data, value_type*, uint32_t max = 0)
  {
    (max);
    uint32_t len;
    read(store_data, len);
    if (store_data.error())
    {
      return;
    }
    for (uint32_t i = 0; i < len; ++i)
    {
      typename value_type::value_type* elem_value = nullptr;
      skip_read(elem_value);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_sequence_container<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value, uint32_t max = 0)
  {
    uint32_t len;
    read(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    value.resize(len);
    uint32_t c = 0;
    for (typename value_type::iterator i = value.begin(); i != value.end(); ++i, ++c)
    {
      typename value_type::value_type& elem_value = *i;
      read(store_data, elem_value);
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_sequence_container<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value, uint32_t max = 0)
  {
    uint32_t len = (uint32_t)value.size();
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    write(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    uint32_t c = 0;
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i, ++c)
    {
      const typename value_type::value_type& elem_value = *i;
      write(store_data, elem_value);
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
  }

  template<typename type>
  struct is_array : public ::std::false_type{};

  template<typename type, ::std::size_t size>
  struct is_array< ::std::array<type, size> > : public ::std::true_type{};

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_array<value_type>::value, uint32_t>::type
    size_of(const value_type& value, uint32_t max = 0)
  {
    (max);
    uint32_t size = (uint32_t)value.size();
    size = size_of(size);
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i)
    {
      size += size_of(*i);
    }
    return size;
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_array<value_type>::value, void>::type
    skip_read(store_ty& store_data, value_type*, uint32_t max = 0)
  {
    (max);
    uint32_t len;
    read(store_data, len);
    if (store_data.error())
    {
      return;
    }
    for (uint32_t i = 0; i < len; ++i)
    {
      typename value_type::value_type* elem_value = nullptr;
      skip_read(elem_value);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_array<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value, uint32_t max = 0)
  {
    uint32_t len;
    read(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    uint32_t c = 0;
    for (typename value_type::iterator i = value.begin(); i != value.end(); ++i, ++c)
    {
      typename value_type::value_type& elem_value = *i;
      read(store_data, elem_value);
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
    for (; c < len; ++c)
    {
      typename value_type::value_type* elem_value = nullptr;
      skip_read(store_data, elem_value);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_array<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value, uint32_t max = 0)
  {
    uint32_t len = (uint32_t)value.size();
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    write(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    uint32_t c = 0;
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i, ++c)
    {
      const typename value_type::value_type& elem_value = *i;
      write(store_data, elem_value);
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
  }

  template<typename type>
  struct is_unordered_container : public ::std::false_type{};

  template<typename key_ty, typename ty, typename cmp_ty, typename alloc_ty>
  struct is_unordered_container< ::std::map<key_ty, ty, cmp_ty, alloc_ty> > : public ::std::true_type{};

  template<typename key_ty, typename ty, typename cmp_ty, typename alloc_ty>
  struct is_unordered_container< ::std::unordered_map<key_ty, ty, cmp_ty, alloc_ty> > : public ::std::true_type{};

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_unordered_container<value_type>::value, uint32_t>::type
    size_of(const value_type& value, uint32_t max = 0)
  {
    (max);
    uint32_t len = 0;
    uint32_t size = 0;
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i, ++len)
    {
      size += size_of(i->first);
      size += size_of(i->second);
    }
    return size + size_of(len);
  }

//   template<typename value_type>
//   AMSG_INLINE
//     typename ::std::enable_if<is_unordered_container<value_type>::value, bool>::type
//     can_skip(const value_type value)
//   {
//     return value.empty();
//   }

  template<typename key_ty, typename ty, typename cmp_ty, typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::map<key_ty, ty, cmp_ty, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename key_ty, typename ty, typename cmp_ty, typename alloc_ty>
  AMSG_INLINE bool can_skip(const ::std::unordered_map<key_ty, ty, cmp_ty, alloc_ty>& value)
  {
    return value.empty();
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_unordered_container<value_type>::value, void>::type
    skip_read(store_ty& store_data, value_type*, uint32_t max = 0)
  {
    (max);
    uint32_t len;
    read(store_data, len);
    if (store_data.error())
    {
      return;
    }
    for (uint32_t i = 0; i < len; ++i)
    {
      typename value_type::value_type::first_type* value1 = nullptr;
      typename value_type::value_type::second_type* value2 = nullptr;
      skip_read(value1);
      skip_read(value2);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_unordered_container<value_type>::value, void>::type
    read(store_ty& store_data, value_type& value, uint32_t max = 0)
  {
    uint32_t len;
    read(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    for (uint32_t c = 0; c < len; ++c)
    {
      typename ::std::remove_const<value_type::value_type::first_type>::type value1;
      typename value_type::value_type::second_type value2;
      read(store_data, value1);
      if (!store_data.error())
      {
        read(store_data, value2);
        if (!store_data.error())
        {
          value.insert(std::make_pair(value1, value2));
        }
      }
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<is_unordered_container<value_type>::value, void>::type
    write(store_ty& store_data, const value_type& value, uint32_t max = 0)
  {
    uint32_t len = (uint32_t)value.size();
    if (max > 0 && max < len)
    {
      store_data.set_error_code(sequence_length_overflow);
      return;
    }
    write(store_data, len);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    uint32_t c = 0;
    for (typename value_type::const_iterator i = value.begin(); i != value.end(); ++i, ++c)
    {
      const typename value_type::value_type::first_type& value1 = i->first;
      const typename value_type::value_type::second_type& value2 = i->second;
      write(store_data, value1);
      if (!store_data.error())
      {
        write(store_data, value2);
      }
      if (store_data.error())
      {
        char buffer[64];
        to_str(c, buffer, 64);
        store_data.append_debug_info("[");
        store_data.append_debug_info(buffer);
        store_data.append_debug_info("]");
        return;
      }
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, uint32_t>::type
    size_of(const sfix_op<value_type>& value)
  {
    (value);
    return sizeof(value.val);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    skip_read(store_ty& store_data, sfix_op<value_type>&)
  {
    store_data.skip_read(sizeof(value_type));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    read(store_ty& store_data, const sfix_op<value_type>& value)
  {
    store_data.read((char*)&(value.val), sizeof(value_type));
    value.val = le_to_host(value.val);
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    write(store_ty& store_data, const sfix_op<value_type>& value)
  {
    value_type data = host_to_le(value.val);
    store_data.write((char*)&data, sizeof(value_type));
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
    }
  }

  template<typename value_type>
  AMSG_INLINE uint32_t size_of(const smax_valid<value_type>& value)
  {
    return size_of(value.val);
  }

  template<typename value_type>
  AMSG_INLINE bool can_skip(const smax_valid<value_type>& value)
  {
    return can_skip(value.val);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE void skip_read(store_ty& store_data, const smax_valid<value_type>& value)
  {
    skip_read(store_data, &value.val);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE void read(store_ty& store_data, const smax_valid<value_type>& value)
  {
    smax_valid<value_type> * ptr = (smax_valid<value_type>*)&value;
    read(store_data, ptr->val, ptr->size);
  }

  template<typename store_ty, typename value_type>
  AMSG_INLINE void write(store_ty& store_data, const smax_valid<value_type>& value)
  {
    write(store_data, value.val, value.size);
  }

}

#define AMSG_TAG_MEMBER_X( r ,v , elem ) \
  if(!can_skip(v.elem))	\
  {\
    tag |= mask;\
  }\
  mask <<= 1;

#define AMSG_SIZE_MEMBER_X( r ,v , elem ) \
  if(!can_skip(v.elem))	\
  {\
    tag |= mask;\
    size += ::amsg::size_of(v.elem);\
  }\
  mask <<= 1;

#define AMSG_READ_MEMBER( r , v , elem ) \
  if(tag&mask)\
  {\
	  ::amsg::read(store_data,v.elem);\
  	if(store_data.error())\
    {\
	  store_data.append_debug_info(".");\
	  store_data.append_debug_info(BOOST_PP_STRINGIZE(elem));\
	  return;\
    }\
  }\
  mask <<= 1;

#define AMSG_WRITE_MEMBER( r ,v , elem ) \
  if(tag&mask)\
  {\
    ::amsg::write(store_data, v.elem); \
    if (store_data.error())\
    {\
    store_data.append_debug_info("."); \
    store_data.append_debug_info(BOOST_PP_STRINGIZE(elem)); \
    return; \
    }\
  }\
  mask <<= 1;

#define AMSG(TYPE, MEMBERS)\
namespace amsg {\
AMSG_INLINE uint32_t size_of(const TYPE& value)\
{\
	uint32_t size = 0;\
  uint64_t tag = 0;\
  uint64_t mask = 1;\
	BOOST_PP_SEQ_FOR_EACH( AMSG_SIZE_MEMBER_X , value , MEMBERS ) \
  size += size_of(tag);\
  size += size_of(size + size_of(size));\
	return size;\
}\
\
template<typename store_ty>	\
AMSG_INLINE void read(store_ty& store_data, TYPE& value)\
{\
  ::std::size_t offset = store_data.read_length();\
  uint64_t tag = 0;\
  uint64_t mask = 1;\
  read(store_data,tag);\
  if(store_data.error()){return;}\
  int32_t len_tag = 0;\
  read(store_data, len_tag);\
  if (store_data.error()){return;}\
	BOOST_PP_SEQ_FOR_EACH( AMSG_READ_MEMBER , value , MEMBERS ) \
  if(len_tag >= 0)\
  {\
    ::std::size_t read_len = store_data.read_length() - offset;\
    ::std::size_t len = (::std::size_t)len_tag;\
    if (len > read_len) store_data.skip_read(len - read_len);\
  }\
}\
\
template<typename store_ty>	\
AMSG_INLINE void write(store_ty& store_data, const TYPE& value)\
{\
	uint32_t size = size_of(value);\
  uint64_t tag = 0;\
  uint64_t mask = 1;\
  BOOST_PP_SEQ_FOR_EACH( AMSG_TAG_MEMBER_X , value , MEMBERS ) \
  write(store_data,tag);\
  write(store_data, size);\
  mask = 1;\
	BOOST_PP_SEQ_FOR_EACH( AMSG_WRITE_MEMBER , value , MEMBERS ) \
}\
}

#define AMSGF(TYPE,X)	\
  template <typename ty=TYPE> friend uint32_t ::amsg::size_of(ty&);\
	template <typename store_ty,typename ty=TYPE> friend void ::amsg::read(store_ty&,ty&);\
	template <typename store_ty,typename ty=TYPE> friend void ::amsg::write(store_ty&,const ty&);

#endif
