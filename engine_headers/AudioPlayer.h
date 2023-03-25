#ifndef AUDIOPLAYER_CLASS_H
#define AUDIOPLAYER_CLASS_H

#include<iostream>
#include<vector>
#include<deque>

#include<irrKlang/irrKlang.h>
#include"Transform.h"

struct Soundtrack
{
	const char* filename;
	float volume;
};

class AudioPlayer
{
	public:
		static AudioPlayer Instance;

		std::vector<Soundtrack> soundtracks;
		std::vector<irrklang::ISound*> loopedSounds;
		std::deque<int> lastTwoPlayedSounds { -1, -2 };

		irrklang::ISoundEngine* soundEngine;

		irrklang::ISound* currentSoundtrack;
		int currentSoundtrackIndex;

		void InitializeSoundEngine();
		void SetSoundtracks(std::vector<Soundtrack> soundtracks);
		void Play2DSound(const char* filename, float volume, bool looped);
		void Play3DSound(const char* filename, glm::vec3 position, float minimumDistance, float volume, bool looped);
		void PlaySoundtrack(int soundtrack, bool random);
		void UpdateSoundtracks();
		void SetListenerPosition(glm::vec3 position, glm::vec3 orientation);

		void DropLoopedSounds();
};

#endif AUDIOPLAYER_CLASS_H
