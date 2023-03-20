#include "Serialization.hpp"

#include "Serializer.hpp"

#include <pwd.h>
#include <unistd.h>

#include "../Features/Features.hpp"

char* Serialization::GetConfigDirectory()
{
	return strcat(getpwuid(getuid())->pw_dir, xorstr_("/.config/Framework"));
}

bool Serialization::Load(const char* filename)
{
	const mINI::INIFile file(filename);
	mINI::INIStructure ini;
	if (!file.read(ini))
		return false;

	const Serializer serializer(ini);

	Features::Serializer(serializer, Direction::DESERIALIZE);

	return true;
}

bool Serialization::Save(const char* filename)
{
	const mINI::INIFile file(filename);
	mINI::INIStructure ini;

	const Serializer serializer(ini);

	Features::Serializer(serializer, Direction::SERIALIZE);

	return file.write(ini, true);
}