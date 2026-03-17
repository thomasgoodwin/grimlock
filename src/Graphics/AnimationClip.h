#ifndef ANIMATION_CLIP_H
#define ANIMATION_CLIP_H
#include <string>

struct AnimationClip
{
  std::string name;
  int row;           // sprite sheet row (0-indexed from top)
  int startFrame;    // first frame column (0-indexed)
  int endFrame;      // last frame column (inclusive)
  float fps;         // frames per second
  bool loop;
  float offsetX = 0.0f; // horizontal UV offset (0-1)
  float offsetY = 0.0f; // vertical UV offset (0-1)
};

#endif
