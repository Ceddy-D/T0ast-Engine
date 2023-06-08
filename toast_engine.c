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
}


void Draw_Sprite_Scale(int x, int y, Sprite sprite, int scaleX, int scaleY, Camera camera)
{
	// Scaled X and Y size of sprite.
	int scaledXMax = sprite.xMax*scaleX/100 * camera.scale/100;
	int scaledYMax = sprite.yMax*scaleY/100 * camera.scale/100;
	int i, j;
	int a, b;
	
	// For every point on the scaled sprite, draw the correct color.
	for (i = 0; i < scaledXMax; i++)
	{
		for (j = 0; j < scaledYMax; j++)
		{
			
			a = (i * sprite.xMax) / scaledXMax + 2;				// ( To avoid having to type the same equation over and over.
			b = (j * sprite.xMax) / scaledYMax * sprite.xMax;			// I'm unsure how much time this actually saves. :P )
			
			if (sprite.data[a + b] != 3) Bdisp_SetPoint_VRAM(x+i+camera.x, y+j+camera.y, sprite.data[a + b]);
		}
	}
}


void Draw_Sprite(int x, int y, Sprite sprite)
{
	Camera c = {0,0,100};
	Draw_Sprite_Scale(x, y, sprite, 100, 100, c);	// Draws the sprite normally.
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
	
	tilemap->scaleX = 100;
	tilemap->scaleY = 100;
	tilemap->originX = 0;
	tilemap->originY = 0;
	tilemap->pixelSpacing = tileset.sprites[0].xMax;
	
	tilemap->xMax = map[0];
	tilemap->yMax = map[1];
	
	tilemap->map = map;
}


void Draw_Tilemap(Tilemap tilemap, Camera camera)
{
	int i,j;
	Sprite s;
	for (i = 0; i < tilemap.xMax; i++)
	{
		for (j = 0; j < tilemap.yMax; j++)
		{
			
			if (tilemap.map[i+2 + j*tilemap.xMax] != -1) 
				Draw_Sprite_Scale
				(
				(i*tilemap.pixelSpacing) * (tilemap.scaleX/100) * (camera.scale/100) + tilemap.originX + camera.x,
				(j*tilemap.pixelSpacing) * (tilemap.scaleY/100) * (camera.scale/100) + tilemap.originY + camera.y,
				tilemap.tileset.sprites[tilemap.map[i+2 + j*tilemap.xMax]],
				tilemap.scaleX, 
				tilemap.scaleY,
				camera
				);
		}
	}
	
}
