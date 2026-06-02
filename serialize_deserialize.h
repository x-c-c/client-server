#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <netinet/in.h>
class serialize_deserialize
{
public:
	std::vector<uint8_t> serialize(const std::string& data);
	std::vector<uint8_t> deserialize(const std::string& data);
};
