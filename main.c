#include "fxlib.h"
#include "SH4_compatibility.h"

#include "toast_engine.h"

int getkey(void);	// not used
void system_menu(void);

// For sprite testing
const int debugSprite1[] =
{
8,8,
1,1,1,1,0,0,0,0,
1,1,1,1,0,0,0,0,
1,1,1,1,0,0,0,0,
1,1,1,1,0,0,0,0,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1
};
const int debugSprite2[] =
{
8,8,
1,1,1,1,0,1,1,1,
1,1,1,1,0,0,0,1,
1,1,1,1,0,0,0,1,
1,1,1,1,0,0,0,0,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1,
0,0,0,0,1,1,1,1
};
const int debugSprite3[] =
{
8,8,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,3,3,3,3,1,1,
1,1,3,3,3,3,1,1,
1,1,3,3,3,3,1,1,
1,1,3,3,3,3,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1
};
const int debugMap1[] =
{
8,4,
2,2,2,2,2,2,2,2,
0,0,0,0,0,0,0,0,
-1,-1,-1,-1,-1,-1,-1,-1,
1,1,1,1,1,1,1,1
};

int AddIn_main(int isAppli, unsigned short OptionNum)
{
	Sprite s1, s2, s3;
	Tileset tset;
	Tilemap tmap;
	Camera c;
	
	c.x = 0;
	c.y = 0;
	
	Init_Sprite(&s1, debugSprite1);
	Init_Sprite(&s2, debugSprite2);
	Init_Sprite(&s3, debugSprite3);
	Init_Tileset(&tset, 3,s1,s2,s3);
	Init_Tilemap(&tmap, tset, debugMap1);
	
	Bdisp_AllClr_DDVRAM();
	
	while(1)
	{
		if (IsKeyDown(KEY_CTRL_MENU)) system_menu();
		
		Bdisp_AllClr_VRAM();
		
		c.scale = 100;
		Draw_Tilemap(tmap, c);
		c.scale = 400;
		Draw_Tilemap(tmap, c);
		
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

