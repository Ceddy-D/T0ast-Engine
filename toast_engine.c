#include "toast_engine.h"

int Init_Sprite(Sprite *outputSprite, unsigned char *inputSprite)
{
	// Input sprite header:
	// Value 1-2(char): x-y size of the sprite.
	// Value 3(bool): Is visible.
	// Value 4(bool): Is opaque.
	// Value 5(bool): Opacity data same as color data.
	
	// Variable declarations.
	const char headerSize = 5;
	int inputIndex, bitIndex = 0, byteIndex = 0;
	unsigned char byte;
	
	// Setting header data.
	outputSprite->xMax = inputSprite[0];
	outputSprite->yMax = inputSprite[1];
	outputSprite->visible = inputSprite[2];
	outputSprite->opaque = inputSprite[3];
	
	// Allocating memory for the output sprite.
	outputSprite->data = malloc(outputSprite->xMax * outputSprite->yMax);
	outputSprite->visibility = malloc(outputSprite->xMax * outputSprite->yMax);
	// Returning if memory allocation fails.
	if (!outputSprite->data) return -1;
	if (!outputSprite->visibility) return -2;
	
	// Goes through every value of inputSprite following the header until the end of the sprite size
	// and converts the input data into binary in the sprite.data variable.
	for (inputIndex = headerSize; inputIndex < outputSprite->xMax * outputSprite->yMax + headerSize; inputIndex++)
	{
		byte = (byte << 1) | (inputSprite[inputIndex] & 1);
		bitIndex++;
		
		// If the byte is fully written then write to outputSprite and reset.
		if(bitIndex == 8)
		{
			outputSprite->data[byteIndex++] = byte;
			byte = 0;
			bitIndex = 0;
		}
	}
	// Write any extra data that my remain in case the sprite isn't a multiple of 8 pixels.
	if(bitIndex != 8) outputSprite->data[byteIndex] = byte;
	
	// Rest of the function acts as an else statement to this.
	// Basically checks if it should write unique data to outputSprite.visibility.
	if (inputSprite[4] == 1)
	{
		outputSprite->visibility = outputSprite->data;
		return 0;
	}
	
	// Prepares variables for next block.
	bitIndex = 0;
	byteIndex = 0;
	byte = 0;
	
	// Goes through every remaining value of inputSprite and adds it to outputSprite.visibility.
	// Same code block as earlier. Could probably condense this code somehow later.
	for ( ; inputIndex < outputSprite->xMax * outputSprite->yMax * 2 + headerSize; inputIndex++)
	{
		byte = (byte << 1) | (inputSprite[inputIndex] & 1);
		bitIndex++;
		
		if(bitIndex == 8)
		{
			outputSprite->visibility[byteIndex++] = byte;
			byte = 0;
			bitIndex = 0;
		}
	}
	// Write any extra data that my remain in case the sprite isn't a multiple of 8 pixels.
	if(bitIndex != 8) outputSprite->visibility[byteIndex] = byte;
	
	return 0;
}

void Draw_Sprite(Sprite sprite, Coordinate coord)
{
	// Variable declarations.
	int bitIndex, byteIndex, bitPointer;
	int x,y;
	
	// Ends function if the sprite is invisible.
	if(!sprite.visible) return;
	
	// Starts at -1 instead of 0 because we need to increment it before a conditional statement that may prematurely continue the loop.
	bitIndex = -1;
	
	// Increments through each byte of the sprite data until null.
	for (byteIndex = 0; ; byteIndex++)
	{
		// Draws each bit of the current byte to screen.
		for (bitPointer = 7; bitPointer >= 0; bitPointer--)
		{
			// Total number of bits of all bytes calculated.
			bitIndex++;
			
			// Continues if the current pixel shouldn't change.
			if (!((sprite.visibility[byteIndex] >> bitPointer) & 1) && sprite.opaque == 0) continue;
			// Calculates x and y coordinates for the pixel
			x = bitIndex%sprite.xMax+coord.x;
			y = bitIndex/sprite.xMax+coord.y;
			// Continues if the x or y are out of bounds.
			// Returns if the sprite will never be rendered.
			if (x < 0 || y < 0 || x > 127) continue;
			if (y > 63) return;
			// Draw.
			Bdisp_SetPoint_VRAM(x,y,(sprite.data[byteIndex] >> bitPointer) & 1);
		}
		
		// Basically the 2nd argument of the for statement. Here to fix a bug.
		if (sprite.data[byteIndex] == '\0') return;
	}
}

int Init_Tileset(Tileset *tileset, int numSprites, ...)
{
	Sprite *spritesPtrTemp = malloc(sizeof(Sprite) * numSprites);
	int i;
	
	va_list args;
	
	if (!spritesPtrTemp) return -1;
	
	va_start (args, numSprites);
	
	// Points spritesPtrTemp to all sprites in parameters.
	for (i = 0; i < numSprites; i++)
	{
		spritesPtrTemp[i] = va_arg(args, Sprite);
	}
	va_end(args);
	
	tileset->sprites = spritesPtrTemp;
	
	return 0;
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

void Draw_Tilemap(Tilemap tilemap, Camera camera, Coordinate coord)
{
	int i,j;
	Sprite s;
	for (i = 0; i < tilemap.xMax; i++)
	{
		for (j = 0; j < tilemap.yMax; j++)
		{
			if (tilemap.map[i+2 + j*tilemap.xMax] != -1)
				Draw_Sprite(tilemap.tileset.sprites[tilemap.map[i+2 + j*tilemap.xMax]], coord);
		}
	}
}

void Attach_Components(Node *node, Components *components)
{
	node->data = (void *)components;
}

/*void Draw_Char(unsigned char character, Coordinate coord, Font f)
{
	int i, j;
	for (i = 1; f.c[i] != '\0'; i++)
	{
		for (j = 7; j >= 0; j--)
		{
			f.c[0]
		}
	}
}

void Draw_Text(unsigned char *string, Coordinate coord, Font f);
{
	int i;
	int x;
	
	for (i = 0; string[i] != '\0'; i++)
	{
		Draw_Char(string[i], coord, f);
	}
}*/

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


