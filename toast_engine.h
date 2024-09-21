/////////////////////////////
//                         //
//  T0AST ENGINE VER. 0.3  //
//                         //
//       2024-09-16        //
//                         //
//   Created by Ceddy D    //
//                         //
/////////////////////////////


#ifndef toast_engine_h
#define toast_engine_h


////////////
// Macros //
////////////

#define DEFAULTCOORDINATE {0,0}
#define DEFAULTTRANSFORM {DEFAULTCOORDINATE,100,100,0}
#define DEFAULTCAMERA {DEFAULTTRANSFORM}

///////////////
//  Structs  //
///////////////

typedef struct Node{
	void *data;				// Component data.
	unsigned char **tags;	// User defined tags.
	
	struct Node *parent;	// Parent node.
	struct Node *child;		// Child nodes.
	int children;			// Number of children.
} Node;

typedef struct Coordinate{
	float x,y;
} Coordinate, Coord;

typedef struct Transform{
	Coordinate coord;		// X and Y coords.
	float scaleX, scaleY;	// X and Y scale of node.
	float rotation;			// Rotation in degrees.
} Transform;

typedef struct Sprite{
	unsigned char *data;		// Pointer to sprite contents.
	unsigned char *mask;	// Determines which pixels are visible.
	
	unsigned char xMax;	// Number of horizontal pixels, or columns, the sprite is.
	unsigned char yMax;	// Number of vertical pixels, or rows, the sprite is.
	
	unsigned char visible;	// Boolean flag for visiblility. 1 visible 0 invisible.
	unsigned char opaque;	// Boolean flag for opacity. 1 opaque 0 transluscent.
} Sprite;

typedef struct Camera{
	Transform transform;	// Transformation of camera. 
} Camera;

typedef struct Tileset{
	Sprite *sprites;	// Table of tiles in the tileset.
} Tileset;

typedef struct Tilemap{
	Tileset tileset;	// The tileset used by this tilemap.
	int pixelSpacing;	// Pixels apart each sprites are. Defaults to xMax of .tileset.sprite[0].
	
	int *map;	// Table of tiles of the tilemap.
	int xMax;	// Number of horizontal sprites, or colums, the tilemap is.
	int yMax;	// Number of vertical sprites, or rows, the tilemap is.
} Tilemap;

typedef struct Components{
	Transform *transform;
	Sprite *sprite;
	Camera *camera;
	Tileset *tileset;
	Tilemap *tilemap;
} Components;

typedef struct Font{
	Sprite *character;
	unsigned char spacing;
	unsigned char height;
} Font;


/////////////////
//  Functions  //
/////////////////

int Init_Sprite(Sprite *sprite, unsigned char *templateSprite);
void Draw_Sprite(Sprite sprite, Coordinate coord);
int Init_Tileset(Tileset *tileset, int numSprites, ...);
void Init_Tilemap(Tilemap *tilemap, Tileset tileset, int *map);
void Draw_Tilemap(Tilemap tilemap, Camera camera, Coordinate coord);

void Attach_Components(Node *node, Components *components);

void Draw_Text(unsigned char *string, Coordinate coord, Font font);

//void Sprite_Rotate(sprite Sprite, Transform *transform);


////////////
// Consts //
////////////

extern const Sprite TOAST;


//////////////
// Includes //
//////////////

// Standard Libararies
#include <stdlib.h>	// malloc()
#include <stdarg.h>	// arg
#include <math.h>	// sin(), cos()
// Cedmade Libraries
#include "toast_fonts.h"


#endif
