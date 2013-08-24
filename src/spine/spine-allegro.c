#include <spine/spine-allegro.h>
#include <spine/extension.h>

#include <allegro5/allegro.h>

static ALLEGRO_BITMAP* _spalleg_load_bitmap (const char* path) {
	ALLEGRO_PATH *al_path;
	ALLEGRO_BITMAP *image;
	
	al_path = al_create_path(path);
	if (!al_path) return NULL;
	
	image = al_load_bitmap(al_path_cstr(al_path, '/'));
	al_destroy_path(al_path);
	return image;
}

void _AtlasPage_createTexture (AtlasPage* self, const char* path) {
	ALLEGRO_BITMAP* texture = _spalleg_load_bitmap(path);
	if (!texture) return;
	self->rendererObject = texture;
	
	self->width = al_get_bitmap_width(texture);
	self->height = al_get_bitmap_height(texture);
}

void _AtlasPage_disposeTexture (AtlasPage* self) {
	al_destroy_bitmap((ALLEGRO_BITMAP*)self->rendererObject);
}

char* _Util_readFile (const char* path, int* length) {
	return _readFile(path, length);
}


SkeletonDrawable* spalleg_load_skeleton(SkeletonData* skeleton, AnimationStateData* stateData)
{
	SkeletonDrawable* sd = malloc(sizeof(SkeletonDrawable));
	sd->timeScale = 1.;
	Bone_setYDown(true);

	sd->skeleton = Skeleton_create(skeleton);
	sd->state = AnimationState_create(stateData);

	return sd;
}
