#ifndef FEATURES
#define FEATURES

#include "../Serialization/Serializer.hpp"

#include "General/General.hpp"
#include "Legit/Legit.hpp"
#include "Movement/Movement.hpp"
#include "Semirage/Semirage.hpp"
#include "Visuals/Visuals.hpp"

// TODO OOP
namespace Features {
	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
