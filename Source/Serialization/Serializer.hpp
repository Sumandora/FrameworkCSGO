#ifndef SERIALIZATION_SERIALIZER
#define SERIALIZATION_SERIALIZER

#include "mini/ini.h"

#include "imgui.h"

enum Direction {
	SERIALIZE,
	DESERIALIZE
};

namespace Serialization {
	class Serializer {
	public:
		Serializer(mINI::INIStructure& structure);

		void SetSection(const char* section);
		void LeaveSection();

		std::string& GetValue(const char* name);

		// Primitives
		void Serialize(const char* name, bool& type, Direction direction);
		void Serialize(const char* name, int& type, Direction direction);
		void Serialize(const char* name, float& type, Direction direction);

		// ImGui
		void Serialize(const char* name, ImColor& type, Direction direction);

	private:
		std::string currentSection;
		mINI::INIStructure& structure;
	};
}

#define BEGIN_SERIALIZED_STRUCT(functionName, name)                              \
	void functionName(Serialization::Serializer serializer, Direction direction) \
	{                                                                            \
		serializer.SetSection(name);

#define SERIALIZED_TYPE(name, variable) \
	serializer.Serialize(name, variable, direction);

#define SERIALIZED_STRUCTURE(variable) \
	variable.Serializer(serializer, direction);

#define SERIALIZED_NAMESPACE(name) \
	name::Serializer(serializer, direction);

#define END_SERIALIZED_STRUCT  \
	serializer.LeaveSection(); \
	}

#define DECLARE_SERIALIZER(functionName) \
	void functionName(Serialization::Serializer serializer, Direction direction);

#endif