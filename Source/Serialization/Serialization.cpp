#include "Serialization.hpp"

#include "Serializer.hpp"

#include "json.hpp"

#include <cstring>
#include <fstream>
#include <pwd.h>
#include <sstream>
#include <string>
#include <unistd.h>

#include "../Features/Features.hpp"

std::string Serialization::getConfigDirectory()
{
	return std::string(getpwuid(getuid())->pw_dir) + "/.config/Framework";
}

bool Serialization::load(std::string filename)
{
	std::ifstream fileStream(filename);
	if (fileStream.fail())
		return false;

	std::ostringstream stringStream;
	fileStream >> stringStream.rdbuf();

	if (!fileStream.eof())
		return false;

	json::JSON json = json::JSON::Load(stringStream.str());

	if (json.IsNull())
		return false;

	for (auto& [category, vector] : features) {
		if (!json.hasKey(category))
			continue;

		json::JSON categoryJson = json[category];
		for (Feature* feature : vector) {
			const std::string& featureName = feature->getName();
			if (!categoryJson.hasKey(featureName))
				continue;

			json::JSON featureJson = categoryJson[featureName];
			feature->serialize(featureJson, Direction::DESERIALIZE);
		}
	}

	fileStream.close();

	return true;
}

bool Serialization::save(std::string filename)
{
	std::ofstream fileStream(filename);
	if (fileStream.fail())
		return false;

	json::JSON json = json::Object();
	for (auto& [category, vector] : features) {
		json::JSON categoryJson = json::Object();
		for (Feature* feature : vector) {
			json::JSON featureJson = json::Object();
			feature->serialize(featureJson, Direction::SERIALIZE);
			categoryJson[feature->getName()] = featureJson;
		}
		json[category] = categoryJson;
	}

	fileStream << json;
	fileStream.close();
	return true;
}