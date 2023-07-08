#ifndef SERIALIZATION
#define SERIALIZATION

#include <string>

namespace Serialization {
	std::string GetConfigDirectory();

	bool Load(std::string filename);
	bool Save(std::string filename);
}

#endif