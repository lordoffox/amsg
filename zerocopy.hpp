///
/// Copyright (c) 2012 - 2015 Ning Ding (lordoffox@gmail.com)
///
/// Distributed under the Boost Software License, Version 1.0. (See accompanying
/// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///
/// See https://github.com/lordoffox/amsg for latest version.
///

#ifndef AMSG_ZEROCOPY_HPP
#define AMSG_ZEROCOPY_HPP

#include "amsg.hpp"

namespace amsg
{
  struct zero_copy_buffer : public basic_store
  {
  private:
    ::std::string		m_error_info;

    unsigned char const* m_read_header_ptr;
    unsigned char* m_write_header_ptr;
    unsigned char const* m_read_ptr;
    unsigned char* m_write_ptr;
    unsigned char const* m_read_tail_ptr;
    unsigned char const* m_write_tail_ptr;
    int							m_status;

  public:
    enum { good, read_overflow, write_overflow };

    zero_copy_buffer()
      : m_read_header_ptr(0)
      , m_write_header_ptr(0)
      , m_read_ptr(0)
      , m_write_ptr(0)
      , m_read_tail_ptr(0)
      , m_write_tail_ptr(0)
      , m_status(good)
    {
    }

    ~zero_copy_buffer()
    {
    }

    AMSG_INLINE void set_read(unsigned char const* buffer, ::std::size_t length)
    {
      this->m_read_header_ptr = buffer;
      this->m_read_ptr = this->m_read_header_ptr;
      this->m_read_tail_ptr = this->m_read_header_ptr + length;
      this->m_status = good;
    }

    AMSG_INLINE void set_read(char const* buffer, ::std::size_t length)
    {
      set_read((unsigned char const*)buffer, length);
    }

    AMSG_INLINE void set_write(unsigned char* buffer, ::std::size_t length)
    {
      this->m_write_header_ptr = buffer;
      this->m_write_ptr = this->m_write_header_ptr;
      this->m_write_tail_ptr = this->m_write_header_ptr + length;
      this->m_status = good;
    }

    AMSG_INLINE void set_write(char* buffer, ::std::size_t length)
    {
      set_write((unsigned char*)buffer, length);
    }

    void append_debug_info(const char * info)
    {
      m_error_info.append(info);
    }

    ::std::size_t read(char * buffer, ::std::size_t len)
    {
      if (this->m_read_ptr + len > this->m_read_tail_ptr)
      {
        this->m_status = read_overflow;
        return 0;
      }
      ::std::memcpy(buffer, this->m_read_ptr, len);
      this->m_read_ptr += len;
      return len;
    }

    unsigned char get_char()
    {
      if (this->m_read_ptr + 1 > this->m_read_tail_ptr)
      {
        m_status = read_overflow;
        return 0;
      }
      return *m_read_ptr++;
    }

    ::std::size_t write(const char * buffer, ::std::size_t len)
    {
      if (this->m_write_ptr + len > this->m_write_tail_ptr)
      {
        this->m_status = write_overflow;
        return 0;
      }
      ::std::memcpy((void*)this->m_write_ptr, buffer, len);
      this->m_write_ptr += len;
      return len;
    }

    bool bad(){ return m_status != good || basic_store::error(); }

    AMSG_INLINE unsigned char * append_write(::std::size_t len)
    {
      if (this->m_write_ptr + len > this->m_write_tail_ptr)
      {
        this->m_status = write_overflow;
        return 0;
      }
      unsigned char * append_ptr = this->m_write_ptr;
      this->m_write_ptr += len;
      return append_ptr;
    }

    AMSG_INLINE unsigned char const* skip_read(::std::size_t len)
    {
      if (this->m_read_ptr + len > this->m_read_tail_ptr)
      {
        this->m_status = read_overflow;
        return 0;
      }
      unsigned char const* ptr = this->m_read_ptr;
      this->m_read_ptr += len;
      return ptr;
    }

    AMSG_INLINE void clear_write()
    {
      basic_store::clear();
      this->m_write_ptr = this->m_write_header_ptr;
    }

    AMSG_INLINE void clear()
    {
      basic_store::clear();
      this->m_read_ptr = this->m_read_header_ptr;
      this->m_write_ptr = this->m_write_header_ptr;
      m_status = good;
    }

    AMSG_INLINE unsigned char const* read_ptr() const
    {
      return this->m_read_ptr;
    }

    AMSG_INLINE unsigned char* write_ptr() const
    {
      return this->m_write_ptr;
    }

    AMSG_INLINE unsigned char* write_data() const
    {
      return this->m_write_header_ptr;
    }

    AMSG_INLINE::std::size_t read_length() const
    {
      return this->m_read_ptr - this->m_read_header_ptr;
    }

    AMSG_INLINE::std::size_t write_length() const
    {
      return this->m_write_ptr - this->m_write_header_ptr;
    }
  };

  template<std::size_t size>
  struct le_pos;

#if defined(__LITTLE_ENDIAN__)
  template<std::size_t size>
  struct le_pos
  {
    enum
    {
      pos0,
      pos1,
      pos2,
      pos3,
      pos4,
      pos5,
      pos6,
      pos7,
    };
  };
#else
  template<>
  struct le_pos < 1 >
  {
    enum
    {
      pos7,
      pos6,
      pos5,
      pos4,
      pos3,
      pos2,
      pos1,
      pos0 = 0,
    };
  };
  template<>
  struct le_pos < 2 >
  {
    enum
    {
      pos7,
      pos6,
      pos5,
      pos4,
      pos3,
      pos2,
      pos1 = 0,
      pos0,
    };
  };

  template<>
  struct le_pos < 4 >
  {
    enum
    {
      pos7,
      pos6,
      pos5,
      pos4,
      pos3 = 0,
      pos2,
      pos1,
      pos0,
    };
  };

  template<>
  struct le_pos < 8 >
  {
    enum
    {
      pos7,
      pos6,
      pos5,
      pos4,
      pos3,
      pos2,
      pos1,
      pos0,
    };
  };
#endif

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_signed<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    read(zero_copy_buffer& stream, value_type& value)
  {
    const int bytes = sizeof(value_type);
    uint8_t tag = stream.get_char();
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    if (tag > const_tag_as_value)
    {
      int sign = 1;
      if ((long)tag & const_negative_bit_value)
      {
        sign = -1;
      }
      int read_bytes = (int(tag) & const_interger_byte_msak) + 1;
      if (bytes < read_bytes)
      {
        stream.m_error_code = value_too_large_to_integer_number;
        return;
      }
      value = 0;
      uint8_t const* read_ptr = stream.skip_read(read_bytes);
      if (stream.bad())
      {
        stream.m_error_code = stream_buffer_overflow;
        return;
      }
      uint8_t * value_ptr = (uint8_t *)&value;
      value = 0;
      switch (read_bytes)
      {
      case 8:value_ptr[7] = read_ptr[le_pos<bytes>::pos7];
      case 7:value_ptr[6] = read_ptr[le_pos<bytes>::pos6];
      case 6:value_ptr[5] = read_ptr[le_pos<bytes>::pos5];
      case 5:value_ptr[4] = read_ptr[le_pos<bytes>::pos4];
      case 4:value_ptr[3] = read_ptr[le_pos<bytes>::pos3];
      case 3:value_ptr[2] = read_ptr[le_pos<bytes>::pos2];
      case 2:value_ptr[1] = read_ptr[le_pos<bytes>::pos1];
      case 1:value_ptr[0] = read_ptr[le_pos<bytes>::pos0];
      }
      if (sign < 0)
      {
        value = -value;
      }
    }
    else
    {
      value = tag;
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_signed<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    write(zero_copy_buffer& stream, const value_type& value)
  {
    const int bytes = sizeof(value_type);
    int write_bytes = 1;
    uint8_t write_buff[12];
    if (0 <= value && value < const_tag_as_type)
    {
      write_buff[0] = (uint8_t)value;
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
      uint8_t * ptr = (uint8_t *)&temp;
      if (temp < 0x100)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_bytes = 2;
      }
      else if (temp < 0x10000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_bytes = 3;
      }
      else if (temp < 0x1000000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_bytes = 4;
      }
      else if (temp < 0x100000000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_buff[4] = ptr[le_pos<bytes>::pos3];
        write_bytes = 5;
      }
      else if (temp < 0x10000000000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_buff[4] = ptr[le_pos<bytes>::pos3];
        write_buff[5] = ptr[le_pos<bytes>::pos4];
        write_bytes = 6;
      }
      else if (temp < 0x1000000000000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_buff[4] = ptr[le_pos<bytes>::pos3];
        write_buff[5] = ptr[le_pos<bytes>::pos4];
        write_buff[6] = ptr[le_pos<bytes>::pos5];
        write_bytes = 7;
      }
      else if (temp < 0x100000000000000)
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_buff[4] = ptr[le_pos<bytes>::pos3];
        write_buff[5] = ptr[le_pos<bytes>::pos4];
        write_buff[6] = ptr[le_pos<bytes>::pos5];
        write_buff[7] = ptr[le_pos<bytes>::pos6];
        write_bytes = 8;
      }
      else
      {
        write_buff[1] = ptr[le_pos<bytes>::pos0];
        write_buff[2] = ptr[le_pos<bytes>::pos1];
        write_buff[3] = ptr[le_pos<bytes>::pos2];
        write_buff[4] = ptr[le_pos<bytes>::pos3];
        write_buff[5] = ptr[le_pos<bytes>::pos4];
        write_buff[6] = ptr[le_pos<bytes>::pos5];
        write_buff[7] = ptr[le_pos<bytes>::pos6];
        write_buff[8] = ptr[le_pos<bytes>::pos7];
        write_bytes = 9;
      }
      write_buff[0] = (uint8_t)(const_store_postive_integer_byte_mask + negative_bit + write_bytes);
    }
    stream.write((char*)write_buff, write_bytes);
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_unsigned<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    read(zero_copy_buffer& stream, value_type& value)
  {
    const int bytes = sizeof(value_type);
    value = stream.get_char();
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    if (value > const_tag_as_value)
    {
      if ((long)value & const_negative_bit_value)
      {
        stream.m_error_code = negative_assign_to_unsigned_integer_number;
        return;
      }
      int read_bytes = (int)(value & const_interger_byte_msak) + 1;
      if (bytes < read_bytes)
      {
        stream.m_error_code = value_too_large_to_integer_number;
        return;
      }
      uint8_t const* read_ptr = stream.skip_read(read_bytes);
      if (stream.bad())
      {
        stream.m_error_code = stream_buffer_overflow;
        return;
      }
      uint8_t * value_ptr = (uint8_t *)&value;
      value = 0;
      switch (read_bytes)
      {
      case 8:value_ptr[7] = read_ptr[le_pos<bytes>::pos7];
      case 7:value_ptr[6] = read_ptr[le_pos<bytes>::pos6];
      case 6:value_ptr[5] = read_ptr[le_pos<bytes>::pos5];
      case 5:value_ptr[4] = read_ptr[le_pos<bytes>::pos4];
      case 4:value_ptr[3] = read_ptr[le_pos<bytes>::pos3];
      case 3:value_ptr[2] = read_ptr[le_pos<bytes>::pos2];
      case 2:value_ptr[1] = read_ptr[le_pos<bytes>::pos1];
      case 1:value_ptr[0] = read_ptr[le_pos<bytes>::pos0];
      }
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_unsigned<value_type>::value && ::std::is_integral<value_type>::value, void>::type
    write(zero_copy_buffer& stream, const value_type& value)
  {
    const int bytes = sizeof(value_type);
    if (value < const_tag_as_type)
    {
      uint8_t * ptr = stream.append_write(1);
      *ptr = (uint8_t)value;
    }
    else
    {
      uint8_t * ptr = (uint8_t *)(&value);
      if (value < 0x100)
      {
        uint8_t * wptr = stream.append_write(2);
        wptr[0] = 0x80;
        wptr[1] = ptr[le_pos<bytes>::pos0];
      }
      else if (value < 0x10000)
      {
        uint8_t * wptr = stream.append_write(3);
        wptr[0] = 0x81;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
      }
      else if (value < 0x1000000)
      {
        uint8_t * wptr = stream.append_write(4);
        wptr[0] = 0x82;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
      }
      else if (value < 0x100000000)
      {
        uint8_t * wptr = stream.append_write(5);
        wptr[0] = 0x83;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
        wptr[4] = ptr[le_pos<bytes>::pos3];
      }
      else if (value < 0x10000000000LL)
      {
        uint8_t * wptr = stream.append_write(6);
        wptr[0] = 0x84;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
        wptr[4] = ptr[le_pos<bytes>::pos3];
        wptr[5] = ptr[le_pos<bytes>::pos4];
      }
      else if (value < 0x1000000000000LL)
      {
        uint8_t * wptr = stream.append_write(7);
        wptr[0] = 0x85;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
        wptr[4] = ptr[le_pos<bytes>::pos3];
        wptr[5] = ptr[le_pos<bytes>::pos4];
        wptr[6] = ptr[le_pos<bytes>::pos5];
      }
      else if (value < 0x100000000000000LL)
      {
        uint8_t * wptr = stream.append_write(8);
        wptr[0] = 0x86;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
        wptr[4] = ptr[le_pos<bytes>::pos3];
        wptr[5] = ptr[le_pos<bytes>::pos4];
        wptr[6] = ptr[le_pos<bytes>::pos5];
        wptr[7] = ptr[le_pos<bytes>::pos6];
      }
      else
      {
        uint8_t * wptr = stream.append_write(9);
        wptr[0] = 0x87;
        wptr[1] = ptr[le_pos<bytes>::pos0];
        wptr[2] = ptr[le_pos<bytes>::pos1];
        wptr[3] = ptr[le_pos<bytes>::pos2];
        wptr[4] = ptr[le_pos<bytes>::pos3];
        wptr[5] = ptr[le_pos<bytes>::pos4];
        wptr[6] = ptr[le_pos<bytes>::pos5];
        wptr[7] = ptr[le_pos<bytes>::pos6];
        wptr[8] = ptr[le_pos<bytes>::pos7];
      }
    }
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    read(zero_copy_buffer& store_data, const sfix_op<value_type>& value)
  {
    uint8_t * read_ptr = (uint8_t *)store_data.skip_read(sizeof(value_type));
    uint8_t * value_ptr = (uint8_t *)&value.val;
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    const int bytes = sizeof(value_type);
    switch (bytes)
    {
    case 8:value_ptr[7] = read_ptr[le_pos<bytes>::pos7];
    case 7:value_ptr[6] = read_ptr[le_pos<bytes>::pos6];
    case 6:value_ptr[5] = read_ptr[le_pos<bytes>::pos5];
    case 5:value_ptr[4] = read_ptr[le_pos<bytes>::pos4];
    case 4:value_ptr[3] = read_ptr[le_pos<bytes>::pos3];
    case 3:value_ptr[2] = read_ptr[le_pos<bytes>::pos2];
    case 2:value_ptr[1] = read_ptr[le_pos<bytes>::pos1];
    case 1:value_ptr[0] = read_ptr[le_pos<bytes>::pos0];
    }
  }

  template<typename value_type>
  AMSG_INLINE
    typename ::std::enable_if<::std::is_integral<value_type>::value, void>::type
    write(zero_copy_buffer& store_data, const sfix_op<value_type>& value)
  {
    uint8_t * write_ptr = store_data.append_write(sizeof(value_type));
    uint8_t * value_ptr = (uint8_t *)&value.val;
    if (store_data.bad())
    {
      store_data.set_error_code(stream_buffer_overflow);
      return;
    }
    const int bytes = sizeof(value_type);
    switch (bytes)
    {
    case 8:write_ptr[7] = value_ptr[le_pos<bytes>::pos7];
    case 7:write_ptr[6] = value_ptr[le_pos<bytes>::pos6];
    case 6:write_ptr[5] = value_ptr[le_pos<bytes>::pos5];
    case 5:write_ptr[4] = value_ptr[le_pos<bytes>::pos4];
    case 4:write_ptr[3] = value_ptr[le_pos<bytes>::pos3];
    case 3:write_ptr[2] = value_ptr[le_pos<bytes>::pos2];
    case 2:write_ptr[1] = value_ptr[le_pos<bytes>::pos1];
    case 1:write_ptr[0] = value_ptr[le_pos<bytes>::pos0];
    }
  }

  AMSG_INLINE void read(zero_copy_buffer& stream, float& value)
  {
    typedef float value_type;
    uint8_t const* read_ptr = stream.skip_read(sizeof(value_type));
    uint8_t * value_ptr = (uint8_t *)&value;
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    value_ptr[le_pos<4>::pos0] = read_ptr[0];
    value_ptr[le_pos<4>::pos1] = read_ptr[1];
    value_ptr[le_pos<4>::pos2] = read_ptr[2];
    value_ptr[le_pos<4>::pos3] = read_ptr[3];
  }

  AMSG_INLINE void write(zero_copy_buffer& stream, const float& value)
  {
    typedef float value_type;
    uint8_t * write_ptr = stream.append_write(sizeof(value_type));
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    uint8_t * value_ptr = (uint8_t *)&value;
    write_ptr[le_pos<4>::pos0] = value_ptr[0];
    write_ptr[le_pos<4>::pos1] = value_ptr[1];
    write_ptr[le_pos<4>::pos2] = value_ptr[2];
    write_ptr[le_pos<4>::pos3] = value_ptr[3];
  }

  AMSG_INLINE void read(zero_copy_buffer& stream, double& value)
  {
    typedef double value_type;
    uint8_t const* read_ptr = stream.skip_read(sizeof(value_type));
    uint8_t * value_ptr = (uint8_t *)&value;
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    value_ptr[le_pos<8>::pos0] = read_ptr[0];
    value_ptr[le_pos<8>::pos1] = read_ptr[1];
    value_ptr[le_pos<8>::pos2] = read_ptr[2];
    value_ptr[le_pos<8>::pos3] = read_ptr[3];
    value_ptr[le_pos<8>::pos4] = read_ptr[4];
    value_ptr[le_pos<8>::pos5] = read_ptr[5];
    value_ptr[le_pos<8>::pos6] = read_ptr[6];
    value_ptr[le_pos<8>::pos7] = read_ptr[7];
  }

  AMSG_INLINE void write(zero_copy_buffer& stream, const double& value)
  {
    typedef double value_type;
    uint8_t * write_ptr = stream.append_write(sizeof(value_type));
    if (stream.bad())
    {
      stream.m_error_code = stream_buffer_overflow;
      return;
    }
    uint8_t * value_ptr = (uint8_t *)&value;
    write_ptr[le_pos<8>::pos0] = value_ptr[0];
    write_ptr[le_pos<8>::pos1] = value_ptr[1];
    write_ptr[le_pos<8>::pos2] = value_ptr[2];
    write_ptr[le_pos<8>::pos3] = value_ptr[3];
    write_ptr[le_pos<8>::pos4] = value_ptr[4];
    write_ptr[le_pos<8>::pos5] = value_ptr[5];
    write_ptr[le_pos<8>::pos6] = value_ptr[6];
    write_ptr[le_pos<8>::pos7] = value_ptr[7];
  }

}

#endif
