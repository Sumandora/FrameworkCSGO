#ifndef SERIALIZATION_SERIALIZER
#define SERIALIZATION_SERIALIZER

#include "xorstr.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#include "json.hpp"
#pragma GCC diagnostic pop

#include <type_traits>

enum class Direction {
	SERIALIZE,
	DESERIALIZE
};

constexpr void Assign(auto& variable, json::JSON& jsonType)
{
	if constexpr (std::is_same_v<std::remove_cvref_t<decltype(variable)>, bool>)
		variable = jsonType.ToBool();
	else if constexpr (std::is_integral_v<std::remove_cvref_t<decltype(variable)>>)
		variable = jsonType.ToInt();
	else if constexpr (std::is_floating_point_v<std::remove_cvref_t<decltype(variable)>>)
		variable = jsonType.ToFloat();
	else if constexpr (std::is_same_v<std::remove_cvref_t<decltype(variable)>, const char*> || std::is_same_v<std::remove_cvref_t<decltype(variable)>, std::string>)
		variable = jsonType.ToString();
	else
		throw std::runtime_error(xorstr_("Not a primitive type!"));
}

#define SERIALIZE(name, variable)                     \
	do {                                              \
		if (direction == Direction::SERIALIZE)        \
			json[name] = variable;                    \
		else if (direction == Direction::DESERIALIZE) \
			if (json.hasKey(name))                    \
				Assign(variable, json[name]);         \
	} while (0)

// Vector serialization sadly requires me to access x,y,z,w variables, because of ImGui
// TODO Fail-safes
#define SERIALIZE_VECTOR2D(name, vector)                                             \
	do {                                                                             \
		if (direction == Direction::SERIALIZE) {                                     \
			json[name] = json::Array(vector.x, vector.y);                            \
		} else if (direction == Direction::DESERIALIZE) {                            \
			if (!json.hasKey(name))                                                  \
				break;                                                               \
			json::JSON& array = json[name];                                          \
			if (array.JSONType() != json::JSON::Class::Array || array.length() != 2) \
				break;                                                               \
			vector.x = array[0].ToFloat();                                           \
			vector.y = array[1].ToFloat();                                           \
		}                                                                            \
	} while (0)

#define SERIALIZE_VECTOR3D(name, vector)                                             \
	do {                                                                             \
		if (direction == Direction::SERIALIZE) {                                     \
			json[name] = json::Array(vector.x, vector.y, vector.z);                  \
		} else if (direction == Direction::DESERIALIZE) {                            \
			if (!json.hasKey(name))                                                  \
				break;                                                               \
			json::JSON& array = json[name];                                          \
			if (array.JSONType() != json::JSON::Class::Array || array.length() != 3) \
				break;                                                               \
			vector.x = array[0].ToFloat();                                           \
			vector.y = array[1].ToFloat();                                           \
			vector.z = array[2].ToFloat();                                           \
		}                                                                            \
	} while (0)

#define SERIALIZE_VECTOR4D(name, vector)                                             \
	do {                                                                             \
		if (direction == Direction::SERIALIZE) {                                     \
			json[name] = json::Array(vector.x, vector.y, vector.z, vector.w);        \
		} else if (direction == Direction::DESERIALIZE) {                            \
			if (!json.hasKey(name))                                                  \
				break;                                                               \
			json::JSON& array = json[name];                                          \
			if (array.JSONType() != json::JSON::Class::Array || array.length() != 4) \
				break;                                                               \
			vector.x = array[0].ToFloat();                                           \
			vector.y = array[1].ToFloat();                                           \
			vector.z = array[2].ToFloat();                                           \
			vector.w = array[3].ToFloat();                                           \
		}                                                                            \
	} while (0)

#define SERIALIZE_STRUCT(name, struct)                                      \
	do {                                                                    \
		if (direction == Direction::SERIALIZE) {                            \
			json::JSON inner = json::JSON::Make(json::JSON::Class::Object); \
			struct.Serialize(inner, direction);                             \
			json[name] = inner;                                             \
		} else if (direction == Direction::DESERIALIZE) {                   \
			if (!json.hasKey(name))                                         \
				break;                                                      \
			struct.Serialize(json[name], direction);                        \
		}                                                                   \
	} while (0)

#define SERIALIZER() \
	void Serialize(json::JSON& json, Direction direction)

#define SCOPED_SERIALIZER(scope) \
	void scope::Serialize(json::JSON& json, Direction direction)

#endif