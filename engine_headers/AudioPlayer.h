#ifndef AUDIOPLAYER_CLASS_H
#define AUDIOPLAYER_CLASS_H

#include<irrKlang/irrKlang.h>
#include"Transform.h"

class AudioPlayer
{
	public:
		static AudioPlayer Instance;
		irrklang::ISoundEngine* soundEngine;

		void InitializeSoundEngine();
		void Play2DSound(const char* filename, bool looped);
		void Play3DSound(const char* filename, glm::vec3 position, float minimumDistance, bool looped);
		void SetListenerPosition(glm::vec3 position, glm::vec3 orientation);
};

#endif AUDIOPLAYER_CLASS_H
