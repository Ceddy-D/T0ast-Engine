/////////////////////////////
//                         //
//  T0AST ENGINE VER. 0.2  //
//                         //
//       2024-08-28        //
//                         //
//   Created by Ceddy D    //
//                         //
/////////////////////////////


#ifndef toast_engine_h
#define toast_engine_h


//////////////
// Includes //
//////////////

#include <stdlib.h>	// malloc()
#include <stdarg.h>	// arg
#include <math.h>	// sin(), cos()


////////////
// Macros //
////////////

#define DEFAULTTRANSFORM {0,0,100,100,0}
#define DEFAULTCAMERA {{0,0},{DEFAULTTRANSFORM}}


///////////////
//  Structs  //
///////////////

typedef struct Node{
	void *data;		// Component data.
	char **tags;	// User defined tags.
	
	struct Node *parent;	// Parent node.
	struct Node *child;		// Child nodes.
	int children;			// Number of children.
} Node;

typedef struct Transform{
	float x,y;				// X and Y coords.
	float scaleX, scaleY;	// X and Y scale of node.
	float rotation;			// Rotation in degrees.
} Transform;

typedef struct Sprite{
	int *data;	// Pointer to sprite contents.
	int xMax;	// Number of horizontal pixels, or columns, the sprite is.
	int yMax;	// Number of vertical pixels, or rows, the sprite is.
	
	int visible;	// Boolean flag for visiblility. 1 visible 0 invisible.
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

typedef struct Coordinate{
	float x,y;
} Coordinate;

/////////////////
//  Functions  //
/////////////////

/*
** Initiates sprite contents to point to that of a template and sets sprite X and Y values.
** ========================================================================================
** parameter *sprite - The sprite to be initiated.
** parameter *templateSprite - The template to be pointed to.
*/
void Init_Sprite(Sprite *sprite, int *templateSprite);

/*
** Draws sprite at ( transform.x , transform.y )
** scaled to ( transform.scaleX , transform.scaleY)
** rotated by ( transform.rotation ) degrees clockwise.
** ====================================================
** parameter sprite - The sprite to be drawn.
** parameter transform - The transformation to be done to the sprite.
** return - Returns 1 is sprite is drawn and 0 if sprite is not drawn.
*/
int Draw_Sprite(Sprite sprite, Transform transform);

/*
** Initializes Tileset's sprites.
** ==============================
** parameter *tileset - The tileset to be initialized.
** parameter numSprites - How many sprites are to be initialized to the tileset.
** parameter ... - Sprites to be set to tileset.sprites. 
*/
void Init_Tileset(Tileset *tileset, int numSprites, ...);

/*
** Initializes a Tilemap's tileset and map.
** ========================================
** parameter *tilemap - The tilemap to be initialized.
** parameter tileset - The tilemap's tileset.
** parameter *map - The tilemap's map.
*/
void Init_Tilemap(Tilemap *tilemap, Tileset tileset, int *map);

/*
** Draws a tilemap to the screen.
** ==============================
** parameter tilemap - The tilemap to draw to the screen.
** parameter camera - The camera that looks at the tilemap.
** parameter transform - The transformation to be done to the tilemap.
*/
void Draw_Tilemap(Tilemap tilemap, Camera camera, Transform transform);

/*
** Attaches any number of components to a node's data.
** ===================================================
** parameter *node - Pointer to Node object to be edited.
** parameter *components - Pointer to Components object to add to *node.
*/
void Attach_Components(Node *node, Components *components);


//void Sprite_Rotate(sprite Sprite, Transform *transform);

#endif
