#pragma once

#include <stdint.h>
#include "packetFlags.hpp"
#include "displaySerialBuffer.hpp"

template<typename T>
class packProperty
{
	private:
	T m_value;
	packetFlags& m_flags;
	const uint8_t m_flagId;

	public:
	//constructor
	packProperty(packetFlags& flags, const uint8_t flagId)
		: m_flags(flags)
		, m_flagId(flagId)
	{};

	const T& value() const
	{
		return m_value;
	}

	void setValue(const T& value)
	{
		m_flags.setFlag(m_flagId);
		m_value = value;
	}

	T& operator=(const T& value)
	{
		//guard against self assignment
		if(&m_value == &value)
		{
			return m_value;
		}

		setValue(value);

		return m_value;
	}

	operator T() const { return m_value; };

	//serialize direct value
	template<typename Q = T>
	typename std::enable_if<!(std::is_pointer<Q>::value), bool>::type
	serialize(displaySerialBuffer& buffer) const
	{
		if(m_flags.getFlag(m_flagId))
		{
			return buffer.add(m_value);
		}
		return true;
	}

	//serialize an array
	template<typename Q = T>
	typename std::enable_if<std::is_pointer<Q>::value, bool>::type
	serialize(displaySerialBuffer& buffer, uint8_t num_elements) const
	{
		if(m_flags.getFlag(m_flagId))
		{
			return buffer.add_buffer(m_value, num_elements);
		}
		return true;
	}

	//deserialize direct value
	template<typename Q = T>
	typename std::enable_if<!(std::is_pointer<Q>::value), bool>::type
	deserialize(displaySerialBuffer& buffer)
	{
		if(m_flags.getFlag(m_flagId))
		{
			return buffer.get(m_value);
		}
		return true;
	}

	//deserialize array, sets num_elements with the amount of items
	template<typename Q = T>
	typename std::enable_if<std::is_pointer<Q>::value, bool>::type
	deserialize(displaySerialBuffer& buffer, uint8_t& num_elements)
	{
		if(m_flags.getFlag(m_flagId))
		{
			return buffer.get_buffer_ptr(&m_value, num_elements);
		}
		return true;
	}

	//deserialize array
	template<typename Q = T>
	typename std::enable_if<std::is_pointer<Q>::value, bool>::type
	deserialize(displaySerialBuffer& buffer)
	{
		uint8_t dummy;
		return deserialize(buffer, dummy);
	}

	bool isSet() const
	{
		return m_flags.getFlag(m_flagId);
	}
};
