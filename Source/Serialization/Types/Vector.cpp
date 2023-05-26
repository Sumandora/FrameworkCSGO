#include "../Serializer.hpp"

#include "xorstr.hpp"

void Serialization::Serializer::Serialize(const char* name, Vector& type, Direction direction)
{
	switch (direction) {
	case SERIALIZE:
		GetValue(name) = std::to_string(type.x) + xorstr_("|") + std::to_string(type.y) + xorstr_("|") + std::to_string(type.z);
		break;
	case DESERIALIZE:
		const std::string str = GetValue(name);
		if (!str.empty()) {
			char* saveptr{};
			char* cstr = const_cast<char*>(str.c_str());
			type.x = std::stof(strtok_r(cstr, xorstr_("|"), &saveptr));
			type.y = std::stof(strtok_r(nullptr, xorstr_("|"), &saveptr));
			type.z = std::stof(strtok_r(nullptr, xorstr_("|"), &saveptr));
		}
		break;
	}
}
