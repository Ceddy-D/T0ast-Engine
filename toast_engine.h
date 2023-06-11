/////////////////////////////
//                         //
//  T0AST ENGINE VER. 0.1  //
//                         //
//       2023-06-08        //
//                         //
/////////////////////////////


#ifndef toast_engine_h
#define toast_engine_h


///////////////
//  Structs  //
///////////////

typedef struct{
	int *data;	// Pointer to sprite contents.
	int xMax;	// Number of horizontal pixels, or columns, the sprite is.
	int yMax;	// Number of vertical pixels, or rows, the sprite is.
} Sprite;

typedef struct{
	int x;		// X coord of upper left corner of camera.
	int y;		// Y coord of upper left corner of camera.
	int scale;	// Scale factor (Change logic to Z coord one day).
} Camera;

typedef struct{
	Sprite *sprites;	// Table of tiles in the tileset.
} Tileset;

typedef struct{
	Tileset tileset;	// The tileset used by this tilemap.
	int scaleX;			// X Scale factor of tilemap.
	int scaleY;			// Y scale factor of tilemap.
	int originX;		// X coord of upper left corner of tilemap.
	int originY;		// Y coord of upper left corner of tilemap.
	int pixelSpacing;	// Pixels apart each sprites are. Defaults to xMax of .tileset.sprite[0].
	
	int *map;			// Table of tiles of the tilemap.
	int xMax;			// Number of horizontal sprites, or colums, the tilemap is.
	int yMax;			// Number of vertical sprites, or rows, the tilemap is.
} Tilemap;


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
** draws sprite at ( x , y ) scaled to a percentage.
** =================================================
** parameter x - Horizontal position of top-left pixel of drawn sprite.
** parameter y - Vertical position of top-left pixel of drawn sprite.
** parameter sprite - The sprite to be drawn.
** parameter scaleX - the percentage scale on the X axis.
** parameter scaleY - the percentage scale on the Y axis.
** return - Returns 1 is sprite is drawn and 0 if sprite is not drawn.
*/
int Draw_Sprite_Scale(int x, int y, Sprite sprite, int scaleX, int scaleY, Camera camera);

/*
** Draws sprite at ( x , y ) scaled to 100%.
** =========================================
** parameter x - Horizontal position of top-left pixel of drawn sprite.
** parameter y - Vertical position of top-left pixel of drawn sprite.
** parameter sprite - The sprite to be drawn.
** return - Returns 1 is sprite is drawn and 0 if sprite is not drawn.
*/
int Draw_Sprite(int x, int y, Sprite sprite);

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
** paramerter *tilemap - The tilemap to be initialized.
** parameter tileset - The tilemap's tileset.
** parameter *map - The tilemap's map.
*/
void Init_Tilemap(Tilemap *tilemap, Tileset tileset, int *map);

/*
** Draws a tilemap to the screen.
** ==============================
** parameter tilemap - The tilemap to draw to the screen.
** parameter camera - The camera that looks at the tilemap.
*/
void Draw_Tilemap(Tilemap tilemap, Camera camera);



#endif
