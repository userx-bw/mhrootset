#include <X11/Xatom.h>
#include </usr/include/Imlib2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"




void usage (char *commandline)
{
  printf (PACKAGE_STRING " - " DESCRIPTION "\n"
        "\n"
        "Syntaxis: %s [command1 [arg1..]] [command2 [arg1..]]..."
        "\n"
        "Gradients:\n"
        " -add <color>               Add color to range using distance 1\n"
        " -addd <color> <distance>   Add color to range using custom distance\n"
        " -gradient <angle>          Render gradient using specified angle\n"
        " -clear                     Clear the color range\n"
        "\n"
        "Dia:\n"
        " -Dia <user specified>     Renders image within monitor settings\n"
        "                           Format  <size>x<size>\n"
        "Solid:\n"
        " -solid <color>             Render a solid using the specified color\n"
        "\n"
        "Image files:\n"
        " -center <image>            Render an image centered on screen\n"
        " -tile 0 <image>            Tiles the image at orginal size\n"
        " -tile <size>x<image>       Tiles image at user set size\n"
        " -fill <image>              Render image at monitor settings\n"
        "\n"
        "Manipulations:\n"
        " -tint <color>              Tint the current image\n"
        " -blur <radius>             Blur the current image\n"
        " -sharpen <radius>          Sharpen the current image\n"
        " -contrast <amount>         Adjust contrast of current image\n"
        " -brightness <amount>       Adjust brightness of current image\n"
        " -gamma <amount>            Adjust gamma level of current image\n"
        " -flipv                     Flip the current image vertically\n"
        " -fliph                     Flip the current image horizontally\n"
        " -flipd                     Flip the current image diagonally\n"
        "\n"
        "Misc:\n"
        " -alpha <amount>            Adjust alpha level for colors and images\n"
        " -write <filename>          Write current image to file\n"
        "\n"
        "Colors are in the #rrbbgg or #rrggbbaa format.\n"
        "\n"
        "Send bugreports to: " PACKAGE_BUGREPORT "\n" "\n", commandline);
}

// Globals:
Display *display;
int screen;

// Adapted from fluxbox' bsetroot
int setRootAtoms (Pixmap pixmap)
{
	Atom atom_root, atom_eroot, type;
	unsigned char *data_root, *data_eroot;
	int format;
	unsigned long length, after;

	atom_root = XInternAtom (display, "_XROOTMAP_ID", True);
	atom_eroot = XInternAtom (display, "ESETROOT_PMAP_ID", True);

	// doing this to clean up after old background
	if (atom_root != None && atom_eroot != None)
	{
		XGetWindowProperty (display, RootWindow (display, screen),
							atom_root, 0L, 1L, False, AnyPropertyType,
							&type, &format, &length, &after, &data_root);

	if (type == XA_PIXMAP)
	{
		XGetWindowProperty (display, RootWindow (display, screen),
							atom_eroot, 0L, 1L, False, AnyPropertyType,
							&type, &format, &length, &after, &data_eroot);

	if (data_root && data_eroot && type == XA_PIXMAP &&
		*((Pixmap *) data_root) == *((Pixmap *) data_eroot))
	{
		XKillClient (display, *((Pixmap *) data_root));
	}
  } // second if
} //first if

	atom_root = XInternAtom (display, "_XROOTPMAP_ID", False);
	atom_eroot = XInternAtom (display, "ESETROOT_PMAP_ID", False);

	if (atom_root == None || atom_eroot == None)
		return 0;

	// setting new background atoms
	XChangeProperty (display, RootWindow (display, screen),
					atom_root, XA_PIXMAP, 32, PropModeReplace,
					(unsigned char *) &pixmap, 1);

	XChangeProperty (display, RootWindow (display, screen), atom_eroot,
					XA_PIXMAP, 32, PropModeReplace, (unsigned char *) &pixmap,
					1);

	return 1;
}

int getHex (char c)
{
	switch (c)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return c - '0';
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return c - 'A' + 10;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return c - 'a' + 10;
	default:
return 0;
	}// end switch
} // end function

int parse_color (char *arg, PColor c, int a)
{
	if (arg[0] != '#')
		return 1;

	if ((strlen (arg) != 7) && (strlen (arg) != 9))
		return 1;

	c->r = getHex (arg[1]) * 16 + getHex (arg[2]);
	c->g = getHex (arg[3]) * 16 + getHex (arg[4]);
	c->b = getHex (arg[5]) * 16 + getHex (arg[6]);
	c->a = a;

	if (strlen (arg) == 9)
		c->a = getHex (arg[7]) * 16 + getHex (arg[8]);

	return 0;
}
/*****************************
 *
 * my modified code
 *
 * **********************/

int load_Mod_image (ImageMode mode, const char *arg, int userW, int userH,
				int alpha, Imlib_Image rootimg, int ck0)
{
	int width, height;
	int imgW, imgH, o;
	int left, top;
	left=top=0;

	width = DisplayWidth (display, screen);
	height = DisplayHeight (display, screen);

	Imlib_Image buffer = imlib_load_image (arg);

	if ( !rootimg )
	{
		return 1;
	}

	imlib_context_set_image (buffer);
	imgW = imlib_image_get_width (), imgH = imlib_image_get_height ();

	if (alpha < 255)
	{
		// Create alpha-override mask
		imlib_image_set_has_alpha (1);
		Imlib_Color_Modifier modifier = imlib_create_color_modifier ();
		imlib_context_set_color_modifier (modifier);

		DATA8 red[256], green[256], blue[256], alph[256];
		imlib_get_color_modifier_tables (red, green, blue, alph);

		for (o = 0; o < 256; o++)
			alph[o] = (DATA8) alpha;

		imlib_set_color_modifier_tables (red, green, blue, alph);

		imlib_apply_color_modifier ();
		imlib_free_color_modifier ();
	}

		imlib_context_set_image (rootimg);

	if (mode == Fill)
	{
		imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH,
									0, 0, userW, userH);
	}

	if (mode == Dia)
	{


		if (userW >= width || userH >= height )
		{
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, 0, 0, width, height);
		}
		else if ( userH < height || userW < width )
		{
			int left, top;

			left = (width - userW) / 2;
			top =  (height - userH) / 2;

			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, userW, userH);
		}
	}

	if (mode == Tile)
	{
		int x, y;

		if ( ck0 == 3 )
		{
			left = (width - imgW) / 2;
			top = (height - imgH) / 2;

			for (; left > 0; left -= imgW);
				for (; top > 0; top -= imgH);

			for (x = left; x < width; x += imgW)
				for (y = top; y < height; y += imgH)

			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, imgW, imgH);
		}
		if (ck0 != 3)
		{
			left = (width - userW) / 2;
			top = (height - userH) /2;

			for (; left > 0; left -= userW);
				for (; top > 0; top -= userH);

			for (x = left; x < width; x += userW)
				for (y = top; y < height; y += userH)

			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, userW, userH);
		}
	}

	if (mode == Center)
	{
		left = (width - imgW) / 2;
		top =  (height - imgH) / 2;
		imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, imgW, imgH);
	}

	imlib_context_set_image (buffer);
	imlib_free_image ();

	imlib_context_set_image (rootimg);

	return 0;
}

int findX(char *whereisX, int *rW, int *rH)
{
	char *tok1, *tok2, *saveptr;
	char str1[40];
	int bW, bH;
	strcpy(str1, whereisX);
	tok1 = strtok_r(whereisX, "x", &saveptr);
	tok2 = strtok_r(NULL, "x", &saveptr);

	if ( strcmp(tok1, "0") == 0)
	{
		return 3;
	}
	else if ( tok2 == NULL)
	{
		return 1;
	}
	 else
		{
			tok1 = strtok_r(str1, "x", &saveptr);
			tok2 = strtok_r(NULL, "x", &saveptr);
			bW = atoi(tok1);
		    bH = atoi(tok2);
		/* assigning the results to the output */
           *rW =  bW;
           *rH =  bH;

           return 0;
         }
} //end findX

void checkForNull(char *A1, char *A2) {



	 		if (A1 && *A1  || A2 && *A2)
	 		{
					;
			}
			else
			{	printf("go buy your mom some flowers, come back and try it again \
						user error, %s -- %s\n", A1, A2);

				abort();
			}
			}
