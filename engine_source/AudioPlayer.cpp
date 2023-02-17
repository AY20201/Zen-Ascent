#include"../engine_headers/AudioPlayer.h"

AudioPlayer AudioPlayer::Instance;

void AudioPlayer::InitializeSoundEngine()
{
	soundEngine = irrklang::createIrrKlangDevice();
}

void AudioPlayer::Play2DSound(const char* filename, bool looped)
{
	irrklang::ISound* sound = soundEngine->play2D(filename, looped);

	if (!looped)
	{
		sound->drop();
	}
}

void AudioPlayer::Play3DSound(const char* filename, glm::vec3 position, float minimumDistance, bool looped)
{
	irrklang::ISound* sound = soundEngine->play3D(filename, irrklang::vec3df(position.x, position.y, position.z), looped);
	//sound->setMinDistance(minimumDistance);

	if (!looped && sound != nullptr)
	{
		sound->drop();
	}
}

void AudioPlayer::SetListenerPosition(glm::vec3 position, glm::vec3 orientation)
{
	soundEngine->setListenerPosition(irrklang::vec3df(position.x, position.y, position.z), irrklang::vec3df(orientation.x, orientation.y, orientation.z));
}