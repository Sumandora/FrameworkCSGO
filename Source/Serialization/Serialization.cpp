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

	Direction direction = Direction::DESERIALIZE; // used inside macro
	SERIALIZED_NAMESPACE(xorstr_("Features"), Features)

	return true;
}

bool Serialization::Save(const char* filename)
{
	const mINI::INIFile file(filename);
	mINI::INIStructure ini;

	const Serializer serializer(ini);

	Direction direction = Direction::SERIALIZE; // used inside macro
	SERIALIZED_NAMESPACE(xorstr_("Features"), Features)

	return file.write(ini, true);
}