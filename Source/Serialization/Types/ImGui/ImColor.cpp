#include "../../Serializer.hpp"

#include "xorstr.hpp"

void Serialization::Serializer::Serialize(const char* name, ImColor& type, Direction direction)
{
	switch (direction) {
	case SERIALIZE:
		GetValue(name) = std::to_string(type.Value.x) + xorstr_("|") + std::to_string(type.Value.y) + xorstr_("|") + std::to_string(type.Value.z) + xorstr_("|") + std::to_string(type.Value.w);
		break;
	case DESERIALIZE:
		const std::string str = GetValue(name);
		if (!str.empty()) {
			char* saveptr {};
			char* cstr = const_cast<char*>(str.c_str());
			type.Value.x = std::stof(strtok_r(cstr, xorstr_("|"), &saveptr));
			type.Value.y = std::stof(strtok_r(nullptr, xorstr_("|"), &saveptr));
			type.Value.z = std::stof(strtok_r(nullptr, xorstr_("|"), &saveptr));
			type.Value.w = std::stof(strtok_r(nullptr, xorstr_("|"), &saveptr));
		}
		break;
	}
}
