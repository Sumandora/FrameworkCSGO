#ifndef SERIALIZATION_SERIALIZER
#define SERIALIZATION_SERIALIZER

#include "mini/ini.h"

#include "imgui.h"

#include "../SDK/Math/Vector.hpp"

enum Direction {
	SERIALIZE,
	DESERIALIZE
};

namespace Serialization {
	class Serializer {
	public:
		explicit Serializer(mINI::INIStructure& structure);

		void SetSection(const char* section);
		void LeaveSection();

		std::string& GetValue(const char* name);

		// Primitives
		void Serialize(const char* name, bool& type, Direction direction);
		void Serialize(const char* name, int& type, Direction direction);
		void Serialize(const char* name, float& type, Direction direction);
		
		// TODO Save these as structures rather than types

		// Types
		void Serialize(const char* name, Vector& type, Direction direction);

		// ImGui
		void Serialize(const char* name, ImColor& type, Direction direction);

	private:
		std::string currentSection;
		mINI::INIStructure& structure;
	};
}

#define BEGIN_SERIALIZED_STRUCT(functionName)                                                      \
	void functionName(Serialization::Serializer serializer, Direction direction, const char* name) \
	{                                                                                              \
		serializer.SetSection(name);

#define SERIALIZED_TYPE(name, variable) \
	serializer.Serialize(name, variable, direction);

#define SERIALIZED_STRUCTURE(name, variable) \
	variable.Serializer(serializer, direction, name);

#define SERIALIZED_NAMESPACE(name, namespaceName) \
	namespaceName::Serializer(serializer, direction, name);

#define END_SERIALIZED_STRUCT  \
	serializer.LeaveSection(); \
	}

#define DECLARE_SERIALIZER(functionName) \
	void functionName(Serialization::Serializer serializer, Direction direction, const char* name);

#endif