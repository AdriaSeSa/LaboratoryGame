#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

enum SFX
{
	PLAYER_JUMP,
	POWER_UP_COLLECT,
	CHECK_POINT,
	PLAYER_JUMP_BLOCK,
	ENEMY_DIE,
	ENEMY_HIT,
	GROUND,
	SELECT,
	BLIP_SELECT,
	BUTTONDOWN,
	BUTTONFEEDBACK,
	MOUSEDOWN
};

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init(pugi::xml_node&);
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	Mix_Music*			music = nullptr;
	List<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__