#include "Serializer.hpp"

#include "xorstr.hpp"

Serialization::Serializer::Serializer(mINI::INIStructure& structure)
	: structure(structure)
{
}

void Serialization::Serializer::SetSection(const char* section)
{
	if (!currentSection.empty())
		currentSection = currentSection.append(xorstr_("."));
	currentSection = currentSection.append(section);
}

void Serialization::Serializer::LeaveSection()
{
	currentSection = currentSection.substr(0, currentSection.find_last_of(xorstr_(".")));
}

std::string& Serialization::Serializer::GetValue(const char* name)
{
	return structure[currentSection][name];
}