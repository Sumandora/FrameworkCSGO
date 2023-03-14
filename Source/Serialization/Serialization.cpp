#include "Serialization.hpp"

#include "Serializer.hpp"

#include "../Features/Features.hpp"

bool Serialization::Load(const char* filename)
{
	mINI::INIFile file(filename);
	mINI::INIStructure ini;
	if (!file.read(ini))
		return false;

	Serializer serializer(ini);

	Features::Serializer(serializer, Direction::DESERIALIZE);

	return true;
}

bool Serialization::Save(const char* filename)
{
	mINI::INIFile file(filename);
	mINI::INIStructure ini;

	Serializer serializer(ini);

	Features::Serializer(serializer, Direction::SERIALIZE);

	return file.write(ini, true);
}