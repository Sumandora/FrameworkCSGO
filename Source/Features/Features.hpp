#ifndef FEATURES
#define FEATURES

#include "../Serialization/Serializer.hpp"

#include <memory>
#include <vector>

inline std::map<std::string, std::vector<class Feature*>> features{};

class Feature {
	std::string name;

public:
	Feature() = delete;
	inline Feature(const std::string& group, const std::string& name)
	{
		this->name = name;
		features[group].push_back(this);
	}

	const std::string& GetName() const { return name; }

	virtual void SetupGUI() = 0;
	virtual SERIALIZER() = 0;
};

#endif
