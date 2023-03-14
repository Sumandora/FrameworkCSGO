#ifndef FEATURES
#define FEATURES

#include "../Serialization/Serializer.hpp"

#include "General.hpp"
#include "Legit.hpp"
#include "Movement.hpp"
#include "Semirage.hpp"

namespace Features {
	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
