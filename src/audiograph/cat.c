
/**Consumable animation track
 * 
 * Makes it possible to animate float data
 * over time in a generic fashion
 */

#ifndef CONSUMABLE_ANIMATION_TRACK_C
#define CONSUMABLE_ANIMATION_TRACK_C

#include <stdlib.h>
#include "../boolean.h"
#include "../math.c"
#include "../linkedlist.c"

#define KeyFrameP struct KeyFrame *

struct KeyFrame {
  float time;
  float value;
};

KeyFrameP KeyFrame_create (float time, float value) {
  KeyFrameP result = malloc(sizeof(struct KeyFrame));
  result->time = time;
  result->value = value;
  return result;
}

bool KeyFrame_destroy (KeyFrameP keyframe) {
  if (keyframe == 0) return false;
  free(keyframe);
  return true;
}

/**Calls lerp, but on keyframe values
 * This does not work with time offset as interpolant, use KeyFrame_lerp_time instead
 *
 * interpolant must be a value between 0.0 and 1.0
 */
float KeyFrame_lerp (KeyFrameP first, KeyFrameP second, float interpolant) {
  return lerp(
    first->value,
    second->value,
    interpolant
  );
}

/**Given two keyframes and a current time, interpolate between them
 * Time should be between both first->time and second->time
 * Which are the times of the keyframes as offset in their track
 * 
 * Example:
 * `
 * KeyFrameP first  = KeyFrame_create( 25.0, 0.0  );
 * KeyFrameP second = KeyFrame_create( 75.0, 10.0 );
 * 
 * float currentTime = 50.0;
 * 
 * KeyFrame_lerp_time (first, second, currentTime); //returns 5.0
 * //This is because currentTime was half way between the key frames (25 and 75)
 * //So result is half way between 0 and 10 (keyframe values)
 * `
 */
float KeyFrame_lerp_time (KeyFrameP first, KeyFrameP second, float time) {
  return KeyFrame_lerp (
    first, second,

    inverseLerp(
      first->time,
      second->time,
      time
    )
  );
}

//========Track
#define TrackP struct Track *
struct Track {
  llnp keyframes;
  float duration;
  KeyFrameP (*addKeyFrame)(TrackP thiz, float time, float value, bool recalcTrackDuration);
  float (*getValueAtTime)(TrackP thiz, float time);
  KeyFrameP (*setValueAtTime)(TrackP thiz, float time, float value);
};

KeyFrameP Track_addKeyFrame (TrackP track, float time, float value, bool recalcTrackDuration) {
  KeyFrameP result = KeyFrame_create(time, value);
  if (track->keyframes == 0) {
    llnp first = lln_create();
    first->value = result;

    track->keyframes = first;
  } else {
    lln_add_value(track->keyframes, result);
  }
  if (recalcTrackDuration && time > track->duration) track->duration = time;
  return result;
}

KeyFrameP Track_getKeyFrameFloor (TrackP track, float time) {
  if (track->keyframes == 0) return 0;

  KeyFrameP latest = 0;

  llnp current = track->keyframes;
  KeyFrameP currentFrame;

  while (current != 0) {
    currentFrame = current->value;
    
    //Make sure we don't return frames after the time
    if (currentFrame->time > time) {
      return latest; //may be null
    //Update latest
    } else if (latest == 0 || currentFrame->time > latest->time) {
      latest = currentFrame;
    }

    current = current->next;
  }
  return latest;
}

KeyFrameP Track_getKeyFrameCeil (TrackP track, float time) {
  if (track->keyframes == 0) return 0;

  KeyFrameP earliest = 0;

  llnp current = track->keyframes;
  KeyFrameP currentFrame;

  while (current != 0) {
    currentFrame = current->value;
    
    //Make sure we don't return frames before the time
    if (currentFrame->time < time) {
      //Do nothing
    //Update latest
    } else if (earliest == 0 || currentFrame->time < earliest->time) {
      earliest = currentFrame;
    }

    current = current->next;
  }
  return earliest;
}

float Track_getValueAtTime (TrackP track, float time) {
  if (track->keyframes == 0) return 0;

  KeyFrameP start = Track_getKeyFrameFloor(track, time);
  KeyFrameP end = Track_getKeyFrameCeil(track, time);

  //Technically already handled when we checked keyframes == 0
  if (start == 0 && end == 0) return 0;

  //If no keyframe before time
  if (start == 0) return 0;

  //If no keyframe after, return last known
  if (end == 0) return start->value;

  //Linear interpolate between the frames
  return KeyFrame_lerp_time(start, end, time);
}

KeyFrameP Track_getOrCreateKeyFrameAtTime (TrackP track, float time, float timeTolerance) {
  if (track->keyframes == 0) {
    return Track_addKeyFrame(track, time, 0, true);
  }
  llnp current = track->keyframes;
  KeyFrameP currentFrame;

  while (current != 0) {
    currentFrame = current->value;

    if (
      (timeTolerance == 0 && currentFrame->time == time) ||
      m_fabs(currentFrame->time - time) <= timeTolerance
    ) {
      return currentFrame;
    }

    current = current->next;
  }
  return currentFrame;
}

KeyFrameP Track_setValueAtTime (TrackP track, float time, float value) {
  KeyFrameP result = Track_addKeyFrame(track, time, value, true);
  //TODO - find keyframe if exists at time already
  return result;
}

TrackP Track_create () {
  TrackP result = malloc(sizeof(struct Track));
  result->keyframes = 0;
  result->duration = 0;
  result->addKeyFrame = &Track_addKeyFrame;
  result->getValueAtTime = &Track_getValueAtTime;
  result->setValueAtTime = &Track_setValueAtTime;
  return result;
}

bool Track_destroy (TrackP track) {
  if (track == 0) return false;
  if (track->keyframes != 0) free(track->keyframes);
  free(track);
  return true;
}

#endif
