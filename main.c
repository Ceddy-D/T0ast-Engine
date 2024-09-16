#include "fxlib.h"
#include "SH4_compatibility.h"

#include "toast_engine.h"

int getkey(void);	// not used
void system_menu(void);

// For sprite testing

const unsigned char debugSprite1[] =
{
8,8,1,0,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,0,0,0,0,1,1,
1,1,0,0,0,0,1,1,
1,1,0,0,0,0,1,1,
1,1,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1
};

const unsigned char debugSprite2[] =
{
8,8,1,0,0,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,1,
1,1,1,1,0,0,0,1,
1,1,1,1,0,0,0,1,
1,1,1,1,1,1,1,1,
};

int AddIn_main(int isAppli, unsigned short OptionNum)
{
	Sprite s, s2;
	Tileset tset;
	Tilemap tmap;
	Coord c = DEFAULTCOORDINATE;
	Coord c2 = {56,24};
	
	Init_Sprite(&s, debugSprite1);
	Init_Sprite(&s2,debugSprite2);
	
	Bdisp_AllClr_DDVRAM();
	while(1)
	{
		if (IsKeyDown(KEY_CTRL_MENU)) system_menu();
		if (IsKeyDown(KEY_CTRL_UP)) c.y--;
		if (IsKeyDown(KEY_CTRL_DOWN)) c.y++;
		if (IsKeyDown(KEY_CTRL_LEFT)) c.x--;
		if (IsKeyDown(KEY_CTRL_RIGHT)) c.x++;
		
		Bdisp_AllClr_VRAM();
		
		Draw_Sprite(s, c);
		Draw_Sprite(s2, c2);
//		Draw_Text("ABC", DEFAULTCOORDINATE);
		locate(0,0);
		Bdisp_AllClr_DD();
		Bdisp_PutDisp_DD();
		Sleep(5);
	}
	
	
	
	return 0;
}

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section

#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section

