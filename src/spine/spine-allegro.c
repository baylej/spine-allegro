#include <stdlib.h>

#include <spine/spine-allegro.h>
#include <spine/extension.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

static ALLEGRO_BITMAP* _load_bitmap (const char* path) {
	ALLEGRO_PATH *al_path;
	ALLEGRO_BITMAP *image;
	
	al_path = al_create_path(path);
	if (!al_path) return NULL;
	
	image = al_load_bitmap(al_path_cstr(al_path, '/'));
	al_destroy_path(al_path);
	return image;
}

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	ALLEGRO_BITMAP* texture = _load_bitmap(path);
	if (!texture) return;
	self->rendererObject = texture;
	
	self->width = al_get_bitmap_width(texture);
	self->height = al_get_bitmap_height(texture);}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	al_destroy_bitmap((ALLEGRO_BITMAP*)self->rendererObject);
}

char* _spUtil_readFile (const char* path, int* length) {
	return _readFile(path, length);
}

SkeletonDrawable* loadSkeleton(spSkeletonData* skeleton, spAnimationStateData* stateData) {
	SkeletonDrawable* sd = (SkeletonDrawable*)_malloc(sizeof(SkeletonDrawable), NULL, 0);
	
	sd->timeScale = 1.;
	spBone_setYDown(true);

	sd->skeleton = spSkeleton_create(skeleton);
	sd->state = spAnimationState_create(stateData);

	return sd;
}

void disposeSkeleton(SkeletonDrawable* skeleton) {
	spAnimationState_dispose(skeleton->state);
	spSkeleton_dispose(skeleton->skeleton);
	_free(skeleton);
}

void skeletonUpdate(SkeletonDrawable* sd, float delta) {
	spSkeleton_update(sd->skeleton, delta);
	spAnimationState_update(sd->state, delta * sd->timeScale);
	spAnimationState_apply(sd->state, sd->skeleton);
	spSkeleton_updateWorldTransform(sd->skeleton);

}

void skeletonDraw(SkeletonDrawable* sd) {
	int i;
	unsigned int size_x, size_y;
	unsigned char r, g, b, a;
	float vertexPositions[8];
	const int indices1[] = {0,1,2}, indices2[]={0,2,3};

	spAttachment* attachment;
	spRegionAttachment* regionAttachment;

	ALLEGRO_BITMAP* texture;
	ALLEGRO_VERTEX vertices[4];
	
	for(i=0; i < sd->skeleton->slotCount; ++i) {
		spSlot *slot = sd->skeleton->slots[i];
		attachment = slot->attachment;

		if(!attachment || attachment->type != SP_ATTACHMENT_REGION) continue;
		
		regionAttachment = (spRegionAttachment*)attachment;
		spRegionAttachment_computeWorldVertices(regionAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, vertexPositions);

		r = sd->skeleton->r * slot->r * 255;
		g = sd->skeleton->g * slot->g * 255;
		b = sd->skeleton->b * slot->b * 255;
		a = sd->skeleton->a * slot->a * 255;

		vertices[0].color = al_map_rgba(r, g, b, a);
		vertices[1].color = al_map_rgba(r, g, b, a);
		vertices[2].color = al_map_rgba(r, g, b, a);
		vertices[3].color = al_map_rgba(r, g, b, a);

		vertices[0].x = vertexPositions[SP_VERTEX_X1];
		vertices[0].y = vertexPositions[SP_VERTEX_Y1];
		vertices[1].x = vertexPositions[SP_VERTEX_X2];
		vertices[1].y = vertexPositions[SP_VERTEX_Y2];
		vertices[2].x = vertexPositions[SP_VERTEX_X3];
		vertices[2].y = vertexPositions[SP_VERTEX_Y3];
		vertices[3].x = vertexPositions[SP_VERTEX_X4];
		vertices[3].y = vertexPositions[SP_VERTEX_Y4];
		vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = 0.0;

		texture = (ALLEGRO_BITMAP*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;

		size_y = al_get_bitmap_height(texture);
		size_x = al_get_bitmap_width (texture);
		vertices[0].u = regionAttachment->uvs[SP_VERTEX_X1] * size_x;
		vertices[0].v = regionAttachment->uvs[SP_VERTEX_Y1] * size_y;
		vertices[1].u = regionAttachment->uvs[SP_VERTEX_X2] * size_x;
		vertices[1].v = regionAttachment->uvs[SP_VERTEX_Y2] * size_y;
		vertices[2].u = regionAttachment->uvs[SP_VERTEX_X3] * size_x;
		vertices[2].v = regionAttachment->uvs[SP_VERTEX_Y3] * size_y;
		vertices[3].u = regionAttachment->uvs[SP_VERTEX_X4] * size_x;
		vertices[3].v = regionAttachment->uvs[SP_VERTEX_Y4] * size_y;

		al_draw_indexed_prim(vertices, NULL, texture, indices1, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
		al_draw_indexed_prim(vertices, NULL, texture, indices2, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
	}
}
