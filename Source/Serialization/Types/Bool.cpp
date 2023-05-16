#include "Serialization/Serializer.hpp"

#include <algorithm>

#include "xorstr.hpp"

void Serialization::Serializer::Serialize(const char* name, bool& type, Direction direction)
{
	switch (direction) {
	case SERIALIZE:
		GetValue(name) = type ? xorstr_("true") : xorstr_("false");
		break;
	case DESERIALIZE:
		std::string string = GetValue(name);
		if (!string.empty()) {
			std::transform(string.begin(), string.end(), string.begin(), ::tolower);
			type = (string == xorstr_("true"));
		}
		break;
	}
}
