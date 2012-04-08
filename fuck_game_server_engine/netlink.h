#pragma once

// netlink 连接客户端类
template <typename _link>
class netlink
{
public:
	netlink()
	{
	}
	~netlink()
	{
	}
public:
	fore_inline bool ini(const net_link_param & param)
	{
		return m_link.ini(param);
	}
	fore_inline void tick()
	{
		m_link.tick();
	}
	template<typename _msg>
	fore_inline bool send_msg(const _msg & msg)
	{
		return m_link.send_msg(msg);
	}
	template<typename _msg>
	fore_inline bool recv_msg(_msg & msg)
	{
		return m_link.recv_msg(msg);
	}
private:
	_link m_link;
};

