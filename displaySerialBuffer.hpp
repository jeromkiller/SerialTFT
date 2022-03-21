#pragma once

#include <stdint.h>
#include <vector>
#include <cstring>

namespace serialPacketBaseTypes
{
	enum class packetType : uint8_t;
};

class displaySerialBuffer
{
public:
	displaySerialBuffer();

	std::vector<uint8_t>& getBuffer();

	//add a byte to the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	add(T data)
	{
		//static_assert(T::type != void);
		const size_t oldSize = m_buffer.size();
		const size_t newSize = oldSize + sizeof(T);

		//i'd have preffered to perform the 'is_trivially_copyable'
		//check with an constexpr if, but those aren't available on arduino
		m_buffer.resize(newSize);
		memcpy(m_buffer.data() + oldSize, &data, sizeof(T));

		return true;
	}

	//get a value from the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get(T* data, uint8_t index)
	{
		memcpy(data, m_buffer.data() + index, sizeof(T));
		m_dataIndex = index + sizeof(T);
		return true;
	}

	//
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get(T* data)
	{
		memcpy(data, m_buffer.data() + m_dataIndex, sizeof(T));
		m_dataIndex += sizeof(T);
		return true;
	}

	//set where from the packet we want to get data
	void setPacketIndex(uint8_t newIndex);

	//how long does the packet report to be
	uint8_t getPacketLength() const;
	//how many bytes are in the buffer
	uint8_t getBufferSize() const;
	//have we recieved the full package
	bool fullPacketRecieved() const;
	//calculate the crc byte for the packet
	uint8_t calcCRCbyte();
	//validate if the packet we recieved is correct
	bool validatePacket();

private:
	std::vector<uint8_t> m_buffer;
	uint8_t m_dataIndex;

};