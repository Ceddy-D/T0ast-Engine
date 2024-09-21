#include "toast_engine.h"

/*
** Initiates sprite contents to point to that of a template and sets sprite X and Y values.
** ========================================================================================
** parameter *sprite - The sprite to be initiated.
** parameter *templateSprite - The template to be pointed to.
** return - returns 0 on success and negative error value on failure.
*/
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
	outputSprite->mask = malloc(outputSprite->xMax * outputSprite->yMax);
	// Returning if memory allocation fails.
	if (!outputSprite->data) return -1;
	if (!outputSprite->mask) return -2;
	
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
	// Basically checks if it should write unique data to outputSprite.mask.
	if (inputSprite[4] == 1)
	{
		outputSprite->mask = outputSprite->data;
		return 0;
	}
	
	// Prepares variables for next block.
	bitIndex = 0;
	byteIndex = 0;
	byte = 0;
	
	// Goes through every remaining value of inputSprite and adds it to outputSprite.mask.
	// Same code block as earlier. Could probably condense this code somehow later.
	for ( ; inputIndex < outputSprite->xMax * outputSprite->yMax * 2 + headerSize; inputIndex++)
	{
		byte = (byte << 1) | (inputSprite[inputIndex] & 1);
		bitIndex++;
		
		if(bitIndex == 8)
		{
			outputSprite->mask[byteIndex++] = byte;
			byte = 0;
			bitIndex = 0;
		}
	}
	// Write any extra data that my remain in case the sprite isn't a multiple of 8 pixels.
	if(bitIndex != 8) outputSprite->mask[byteIndex] = byte;
	
	return 0;
}

/*
** Draws sprite at ( transform.x , transform.y )
** scaled to ( transform.scaleX , transform.scaleY)
** rotated by ( transform.rotation ) degrees clockwise.
** ====================================================
** parameter sprite - The sprite to be drawn.
** parameter transform - The transformation to be done to the sprite.
*/
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
			// Returns if all pixels in the sprite have been calculated.
			if (bitIndex >= sprite.xMax*sprite.yMax) return;
			
			// Continues if the current pixel shouldn't change.
			if (sprite.opaque == 0) if (!((sprite.mask[byteIndex] >> bitPointer) & 1))continue;
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
	}
}

/*
** Initializes Tileset's sprites.
** ==============================
** parameter *tileset - The tileset to be initialized.
** parameter numSprites - How many sprites are to be initialized to the tileset.
** parameter ... - Sprites to be set to tileset.sprites. 
** return - returns 0 on success and negative error value on failure.
*/
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

/*
** Initializes a Tilemap's tileset and map.
** ========================================
** parameter *tilemap - The tilemap to be initialized.
** parameter tileset - The tilemap's tileset.
** parameter *map - The tilemap's map.
*/
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

/*
** Draws a tilemap to the screen.
** ==============================
** parameter tilemap - The tilemap to draw to the screen.
** parameter camera - The camera that looks at the tilemap.
** parameter transform - The transformation to be done to the tilemap.
*/
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

/*
** Attaches any number of components to a node's data.
** ===================================================
** parameter *node - Pointer to Node object to be edited.
** parameter *components - Pointer to Components object to add to *node.
*/
void Attach_Components(Node *node, Components *components)
{
	node->data = (void *)components;
}

/*
** Displays the text given.
** ========================
** parameter *string - String to be drawn.
** parameter coord - Upper right coordinate of the text to be written.
** parameter font - Font to display the text in.
*/
void Draw_Text(unsigned char *string, Coordinate coord, Font font)
{
	int i;
	unsigned char c;
	Sprite spacing;
	
	spacing.data = malloc(font.spacing * font.height / 8 + 1);
	for (i = 0; i < font.spacing * font.height / 8 + 1; i++)
		spacing.data[i] = 0x00;
	spacing.visible = 1;
	spacing.opaque = 1;
	spacing.xMax = font.spacing;
	spacing.yMax = font.height;
	
	for (i = 0; string[i] != '\0'; i++)
	{
		c = string[i] - 32;
		Draw_Sprite(font.character[c], coord);
		coord.x += font.character[c].xMax;
//		Draw_Sprite(spacing, coord);
		coord.x += font.spacing;
	}
	
	free(spacing.data);
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


////////////
// Consts //
////////////

// Default sprite
unsigned char D_TOAST[] = {
0x00,0x00,0x00,
0x00,0x00,0x00,0x60,
0x30,0x00,0x0A,
0x02,0xCF,0xFD,0x20,
0x23,0x00,0x23,
0x03,0x00,0x00,0xE0,
0x3C,0x00,0x0F,
0x01,0xC7,0x1C,0x40,
0x1C,0x9A,0x64,
0x80,0x8B,0xAE,0x50,
0x08,0xFB,0xE2,
0x43,0xC7,0x1C,0x18,
0x08,0x00,0x01,
0x07,0xF2,0x50,0x1C,
0x04,0x1A,0x02,
0x00,0x30,0x00,0x40,
0x04,0xE0,0xF8,
0x00,0x81,0xF0,0x00,
0x00,0x00,0x00,
0x00
};
unsigned char M_TOAST[] = {
0x00,0x00,0x07,
0x87,0x80,0x00,0xF8,
0x7F,0xFF,0xFF,
0x87,0xFF,0xFF,0xFC,
0x7F,0xFF,0xFF,
0xC7,0xFF,0xFF,0xFC,
0x7F,0xFF,0xFF,
0xC7,0xFF,0xFF,0xFE,
0x3F,0xFF,0xFF,
0xE3,0xFF,0xFF,0xFF,
0x7F,0xFF,0xFF,
0xF7,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,
0xF0,0x7F,0xFF,0xF8,
0x0F,0xFF,0xFF,
0x00,0xFF,0xFF,0xE0,
0x0E,0x1F,0xE0,
0x00
};
const Sprite TOAST = {D_TOAST,M_TOAST,28,19,1,0};
