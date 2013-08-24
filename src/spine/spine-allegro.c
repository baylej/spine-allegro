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
	self->height = al_get_bitmap_height(texture);}

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

void spalleg_dispose_skeleton(SkeletonDrawable* skeleton)
{
	AnimationState_dispose(skeleton->state);
	Skeleton_dispose(skeleton->skeleton);
	free(skeleton);
}

void spalleg_update(SkeletonDrawable* sd, float delta)
{
	Skeleton_update(sd->skeleton, delta);
	AnimationState_update(sd->state, delta * sd->timeScale);
	AnimationState_apply(sd->state, sd->skeleton);
	Skeleton_updateWorldTransform(sd->skeleton);

}

void spalleg_draw(SkeletonDrawable* sd, ALLEGRO_BITMAP* bm)
{
	
	float vertexPositions[8];
	
	for(int i = 0; i < sd->skeleton->slotCount; ++i)
	{
		Slot *slot = sd->skeleton->slots[i];
		Attachment* attachment = slot->attachment;

		if(!attachment || attachment->type != ATTACHMENT_REGION) continue;
		
		RegionAttachment* regionAttachment = (RegionAttachment*)attachment;
		RegionAttachment_computeVertices(regionAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, vertexPositions);

		Uint8 r = sd->skeleton->r * slot->r * 255;
		Uint8 g = sd->skeleton->g * slot->g * 255;
		Uint8 b = sd->skeleton->b * slot->b * 255;
		Uint8 a = sd->skeleton->a * slot->a * 255;

		float vertices[4][2];
		
		/* colors on vertex ?? */

		vertices[0][0] = vertexPositions[VERTEX_X1]
		vertices[0][1] = vertexPositions[VERTEX_Y1];
		vertices[1][0] = vertexPositions[VERTEX_X2]
		vertices[1][1] = vertexPositions[VERTEX_Y2];
		vertices[2][0] = vertexPositions[VERTEX_X3]         
		vertices[2][1] = vertexPositions[VERTEX_Y3];
		vertices[3][0] = vertexPositions[VERTEX_X4]
		vertices[3][1] = vertexPositions[VERTEX_Y4];

		
		


}
