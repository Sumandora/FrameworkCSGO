#include "Features.hpp"

BEGIN_SERIALIZED_STRUCT(Features::General::Serializer)
SERIALIZED_NAMESPACE(xorstr_("Menu"), Menu)
SERIALIZED_NAMESPACE(xorstr_("Watermark"), Watermark)
SERIALIZED_NAMESPACE(xorstr_("Engine prediction"), EnginePrediction)
SERIALIZED_NAMESPACE(xorstr_("Event log"), EventLog)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Legit::Serializer)
SERIALIZED_NAMESPACE(xorstr_("Legit Aimbot"), Aimbot)
SERIALIZED_NAMESPACE(xorstr_("Spectator list"), Triggerbot)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Serializer)
SERIALIZED_NAMESPACE(xorstr_("Semirage Aimbot"), Aimbot)
SERIALIZED_NAMESPACE(xorstr_("Backtrack"), Backtrack)
SERIALIZED_NAMESPACE(xorstr_("Recoil assistance"), RecoilAssistance)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Movement::Serializer)
SERIALIZED_NAMESPACE(xorstr_("Bhop"), Bhop)
SERIALIZED_NAMESPACE(xorstr_("Crouch jump"), CrouchJump)
SERIALIZED_NAMESPACE(xorstr_("Jump bug"), JumpBug)
SERIALIZED_NAMESPACE(xorstr_("Edge jump"), EdgeJump)
SERIALIZED_NAMESPACE(xorstr_("Auto strafer"), AutoStrafer)
SERIALIZED_NAMESPACE(xorstr_("Fast duck"), FastDuck)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Visuals::Serializer)
SERIALIZED_NAMESPACE(xorstr_("Esp"), Esp)
SERIALIZED_NAMESPACE(xorstr_("Spectator list"), SpectatorList)
SERIALIZED_NAMESPACE(xorstr_("No punch"), NoPunch)
SERIALIZED_NAMESPACE(xorstr_("FOV"), FOV)
SERIALIZED_NAMESPACE(xorstr_("Flash reduction"), FlashReduction)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Serializer)
SERIALIZED_NAMESPACE(xorstr_("General"), General)
SERIALIZED_NAMESPACE(xorstr_("Legit"), Legit)
SERIALIZED_NAMESPACE(xorstr_("Semirage"), Semirage)
SERIALIZED_NAMESPACE(xorstr_("Movement"), Movement)
SERIALIZED_NAMESPACE(xorstr_("Visuals"), Visuals)
END_SERIALIZED_STRUCT
