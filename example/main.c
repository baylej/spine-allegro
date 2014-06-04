#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <spine/spine.h>
#include <spine/spine-allegro.h>

SkeletonDrawable *drawable = NULL;
spSkeletonData *skeletonData;
spAtlas *atlas = NULL;

void init_spineboy(void) {
	spSkeleton *skeleton = NULL;
	spSkeletonJson *json;
	spAnimationStateData *stateData;

	// Load atlas, skeleton, and animations.
	atlas = spAtlas_createFromFile("../spine-runtimes/spine-c/data/spineboy.atlas", NULL);
	json = spSkeletonJson_create(atlas);
	json->scale = 0.3;
	skeletonData = spSkeletonJson_readSkeletonDataFile(json, "../spine-runtimes/spine-c/data/spineboy.json");
	spSkeletonJson_dispose(json);

	// Configure mixing.
	stateData = spAnimationStateData_create(skeletonData);
	spAnimationStateData_setMixByName(stateData, "walk", "jump", 0.2f);
	spAnimationStateData_setMixByName(stateData, "jump", "walk", 0.4f);

	drawable = loadSkeleton(skeletonData, stateData);
	drawable->timeScale = 1;

	skeleton = drawable->skeleton;
	skeleton->flipX = false;
	skeleton->flipY = false;
	spSkeleton_setToSetupPose(skeleton);

	skeleton->x = 320;
	skeleton->y = 420;
	spSkeleton_updateWorldTransform(skeleton);

	spAnimationState_setAnimationByName(drawable->state, 0, "walk",  true);
	spAnimationState_addAnimationByName(drawable->state, 0, "jump", false, 0);
	spAnimationState_addAnimationByName(drawable->state, 0,  "run",  true, 0);
	spAnimationState_addAnimationByName(drawable->state, 0, "jump", false, 3);
	spAnimationState_addAnimationByName(drawable->state, 0, "walk",  true, 0);
	spAnimationState_addAnimationByName(drawable->state, 0, "idle", false, 2);
	spAnimationState_addAnimationByName(drawable->state, 0, "walk",  true, 0);
	spAnimationState_addAnimationByName(drawable->state, 0,"shoot", false, 2);
	spAnimationState_addAnimationByName(drawable->state, 0, "walk",  true, 0);
	spAnimationState_addAnimationByName(drawable->state, 0,"death", false, 6);
}

#define fatal_error(str) { fputs(str, stderr); goto errquit; }

int main(int argc, char *argv[]) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *equeue = NULL;
	ALLEGRO_EVENT ev;

	double time;
 
	if (!al_init())	fatal_error("failed to initialize allegro!");

	al_set_new_display_flags(ALLEGRO_OPENGL);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_REQUIRE);
	display = al_create_display(640, 480);
	if (!display) fatal_error("failed to create display!");

	if (!al_init_primitives_addon()) fatal_error("failed to initialise primitives!");
	if (!al_init_image_addon()) fatal_error("failed to initialise ImageIO!");

	equeue = al_create_event_queue();
	if (!equeue) fatal_error("failed to create event queue!");

	timer = al_create_timer(1.0/30.0);
	if (!timer) fatal_error("failed to create timer!");

	al_register_event_source(equeue, al_get_display_event_source(display));
	al_register_event_source(equeue, al_get_timer_event_source(timer));

	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	init_spineboy();
 
	al_start_timer(timer);
	time = al_get_time();

	while(al_wait_for_event(equeue, &ev), 1) {
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0,0,0));
			skeletonUpdate(drawable, (float) (al_get_time() - time));
			time = al_get_time();
			skeletonDraw(drawable);
			al_flip_display();
		}
	}

	spSkeletonData_dispose(skeletonData);
	spAtlas_dispose(atlas);
	disposeSkeleton(drawable);
	al_destroy_timer(timer);
	al_destroy_event_queue(equeue);
	al_destroy_display(display);
 
	return 0;

errquit:
	return -1;
}