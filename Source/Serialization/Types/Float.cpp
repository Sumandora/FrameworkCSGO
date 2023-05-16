#include "Serialization/Serializer.hpp"

void Serialization::Serializer::Serialize(const char* name, float& type, Direction direction)
{
	switch (direction) {
	case SERIALIZE:
		GetValue(name) = std::to_string(type);
		break;
	case DESERIALIZE:
		const std::string string = GetValue(name);
		if (!string.empty())
			type = std::stof(string);
		break;
	}
}
