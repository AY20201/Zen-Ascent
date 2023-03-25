#include"../engine_headers/AudioPlayer.h"

AudioPlayer AudioPlayer::Instance;

void AudioPlayer::InitializeSoundEngine()
{
	soundEngine = irrklang::createIrrKlangDevice();
}

void AudioPlayer::SetSoundtracks(std::vector<Soundtrack> soundtracks)
{
	AudioPlayer::soundtracks = soundtracks;
}

void AudioPlayer::Play2DSound(const char* filename, float volume, bool looped)
{
	irrklang::ISound* sound = soundEngine->play2D(filename, looped, false, true);

	if (sound != nullptr) {
		sound->setVolume(volume);

		if (!looped)
		{
			sound->drop();
		}
		else 
		{
			loopedSounds.push_back(sound);
		}
	}
}

void AudioPlayer::Play3DSound(const char* filename, glm::vec3 position, float minimumDistance, float volume, bool looped)
{
	irrklang::ISound* sound = soundEngine->play3D(filename, irrklang::vec3df(position.x, position.y, position.z), looped, false, true);

	if (sound != nullptr) {
		sound->setVolume(volume);
		sound->setMinDistance(minimumDistance);

		if (!looped)
		{
			sound->drop();
		}
		else
		{
			loopedSounds.push_back(sound);
		}
	}
}

void AudioPlayer::PlaySoundtrack(int soundtrack, bool random)
{
	int randSoundtrack = rand() % (int)soundtracks.size();
	if (random)
	{
		while (std::count(lastTwoPlayedSounds.begin(), lastTwoPlayedSounds.end(), randSoundtrack) != 0)
		{
			randSoundtrack = rand() % (int)soundtracks.size();
		}
	}

	int soundtrackIndex = random ? randSoundtrack : soundtrack;
	lastTwoPlayedSounds.push_front(soundtrackIndex);
	lastTwoPlayedSounds.pop_back();

	irrklang::ISound* sound = soundEngine->play2D(soundtracks[soundtrackIndex].filename, false, false, true);

	if (sound != nullptr) {
		sound->setVolume(soundtracks[soundtrackIndex].volume);

		currentSoundtrack = sound;
		currentSoundtrackIndex = soundtrackIndex;
	}
}

void AudioPlayer::UpdateSoundtracks()
{
	if (currentSoundtrack != nullptr)
	{
		if (currentSoundtrack->isFinished())
		{
			currentSoundtrack->drop();
			PlaySoundtrack(0, true);
		}
	}
}

void AudioPlayer::SetListenerPosition(glm::vec3 position, glm::vec3 orientation)
{
	soundEngine->setListenerPosition(irrklang::vec3df(position.x, position.y, position.z), irrklang::vec3df(orientation.x, orientation.y, orientation.z));
}

void AudioPlayer::DropLoopedSounds()
{
	for (unsigned int i = 0; i < loopedSounds.size(); i++)
	{
		loopedSounds[i]->drop();
	}
}