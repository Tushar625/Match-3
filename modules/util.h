#pragma once


// plays a sound from a sound buffer

inline void play_sound(SOUND_ENUM sound_index) noexcept
{
	static sf::Sound sound;	// no need a create a new sound object every time this function is called

	extern std::vector<sf::SoundBuffer> sound_buffer;

	sound.setBuffer(sound_buffer[sound_index]);

	sound.play();
}