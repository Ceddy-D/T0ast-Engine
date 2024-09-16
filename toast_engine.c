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

// This is a mess and I probably need to just gut this whole function later.
/*Sprite Sprite_Rotate(Sprite sprite, Transform *transform);
{
	Coordinate *grid = malloc(sizeof(double) * (sprite.xMax*sprite.yMax));
	Coordinate *roatated = malloc(sizeof(double) * (sprite.xMax*sprite.yMax));
	Sprite output;
	
	double cosTheta = cos(Transform.rotation * M_PI / 180);
	double sinTheta = sin(Transform.rotation * M_PI / 180);
	
	double centerX = sprite.xMax/2 -.5;
	double centerY = sprite.yMax/2 -.5;
	
	int xMin = 0;
	int yMin = 0;
	
	int i;
	
	for (i = 0; i < sprite.xMax*sprite.yMax; i++)
	{
		grid[i].x = i%sprite.xMax - centerX;
		grid[i].y = i/sprite.xMax - centerY;
		
		rotated[i].x = round(grid[i].x * cosTheta - grid[i].y * sinTheta + centerX);
		rotated[i].y = round(grid[i].x * sinTheta + grid[i].y * cosTheta + centerY);
		
		if(rotated[i].x < xMin) xMin = rotated[i].x;
		if(rotated[i].y < yMin) yMin = rotated[i].y;
		if(rotated[i].x < output.xMax) output.xMax = rotated[i].x;
		if(rotated[i].y < output.yMax) output.yMax = rotated[i].y;
	}
	
	for (i = 0; i < sprite.xMax*sprite.yMax; i++)
	{
		output.data[output.xMax*rotated[i].y+rotated[i].x]
	}
	
	transform->x -= output.xMin;
	transform->y -= output.yMin;
}*/


