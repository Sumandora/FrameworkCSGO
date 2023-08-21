#ifndef SERIALIZATION
#define SERIALIZATION

#include <string>

namespace Serialization {
	std::string getConfigDirectory();

	bool load(std::string filename);
	bool save(std::string filename);
}

#endif