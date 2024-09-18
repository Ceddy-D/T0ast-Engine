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
1,1,1,1,1,1,1,1,
};

int AddIn_main(int isAppli, unsigned short OptionNum)
{
	Sprite s, s2;
	Font font = Init_Font();
	int fontPointer = 1;
	Tileset tset;
	Tilemap tmap;
	Coord c = {30,10};
	Coord c2 = {56,24};
	char data[] = {0x1D};
	int i;
	
	Init_Sprite(&s, debugSprite1);
	s2 = CATTO;
	
	Bdisp_AllClr_DDVRAM();
	while(1)
	{
		if (IsKeyDown(KEY_CTRL_MENU)) system_menu();
		if (IsKeyDown(KEY_CTRL_UP)) c.y--;
		if (IsKeyDown(KEY_CTRL_DOWN)) c.y++;
		if (IsKeyDown(KEY_CTRL_LEFT)) c.x--;
		if (IsKeyDown(KEY_CTRL_RIGHT)) c.x++;
		if (IsKeyDown(KEY_CTRL_F1)) fontPointer--;
		if (IsKeyDown(KEY_CTRL_F6)) fontPointer++;
		if (fontPointer == -1) fontPointer = 0;
		if (fontPointer == 96) fontPointer = 95;
		Bdisp_AllClr_VRAM();
		
		Draw_Sprite(font.character[fontPointer], c);
		Draw_Sprite(s2, c2);
//		Draw_Text("ABC", DEFAULTCOORDINATE);
		locate(0,0);
		Bdisp_AllClr_DD();
		Bdisp_PutDisp_DD();
		Sleep(500);
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

