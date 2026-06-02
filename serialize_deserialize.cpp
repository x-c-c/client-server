#include "serialize_deserialize.h"

std::vector<uint8_t> serialize_deserialize::serialize(const std::string &data)
{
	std::vector<uint8_t> body;
	uint32_t dataLen = static_cast<uint32_t>(data.size());
	uint32_t littleEndianDataLen = htonl(dataLen);
	
	uint8_t* lenBytesBegin = reinterpret_cast<uint8_t*>(&littleEndianDataLen);
	uint8_t* lenBytesEnd = lenBytesBegin + sizeof(littleEndianDataLen);
	
	body.insert(body.end(), lenBytesBegin, lenBytesEnd);
	
	body.insert(body.end(), data.begin(), data.end());
	return body;
}



std::vector<uint8_t> serialize_deserialize::deserialize(const std::string &data)
{
	
}
