#include "../GameFunctions.hpp"

#include "../../../Features/General/EnginePrediction.hpp"
#include "../../../Features/Misc/AutoAccept.hpp"

int Hooks::Game::EmitSound::hookFunc(void* thisptr,
	void* filter,
	int iEntIndex,
	int iChannel,
	const char* pSoundEntry,
	unsigned int nSoundEntryHash,
	const char* pSample,
	float flVolume,
	int iSoundLevel,
	int nSeed,
	int iFlags,
	int iPitch,
	const Vector* pOrigin,
	const Vector* pDirection,
	void* pUtlVecOrigins,
	bool bUpdatePositions,
	float soundtime,
	int speakerentity,
	void* params)
{
	autoAccept.emitSound(pSoundEntry);

	if (enginePrediction.emitSound(iEntIndex, pSoundEntry))
		return 0;

	return invokeFunction<
		int, void*, void*, int,
		int, const char*, unsigned int, const char*,
		float, int, int, int,
		int, const Vector*, const Vector*, void*,
		bool, float, int, void*>(
		hook->proxy, thisptr, filter, iEntIndex,
		iChannel, pSoundEntry, nSoundEntryHash, pSample,
		flVolume, iSoundLevel, nSeed, iFlags,
		iPitch, pOrigin, pDirection, pUtlVecOrigins,
		bUpdatePositions, soundtime, speakerentity,
		params);
}
