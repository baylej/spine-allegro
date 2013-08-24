/**
 * Spine Runtime for Allegro5 - Bayle Jonathan
 * Image I/O addon must be loaded : include <allegro5/allegro_image.h>
 * al_init_image_addon must return TRUE before you use this code
 */

#pragma once

#ifndef _SPINE_ALLEG_H
#define _SPINE_ALLEG_H

#include <spine/spine.h>

struct SkeletonDrawable
{
	Skeleton* skeleton;
	AnimationState* state;
	float timeScale;
	
};
typedef struct SkeletonDrawable SkeletonDrawable;
SkeletonDrawable* spalleg_load_skeleton (SkeletonData* skeleton, AnimationStateData* stateData);
void spalleg_dispose_skeleton( SkeletonDrawable* skeleton);

void spalleg_update(SkeletonDrawable* sd, float delta);

void spalleg_draw(SkeletonDrawable* sd, ALLEGRO_BITMAP* bm);

#endif /* _SPINE_ALLEG_H */
