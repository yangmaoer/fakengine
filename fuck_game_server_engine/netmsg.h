#pragma once

template <typename _buffer>
class netmsg
{
public:
	force_inline netmsg() : m_iter(0)
	{
	}
	force_inline ~netmsg()
	{
	}
public:
	force_inline void reset() const
	{
		m_iter = 0;
	}
	force_inline void write_int8(int8_t c, size_t num = 1)
	{
		m_buffer.insert(m_buffer.end(), num, c);
	}
	force_inline void write_int16(int16_t i)
	{
		write_buffer((int8_t*)&i, sizeof(i));
	}
	force_inline void write_int32(int32_t i)
	{
		write_buffer((int8_t*)&i, sizeof(i));
	}
	force_inline void write_int64(int64_t i)
	{
		write_buffer((int8_t*)&i, sizeof(i));
	}
	force_inline void write_str(const int8_t * str, size_t size = 0)
	{
		size_t len = strlen((const char *)str);
		if (size == 0)
		{
			size = len;
		}

		if (size <= len)
		{
			write_buffer(str, size);
		}
		else
		{
			write_buffer(str, len);
			write_int8(0, size - len);
		}
	}
	force_inline int8_t read_int8() const
	{
		int8_t i = 0;
		read_buffer((int8_t*)&i, sizeof(i));
		return i;
	}
	force_inline int16_t read_int16() const
	{
		int16_t i = 0;
		read_buffer((int8_t*)&i, sizeof(i));
		return i;
	}
	force_inline int32_t read_int32() const
	{
		int32_t i = 0;
		read_buffer((int8_t*)&i, sizeof(i));
		return i;
	}
	force_inline int64_t read_int64() const
	{
		int64_t i = 0;
		read_buffer((int8_t*)&i, sizeof(i));
		return i;
	}
	force_inline void write_buffer(const int8_t * p, size_t size)
	{
		m_buffer.insert(m_buffer.end(), p, p + size);
	}
	force_inline void read_buffer(int8_t * p, size_t size) const
	{
		if (m_iter + size <= m_buffer.size())
		{
			memcpy(p, (const void *)&m_buffer[m_iter], size);
		}
		else
		{
			memset(p, 0, size);
		}
		m_iter += size;
	}
	force_inline size_t size()
	{
		return m_buffer.size();
	}
public:
	template <typename F>
	force_inline bool to_buffer(const F & f) const
	{
		const uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		// 写入包头
		if (!f((const int8_t*)&c, c_msg_header_size))
		{
			return false;
		}

		// 写入size
		size_t data_size = m_buffer.size();
		FASSERT(c_msg_size_size <= sizeof(size_t));
		FASSERT(data_size <= (1<<(c_msg_size_size * 8)));
		if (!f((int8_t*)&data_size, c_msg_size_size))
		{
			return false;
		}

		// 写入data
		if (!f((int8_t*)&m_buffer[0], data_size))
		{
			return false;
		}
		
		return true;
	}
	template <typename F>
	force_inline bool from_buffer(const F & f)
	{
		// 读出包头
		uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		if (!f((int8_t*)&c, c_msg_header_size))
		{
			return false;
		}
		if (c != c_msg_header)
		{
			return false;
		}

		// 读出size
		size_t data_size = 0;
		FASSERT(c_msg_size_size <= sizeof(size_t));
		if (!f((int8_t*)&data_size, c_msg_size_size))
		{
			return false;
		}

		// 读出data
		m_buffer.resize(data_size, 0);
		if (data_size > 0 && !f((int8_t*)&m_buffer[0], data_size))
		{
			return false;
		}

		return true;
	}
private:
	_buffer m_buffer;
	mutable size_t m_iter;
};

