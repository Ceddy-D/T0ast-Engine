#include <stdlib.h>		//malloc
#include <stdarg.h>		//arg

#include "toast_engine.h"

void Init_Sprite(Sprite *outputSprite, int *inputSprite)
{
	int size;
	int i;
	
	// Gets total byte size of template.
	size = sizeof(inputSprite) / sizeof(inputSprite[0]);
	
	// Gets X and Y size of output sprite.
	outputSprite->xMax = inputSprite[0];
	outputSprite->yMax = inputSprite[1];
	
	// Shifts data of the sprite 2 places to the left.
	for (i = 0; i < size; i++) inputSprite[i] = inputSprite[i+2];
	
	// Points output sprite to input sprite.
	outputSprite->data = inputSprite;
	
	outputSprite->visible = 1;
}

int Draw_Sprite(Sprite sprite, Transform transform)
{
	int i;
	int a,b;
	
	for (i = 0; i < sprite.xMax*sprite.yMax; i++)
	{
		a = i%sprite.xMax+transform.x;
		b = i/sprite.xMax+transform.y;
		if (sprite.data[i+2] != 3) Bdisp_SetPoint_VRAM(a, b, sprite.data[i+2]);
	}
}


void Init_Tileset(Tileset *tileset, int numSprites, ...)
{
	Sprite *spritesPtrTemp = malloc(sizeof(Sprite) * numSprites);
	int i;
	
	va_list args;
	va_start (args, numSprites);
	
	// Points spritesPtrTemp to all sprites in parameters.
	for (i = 0; i < numSprites; i++)
	{
		spritesPtrTemp[i] = va_arg(args, Sprite);
	}
	va_end(args);
	
	tileset->sprites = spritesPtrTemp;
}


void Init_Tilemap(Tilemap *tilemap, Tileset tileset, int *map)
{
	int size;
	int i;
	
	// Size of map.
	size = map[0] * map[1];
	
	tilemap->tileset = tileset;
	
	tilemap->pixelSpacing = tileset.sprites[0].xMax;
	
	tilemap->xMax = map[0];
	tilemap->yMax = map[1];
	
	tilemap->map = map;
}


void Draw_Tilemap(Tilemap tilemap, Camera camera, Transform transform)
{
	int i,j;
	Sprite s;
	for (i = 0; i < tilemap.xMax; i++)
	{
		for (j = 0; j < tilemap.yMax; j++)
		{
			
			if (tilemap.map[i+2 + j*tilemap.xMax] != -1)
				Draw_Sprite(tilemap.tileset.sprites[tilemap.map[i+2 + j*tilemap.xMax]], transform);
		}
	}
}

void Attach_Components(Node *node, Components *components)
{
	node->data = (void *)components;
}


