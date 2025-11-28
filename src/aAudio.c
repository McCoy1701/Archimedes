/*
 * @file src/aAudio.c
 *
 * SDL_mixer-based audio system for Archimedes game engine.
 * Provides channel-based sound effect playback and streaming music.
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Archimedes.h"

/*
 * ========================================================================
 * NEW SDL_MIXER API IMPLEMENTATION
 * ========================================================================
 */

int a_AudioInit(int channels, int frequency)
{
  // Initialize SDL_mixer with requested audio format
  if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    aError_t error;
    error.error_type = FATAL;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to initialize SDL_mixer: %s",
             log_level_strings[error.error_type], Mix_GetError());
    LOG(error.error_msg);
    return -1;
  }

  // Allocate mixing channels
  Mix_AllocateChannels(channels);

  // Store configuration in global app state
  app.audio.channel_count = channels;
  app.audio.reserved_channels = 0;
  app.audio.master_volume = AUDIO_MAX_VOLUME;
  app.audio.music_volume = AUDIO_MAX_VOLUME;

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Audio initialized: %d channels at %d Hz", channels, frequency);

  return 0;
}

void a_AudioQuit(void)
{
  // Halt all playback
  Mix_HaltChannel(-1);
  Mix_HaltMusic();

  // Close audio device
  Mix_CloseAudio();

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Audio system shut down");
}

void a_AudioReserveChannels(int num_reserved)
{
  Mix_ReserveChannels(num_reserved);
  app.audio.reserved_channels = num_reserved;

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Reserved %d audio channels", num_reserved);
}

int a_AudioLoadSound(const char* filename, aSoundEffect_t* sound)
{
  if (!sound) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: NULL sound pointer passed to a_AudioLoadSound",
             log_level_strings[error.error_type]);
    LOG(error.error_msg);
    return -1;
  }

  // Load sound file
  Mix_Chunk* chunk = Mix_LoadWAV(filename);
  if (!chunk) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to load sound '%s': %s",
             log_level_strings[error.error_type], filename, Mix_GetError());
    LOG(error.error_msg);
    return -1;
  }

  // Populate sound structure
  STRCPY(sound->filename, filename);
  sound->chunk = chunk;
  sound->default_volume = AUDIO_MAX_VOLUME;

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loaded sound: %s", filename);

  return 0;
}

int a_AudioPlaySound(aSoundEffect_t* sound, aAudioOptions_t* options)
{
  if (!sound || !sound->chunk) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Invalid sound passed to a_AudioPlaySound",
             log_level_strings[error.error_type]);
    LOG(error.error_msg);
    return -1;
  }

  // Use default options if NULL
  aAudioOptions_t default_opts = a_AudioDefaultOptions();
  aAudioOptions_t* opts = options ? options : &default_opts;

  // Interrupt channel if requested
  if (opts->interrupt && opts->channel >= 0) {
    Mix_HaltChannel(opts->channel);
  }

  // Set volume (chunk volume if specified, otherwise use sound's default)
  int volume = (opts->volume >= 0) ? opts->volume : sound->default_volume;
  Mix_VolumeChunk(sound->chunk, volume);

  // Play sound with or without fade-in
  int channel;
  if (opts->fade_ms > 0) {
    channel = Mix_FadeInChannel(opts->channel, sound->chunk, opts->loops, opts->fade_ms);
  } else {
    channel = Mix_PlayChannel(opts->channel, sound->chunk, opts->loops);
  }

  if (channel < 0) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to play sound '%s': %s",
             log_level_strings[error.error_type], sound->filename, Mix_GetError());
    LOG(error.error_msg);
    return -1;
  }

  return channel;
}

void a_AudioFreeSound(aSoundEffect_t* sound)
{
  if (sound && sound->chunk) {
    Mix_FreeChunk(sound->chunk);
    sound->chunk = NULL;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                   "Freed sound: %s", sound->filename);
  }
}

void a_AudioHaltChannel(int channel)
{
  Mix_HaltChannel(channel);
}

int a_AudioIsChannelPlaying(int channel)
{
  return Mix_Playing(channel);
}

void a_AudioSetChannelVolume(int channel, int volume)
{
  Mix_Volume(channel, volume);
}

int a_AudioLoadMusic(const char* filename, aMusic_t* music)
{
  if (!music) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: NULL music pointer passed to a_AudioLoadMusic",
             log_level_strings[error.error_type]);
    LOG(error.error_msg);
    return -1;
  }

  // Load music file
  Mix_Music* mix_music = Mix_LoadMUS(filename);
  if (!mix_music) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to load music '%s': %s",
             log_level_strings[error.error_type], filename, Mix_GetError());
    LOG(error.error_msg);
    return -1;
  }

  // Populate music structure
  STRCPY(music->filename, filename);
  music->music = mix_music;
  music->default_volume = AUDIO_MAX_VOLUME;

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loaded music: %s", filename);

  return 0;
}

int a_AudioPlayMusic(aMusic_t* music, int loops, int fade_ms)
{
  if (!music || !music->music) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Invalid music passed to a_AudioPlayMusic",
             log_level_strings[error.error_type]);
    LOG(error.error_msg);
    return -1;
  }

  // Play music with or without fade-in
  int result;
  if (fade_ms > 0) {
    result = Mix_FadeInMusic(music->music, loops, fade_ms);
  } else {
    result = Mix_PlayMusic(music->music, loops);
  }

  if (result < 0) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to play music '%s': %s",
             log_level_strings[error.error_type], music->filename, Mix_GetError());
    LOG(error.error_msg);
    return -1;
  }

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Playing music: %s", music->filename);

  return 0;
}

void a_AudioStopMusic(int fade_ms)
{
  if (fade_ms > 0) {
    Mix_FadeOutMusic(fade_ms);
  } else {
    Mix_HaltMusic();
  }
}

void a_AudioPauseMusic(void)
{
  Mix_PauseMusic();
}

void a_AudioResumeMusic(void)
{
  Mix_ResumeMusic();
}

void a_AudioSetMusicVolume(int volume)
{
  Mix_VolumeMusic(volume);
  app.audio.music_volume = volume;
}

int a_AudioIsMusicPlaying(void)
{
  return Mix_PlayingMusic();
}

void a_AudioFreeMusic(aMusic_t* music)
{
  if (music && music->music) {
    // Stop music if currently playing
    if (Mix_PlayingMusic()) {
      Mix_HaltMusic();
    }
    Mix_FreeMusic(music->music);
    music->music = NULL;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                   "Freed music: %s", music->filename);
  }
}

/*
 * ========================================================================
 * DEPRECATED: OLD SDL_QueueAudio API (Backward Compatibility Wrappers)
 * ========================================================================
 */

// Global state for old API (single audio device)
static SDL_AudioDeviceID g_legacy_device_id = 0;

int a_InitAudio(void)
{
  // Forward to new API with default settings
  return a_AudioInit(16, 44100);
}

void a_LoadSounds(const char* filename, aAudioClip_t* clip)
{
  if (!clip) {
    return;
  }

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,
                 "DEPRECATED: a_LoadSounds() called - use a_AudioLoadSound() instead");

  // Store filename
  STRCPY(clip->filename, filename);

  // Load WAV file using old SDL API
  if (SDL_LoadWAV(filename, &clip->spec, &clip->buffer, &clip->length) == NULL) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Error loading WAV file: %s",
             log_level_strings[error.error_type], filename);
    LOG(error.error_msg);
  }
}

void a_AudioPlayEffect(aAudioClip_t* clip)
{
  if (!clip || !clip->buffer) {
    return;
  }

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,
                 "DEPRECATED: a_AudioPlayEffect() called - use a_AudioPlaySound() instead");

  // Convert old aAudioClip_t to Mix_Chunk for playback
  Mix_Chunk temp_chunk;
  temp_chunk.allocated = 0;  // We don't own the buffer
  temp_chunk.abuf = clip->buffer;
  temp_chunk.alen = clip->length;
  temp_chunk.volume = AUDIO_MAX_VOLUME;

  // Play using SDL_mixer (no channel control, auto-select)
  int channel = Mix_PlayChannel(-1, &temp_chunk, 0);
  if (channel < 0) {
    aError_t error;
    error.error_type = WARNING;
    snprintf(error.error_msg, MAX_LINE_LENGTH,
             "%s: Failed to play sound effect: %s",
             log_level_strings[error.error_type], Mix_GetError());
    LOG(error.error_msg);
  }
}
