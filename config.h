#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED


typedef enum
{ Fill, Dia, Tile, Center } ImageMode;

typedef struct
{
	int r, g, b, a;
}	Color, *PColor;




void usage (char *commandline);
int setRootAtoms (Pixmap pixmap);
int getHex (char c);
int parse_color (char *arg, PColor c, int a);
int load_Mod_image (ImageMode mode, const char *arg, int rootW, int rootH,
				int alpha, Imlib_Image rootimg, int ck0);
int findX(char *whereisX, int *rW, int *rH);


#define DESCRIPTION "wallpaper setter with user options"
#define PACKAGE_STRING "mhrootimg 1.0"
#define PACKAGE_NAME "mhrootimg"
#define PACKAGE_TARNAME "mhrootimg"
#define VERSION "1.0"
#define PACKAGE_BUGREPORT "userxbw@gmail.com"

#endif // CONFIG_H_INCLUDED
