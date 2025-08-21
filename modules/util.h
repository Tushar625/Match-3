#pragma once


inline void play_sound(SOUND_ENUM sound_index) noexcept
{
	static sf::Sound sound;

	extern std::vector<sf::SoundBuffer> sound_buffer;

	sound.setBuffer(sound_buffer[sound_index]);

	sound.play();
}