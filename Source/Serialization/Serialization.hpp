#ifndef SERIALIZATION
#define SERIALIZATION

namespace Serialization {
	char* GetConfigDirectory();

	bool Load(const char* filename);
	bool Save(const char* filename);
}

#endif