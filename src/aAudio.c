#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

aAudioClip_t playerSnd;

int a_InitAudio( void )
{
  //aLoadSounds("resources/soundEffects/laser1.wav", &playerSnd);
  
  app.deviceID = SDL_OpenAudioDevice(NULL, 0, &(playerSnd.spec), NULL, 0);

  if (app.deviceID == 0) {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to load device id %s",
              log_level_strings[new_error.error_type], SDL_GetError() );
    LOG( new_error.error_msg );
    
    return 1;
  }
  
  SDL_PauseAudioDevice(app.deviceID, 0);
  return 0;
}

void a_LoadSounds( const char *filename, aAudioClip_t *clip )
{
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
  
  STRNCPY(clip->filename, filename, sizeof(char));

  if(SDL_LoadWAV(filename, &(clip->spec), &(clip->buffer), &(clip->length)) == NULL)
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Error loading: %s",
              log_level_strings[new_error.error_type], filename );
    LOG( new_error.error_msg );
  }
}

void a_AudioPlayEffect(aAudioClip_t *clip)
{
  int success = SDL_QueueAudio(app.deviceID, clip->buffer, clip->length);
  
  if (success < 0)
  {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Couldn't play sound: %s, %s",
              log_level_strings[new_error.error_type], clip->filename, SDL_GetError() );
    LOG( new_error.error_msg );
  }
}

