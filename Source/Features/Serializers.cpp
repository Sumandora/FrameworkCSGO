#include "Features.hpp"

BEGIN_SERIALIZED_STRUCT(Features::General::Serializer)
SERIALIZED_NAMESPACE(Menu, xorstr_("Menu"))
SERIALIZED_NAMESPACE(Watermark, xorstr_("Watermark"))
SERIALIZED_NAMESPACE(EnginePrediction, xorstr_("Engine prediction"))
SERIALIZED_NAMESPACE(EventLog, xorstr_("Event log"))
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Legit::Serializer)
SERIALIZED_NAMESPACE(Aimbot, xorstr_("Legit Aimbot"))
SERIALIZED_NAMESPACE(Triggerbot, xorstr_("Spectator list"))
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Serializer)
SERIALIZED_NAMESPACE(Aimbot, xorstr_("Semirage Aimbot"))
SERIALIZED_NAMESPACE(Backtrack, xorstr_("Backtrack"))
SERIALIZED_NAMESPACE(RecoilAssistance, xorstr_("Recoil assistance"))
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Movement::Serializer)
SERIALIZED_NAMESPACE(Bhop, xorstr_("Bhop"))
SERIALIZED_NAMESPACE(CrouchJump, xorstr_("Crouch jump"))
SERIALIZED_NAMESPACE(JumpBug, xorstr_("Jump bug"))
SERIALIZED_NAMESPACE(EdgeJump, xorstr_("Edge jump"))
SERIALIZED_NAMESPACE(AutoStrafer, xorstr_("Auto strafer"))
SERIALIZED_NAMESPACE(FastDuck, xorstr_("Fast duck"))
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Visuals::Serializer)
SERIALIZED_NAMESPACE(Esp, xorstr_("Esp"))
SERIALIZED_NAMESPACE(SpectatorList, xorstr_("Spectator list"))
SERIALIZED_NAMESPACE(NoPunch, xorstr_("No punch"))
SERIALIZED_NAMESPACE(FOV, xorstr_("FOV"))
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Serializer)
SERIALIZED_NAMESPACE(General, xorstr_("General"))
SERIALIZED_NAMESPACE(Legit, xorstr_("Legit"))
SERIALIZED_NAMESPACE(Semirage, xorstr_("Semirage"))
SERIALIZED_NAMESPACE(Movement, xorstr_("Movement"))
SERIALIZED_NAMESPACE(Visuals, xorstr_("Visuals"))
END_SERIALIZED_STRUCT
