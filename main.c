#include "fxlib.h"
#include "SH4_compatibility.h"

#include "toast_engine.h"

int getkey(void);	// not used
void system_menu(void);

int AddIn_main(int isAppli, unsigned short OptionNum)
{
	Sprite s;
	Font font = Init_Font();
	Tileset tset;
	Tilemap tmap;
	Coord c = {30,10};
	Coord c2 = {56,24};
	char data[] = {0x1D};
	int i;
	
	s = TOAST;
	
	Bdisp_AllClr_DDVRAM();
	while(1)
	{
		if (IsKeyDown(KEY_CTRL_MENU)) system_menu();
		if (IsKeyDown(KEY_CTRL_UP)) c.y--;
		if (IsKeyDown(KEY_CTRL_DOWN)) c.y++;
		if (IsKeyDown(KEY_CTRL_LEFT)) c.x--;
		if (IsKeyDown(KEY_CTRL_RIGHT)) c.x++;
		Bdisp_AllClr_VRAM();
		
		Draw_Sprite(s, c2);
		Draw_Text("ABC\n\0abc", c, font);
		locate(0,0);
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

