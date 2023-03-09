#ifndef SERIALIZATION
#define SERIALIZATION

namespace Serialization {
	bool Load(const char* filename);
	bool Save(const char* filename);

	void SetupGUI();

	char* GetConfigDirectory();
}

#endif