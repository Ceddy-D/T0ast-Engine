#include <stdio.h>

unsigned char* name = "FONT";

unsigned char sprite[] = {
3,2,1,1,1,
1,0,1,
1,0,1
};

int main()
{
	unsigned char headerIndex, byte;
	char hex[3];
	int i, bitIndex = 0, dataIndex = 0, headerSize = 5;
	
	printf("unsigned char D_%s[] = {\n", name);
	for (i = headerSize; i < sprite[0]*sprite[1]+headerSize; i++)
	{
		byte = (byte << 1) | (sprite[i] & 1);
		bitIndex++;
		dataIndex++;
		
		if(bitIndex == 8)
		{
			sprintf(hex, "%02X", byte);
			printf("0x%s,", hex);
			byte = 0;
			bitIndex = 0;
		}
		if (i >= sprite[0]*sprite[1]+headerSize) break;
		if (dataIndex%sprite[0] == 0 && sprite[0] > 7) printf("\n");
	}
	sprintf(hex, "%02X", byte);
	printf("0x%s\n};\n", hex);
	
	byte = 0;
	bitIndex = 0;
	dataIndex = 0;
	
	if(sprite[4] == 0)
	{
		printf("unsigned char M_%s[] = {\n", name);
		for (i = headerSize+sprite[0]*sprite[1]; i < sprite[0]*sprite[1]*2+headerSize; i++)
		{
			byte = (byte << 1) | (sprite[i] & 1);
			bitIndex++;
			dataIndex++;
			
			if(bitIndex == 8)
			{
				sprintf(hex, "%02X", byte);
				printf("0x%s,", hex);
				byte = 0;
				bitIndex = 0;
			}
			
			if (dataIndex%sprite[0] == 0) printf("\n");
		}
		sprintf(hex, "%02X", byte);
		printf("0x%s\n};\n", hex);
	}
	printf("const Sprite %s = {D_%s,", name, name);
	if(sprite[4] == 0)
		printf("M_%s", name);
	else
		printf("NULL");
	printf(",%i,%i,%i,%i};\n", sprite[0], sprite[1], sprite[2], sprite[3]);
}
