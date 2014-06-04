/**
 * Spine Runtime for Allegro5 - Bayle Jonathan
 * Image I/O addon must be loaded : include <allegro5/allegro_image.h>
 * al_init_image_addon must return TRUE before you use this code
 *
 * Primitives addon must be loaded : include <allegro5/allegro_primitives.h>
 * al_init_primitives_addon must return TRUE before you use this code
 */

#pragma once

#ifndef _SPINE_ALLEG_H
#define _SPINE_ALLEG_H

#include <spine/spine.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	spSkeleton* skeleton;
	spAnimationState* state;
	float timeScale;
} SkeletonDrawable;

SkeletonDrawable* loadSkeleton(spSkeletonData* skeleton, spAnimationStateData* stateData);
void disposeSkeleton(SkeletonDrawable* skeleton);

void skeletonUpdate(SkeletonDrawable* sd, float delta);
void skeletonDraw  (SkeletonDrawable* sd);

#ifdef __cplusplus
}
#endif

#endif /* _SPINE_ALLEG_H */
