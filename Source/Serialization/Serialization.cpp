#include "Serialization.hpp"

#include "Serializer.hpp"

#include "../Features/General/EnginePrediction.hpp"
#include "../Features/General/EventLog.hpp"
#include "../Features/General/Menu.hpp"
#include "../Features/General/Watermark.hpp"

#include "../Features/Legit/Aimbot.hpp"
#include "../Features/Legit/ESP/ESP.hpp"
#include "../Features/Legit/SpectatorList.hpp"
#include "../Features/Legit/Triggerbot.hpp"

#include "../Features/Movement/Bhop.hpp"
#include "../Features/Movement/HighJump.hpp"
#include "../Features/Movement/JumpBug.hpp"

#include "../Features/Semirage/Aimbot.hpp"
#include "../Features/Semirage/Backtrack.hpp"
#include "../Features/Semirage/RecoilAssistance.hpp"

bool Serialization::Load(const char* filename)
{
	mINI::INIFile file(filename);
	mINI::INIStructure ini;
	if (!file.read(ini))
		return false;

	Serializer serializer(ini);

	// TODO make a list
	Features::General::Menu::Serializer(serializer, Direction::DESERIALIZE);
	Features::General::Watermark::Serializer(serializer, Direction::DESERIALIZE);
	Features::General::EnginePrediction::Serializer(serializer, Direction::DESERIALIZE);
	Features::General::EventLog::Serializer(serializer, Direction::DESERIALIZE);

	Features::Legit::Aimbot::Serializer(serializer, Direction::DESERIALIZE);
	Features::Legit::Esp::Serializer(serializer, Direction::DESERIALIZE);
	Features::Legit::SpectatorList::Serializer(serializer, Direction::DESERIALIZE);
	Features::Legit::Triggerbot::Serializer(serializer, Direction::DESERIALIZE);

	Features::Semirage::Aimbot::Serializer(serializer, Direction::DESERIALIZE);
	Features::Semirage::Backtrack::Serializer(serializer, Direction::DESERIALIZE);
	Features::Semirage::RecoilAssistance::Serializer(serializer, Direction::DESERIALIZE);

	Features::Movement::Bhop::Serializer(serializer, Direction::DESERIALIZE);
	Features::Movement::HighJump::Serializer(serializer, Direction::DESERIALIZE);
	Features::Movement::JumpBug::Serializer(serializer, Direction::DESERIALIZE);

	return true;
}

bool Serialization::Save(const char* filename)
{
	mINI::INIFile file(filename);
	mINI::INIStructure ini;

	Serializer serializer(ini);

	Features::General::Menu::Serializer(serializer, Direction::SERIALIZE);
	Features::General::Watermark::Serializer(serializer, Direction::SERIALIZE);
	Features::General::EnginePrediction::Serializer(serializer, Direction::SERIALIZE);
	Features::General::EventLog::Serializer(serializer, Direction::SERIALIZE);

	Features::Legit::Aimbot::Serializer(serializer, Direction::SERIALIZE);
	Features::Legit::Esp::Serializer(serializer, Direction::SERIALIZE);
	Features::Legit::SpectatorList::Serializer(serializer, Direction::SERIALIZE);
	Features::Legit::Triggerbot::Serializer(serializer, Direction::SERIALIZE);

	Features::Semirage::Aimbot::Serializer(serializer, Direction::SERIALIZE);
	Features::Semirage::Backtrack::Serializer(serializer, Direction::SERIALIZE);
	Features::Semirage::RecoilAssistance::Serializer(serializer, Direction::SERIALIZE);

	Features::Movement::Bhop::Serializer(serializer, Direction::SERIALIZE);
	Features::Movement::HighJump::Serializer(serializer, Direction::SERIALIZE);
	Features::Movement::JumpBug::Serializer(serializer, Direction::SERIALIZE);

	return file.write(ini, true);
}