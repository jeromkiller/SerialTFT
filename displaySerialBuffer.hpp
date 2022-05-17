#pragma once

#include <stdint.h>
#include <vector>
#include <cstring>

#include "serialPacketBaseTypes.hpp"

class displaySerialBuffer
{
public:
	displaySerialBuffer();

	std::vector<uint8_t>& getBuffer();

	//add a byte to the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	add(const T& value)
	{
		//static_assert(T::type != void);
		const size_t oldSize = m_buffer.size();
		const size_t newSize = oldSize + sizeof(T);

		//i'd have preffered to perform the 'is_trivially_copyable'
		//check with an constexpr if, but those aren't available on arduino
		m_buffer.resize(newSize);
		memcpy(m_buffer.data() + oldSize, &value, sizeof(T));

		return true;
	}
	
	//add a buffer to the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	add_buffer(const T* buff_ptr, const uint16_t numItems)
	{
		if(buff_ptr == nullptr)
		{
			return false;
		}

		//add the buffer size before the buffer
		add(numItems);

		//then add the buffer to the 
		const size_t bufferSize = sizeof(T) * numItems;
		const size_t oldSize = m_buffer.size();
		const size_t newSize = oldSize + bufferSize;

		m_buffer.resize(newSize);
		memcpy(m_buffer.data() + oldSize, buff_ptr, bufferSize);

		return true;
	}

	//get the next value from the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get(T& value)
	{
		const void* p = m_buffer.data() + m_dataIndex;
		value = *reinterpret_cast<const T*>(p);
		m_dataIndex += sizeof(T);
		return true;
	}

	//get a value from the packet
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get(T& data, const uint8_t index)
	{
		m_dataIndex = index;
		return get(data);
	}

	//get a pointer to a sepperate buffer from the buffer
	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get_buffer_ptr(T** placepointer, uint8_t& num_elements)
	{
		//first get the number of items in the buffer
		get(num_elements);

		//then get the pointer to the buffer
		*placepointer = reinterpret_cast<T*>(m_buffer.data() + m_dataIndex);
		m_dataIndex += (num_elements * sizeof(T));
		return true;
	}

	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<T>::value, bool>::type
	get_buffer_ptr(T** placePointer, uint8_t& num_elements, uint8_t index)
	{
		//set the index and then just call the regular getbuffer function
		m_dataIndex = index;
		return get_buffer_ptr(placePointer, num_elements);
	}

	//set where from the packet we want to get data
	void setPacketIndex(uint16_t newIndex);

	//how long does the packet report to be
	uint16_t getPacketLength() const;
	//how many bytes are in the buffer
	uint16_t getBufferSize() const;
	//get the packetType
	serialPacketBaseTypes::packetType getPacketType() const;
	//have we recieved the full package
	bool fullPacketRecieved() const;
	//calculate the crc byte for the packet
	uint8_t calcCRCbyte();
	//validate if the packet we recieved is correct
	bool validatePacket();
	//clear the databuffer
	void clear();

private:
	std::vector<uint8_t> m_buffer;
	uint8_t m_dataIndex;

};
