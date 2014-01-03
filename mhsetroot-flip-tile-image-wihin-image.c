
/* complie with this line
 *
 * `imlib2-config --cflags` `imlib2-config --libs`
 *
 * */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <Imlib2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../config/functions.h"



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
         "Solid:\n"
        " -solid <color>             Render a solid using the specified color\n"
        "\n"
            "Dia:\n"
        " -dia <diamentions>         Renders to user spects within monitor\n"
        "                            settings - Format  100x100\n"
        "\n"
        "Tile:\n"
        "-tile <image>                Renders image at orginal size\n"
        "\n"
        "-tilex <dimentions> <image>  Renders image at users set size\n"
        "                                  Format <size>x<size>\n"
        "-tileh <dimentions> <image> Renders tiled image within image\n"
        "                            horizontally\n"
        "-tilev <dimentions> <image> Renders tiled image within image\n"
        "                            vertically\n"
        "-tilet <dimentions> <image> Renders tiled image within image\n"
        "\n"
        "Image files:\n"
        " -center <image>            Render an image centered on screen\n"
        " -tile <image>              Render an image tiled\n"
        " -full <image>              Render an image maximum aspect\n"
        " -fill <image>              Render an image strechted\n"
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


 void setAlpha(Imlib_Image rootimg,  int alp){
	 
	 int o;
	 
	 if (alp < 255)
	{
		// Create alpha-override mask
		imlib_image_set_has_alpha (1);
		Imlib_Color_Modifier modifier = imlib_create_color_modifier ();
		imlib_context_set_color_modifier (modifier);

		DATA8 red[256], green[256], blue[256], alph[256];
		imlib_get_color_modifier_tables (red, green, blue, alph);
		
		for (o = 0; o < 256; o++)
			alph[o] = (DATA8) alp;
			
		imlib_set_color_modifier_tables (red, green, blue, alph);
		
		imlib_apply_color_modifier ();
		imlib_free_color_modifier ();
	}
	
		imlib_context_set_image (rootimg);
				
}  // end setAlpha
	
	

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


typedef enum
{ Fill, Full, Center, Tile, Dia, Tilex, Tileh, Tilev, Tilet,
	FlipImgH, FlipImgV, FlipImgD } ImageMode;


typedef struct
{
	int r, g, b, a;
}	Color, *PColor;

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

int load_image (ImageMode mode, const char *arg, int rootW, int rootH,
				int alpha, Imlib_Image rootimg)
{
	int imgW, imgH, o;
	Imlib_Image buffer = imlib_load_image (arg);
	
	if (!buffer)
		return 1;
	
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
									0, 0, rootW, rootH);
	}
	
	else if (mode == Full)
	{
		double aspect = ((double) rootW) / imgW;
		int top, left;
		
		if ((int) (imgH * aspect) > rootH)
			aspect = (double) rootH / (double) imgH;
			top = (rootH - (int) (imgH * aspect)) / 2;
			left = (rootW - (int) (imgW * aspect)) / 2;
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH,
											left, top, (int) (imgW * aspect),
											(int) (imgH * aspect));
	}
	else
	{
	
		int left = (rootW - imgW) / 2, top = (rootH - imgH) / 2;
	

	if (mode == Tile)
	{
		int x, y;
		
		for (; left > 0; left -= imgW);
			for (; top > 0; top -= imgH);
			
		for (x = left; x < rootW; x += imgW)
			for (y = top; y < rootH; y += imgH)
		
	imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, imgW, imgH);
	}
	else
	{
		imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, imgW, imgH);
	} // end second else
  } // end first else

	imlib_context_set_image (buffer);
	imlib_free_image ();
	
	imlib_context_set_image (rootimg);
	
	return 0;
} // end function


/*****************************
 * 
 * my modified code
 * 
 * **********************/

int load_Mod_image (ImageMode mode, const char *arg, int rootW, int rootH,
				int alpha, Imlib_Image rootimg)
{
	
	int left, top, x, y;
	int width, height; 
	int imgW, imgH, o;
	
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
	
	
	if (mode == Dia)
	{
		
		
		if (rootW >= width || rootH >= height ) 
		{
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, 0, 0, width, height);
		}
		else if ( rootH < height || rootW < width )
		{
			
			
			left = (width - rootW) / 2; 
			top =  (height - rootH) / 2; 
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, rootW, rootH);
		}
	}
	
	// new code for tiling image
	
	
	
	if (mode == Tilex)
	{
		
		left = (width - rootW) / 2;
			top = (height - rootH) /2;
			
			for (; left > 0; left -= rootW);
				for (; top > 0; top -= rootH);

			for (x = left; x < width; x += rootW)
				for (y = top; y < height; y += rootH)
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, rootW, rootH);
     }
      
    // end new code for tiling image

	imlib_context_set_image (buffer);
	imlib_free_image ();
	
	imlib_context_set_image (rootimg);
	
	return 0;
} 

int tile_in_tile (ImageMode mode, const char *arg, int rootW, int rootH, int alpha, Imlib_Image rootimg) {
				
				
	int left, top, x, y;
	int width, height; 
	int imgW, imgH;
	
	
	Imlib_Image buffer = imlib_load_image (arg);
	
	if (!buffer)
		return 1;
	
	imlib_context_set_image (buffer);
	imgW = imlib_image_get_width (), imgH = imlib_image_get_height ();
				
	
	
	width = DisplayWidth (display, screen);
	height = DisplayHeight (display, screen);
	//sets image to work on	
	imlib_context_set_image (buffer);
	
	

	if (mode == Tileh)
	{
		
		imlib_image_tile_horizontal();
		setAlpha(rootimg, alpha);
		
		left = (width - rootW) / 2;
			top = (height - rootH) /2;
			
			for (; left > 0; left -= rootW);
				for (; top > 0; top -= rootH);

			for (x = left; x < width; x += rootW)
				for (y = top; y < height; y += rootH)
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, rootW, rootH);
     }
     
     
	if (mode == Tilev)
	{
		imlib_image_tile_vertical();
		setAlpha(rootimg, alpha);
		
		left = (width - rootW) / 2;
			top = (height - rootH) /2;
			
			for (; left > 0; left -= rootW);
				for (; top > 0; top -= rootH);

			for (x = left; x < width; x += rootW)
				for (y = top; y < height; y += rootH)
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, rootW, rootH);
     }
      
   if (mode == Tilet)
	{
		imlib_image_tile();
		setAlpha(rootimg, alpha);
		
		left = (width - rootW) / 2;
			top = (height - rootH) /2;
			
			for (; left > 0; left -= rootW);
				for (; top > 0; top -= rootH);

			for (x = left; x < width; x += rootW)
				for (y = top; y < height; y += rootH)
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, x, y, rootW, rootH);
     }
      

	imlib_context_set_image (buffer);
	imlib_free_image ();
	
	imlib_context_set_image (rootimg);
	
	return 0;
}

int  flip_image_within_screen  (ImageMode mode, const char *arg, int rootW, int rootH, int alpha, Imlib_Image rootimg) {
	
	
	
	int width, height, left, top; 
	int imgW, imgH; 	
	width = DisplayWidth (display, screen);
	height = DisplayHeight (display, screen);
	
	
	Imlib_Image buffer = imlib_load_image (arg);
	
	if (!buffer)
		return 1;
	
	imlib_context_set_image (buffer);
	imgW = imlib_image_get_width (), imgH = imlib_image_get_height ();
	
	
	/*

 	Flips/mirrors the current image diagonally (good for quick and dirty 90 degree rotations if used before to after a horizontal or vertical flip).
EAPI void 	imlib_image_orientate (int orientation)

*/

	// for user requesed RE- Size of image then fips the images according to angle
	
	// flip image horzontal
	if (mode == FlipImgH)
	{
		imlib_image_flip_horizontal ();
		setAlpha(rootimg, alpha);
		
		
		if (rootW >= width || rootH >= height ) 
		{
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, 0, 0, width, height);
		}
		else if ( rootH < height || rootW < width )
		{
			
			
			left = (width - rootW) / 2; 
			top =  (height - rootH) / 2; 
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, rootW, rootH);
		}
	}
	
	// flips image vertically
		if (mode == FlipImgV)
	{
		 imlib_image_flip_vertical ();
		 setAlpha(rootimg, alpha);
		
		
		if (rootW >= width || rootH >= height ) 
		{
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, 0, 0, width, height);
		}
		else if ( rootH < height || rootW < width )
		{
			
			
			left = (width - rootW) / 2; 
			top =  (height - rootH) / 2; 
			
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, rootW, rootH);
		}
	}
	
	
	// flips image diagonally
		if (mode == FlipImgD)
		{
		 	imlib_image_flip_diagonal ();
			setAlpha(rootimg, alpha);
		
		
			if (rootW >= width || rootH >= height ) 
			{
				imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, 0, 0, width, height);
			}
			else if ( rootH < height || rootW < width )
			{
			
			// reverced rootW rootH
			left = (width - rootH) / 2; 
			top =  (height - rootW) / 2; 
			                                  
			imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, rootH, rootW); // reverced rootW and rootH
			}
		}
	
	// if orginal size then flip it what ever direction needed 
//	imlib_blend_image_onto_image (buffer, 0, 0, 0, imgW, imgH, left, top, imgW, imgH);



	imlib_context_set_image (buffer);
	imlib_free_image ();
	
	imlib_context_set_image (rootimg);

	return 0;
} // end flip_image_within_screen   function

int findX(char *whereisX, int *rW, int *rH)
{
	
	char *tok1, *tok2, *saveptr;
	char x = (int) malloc(sizeof(whereisX));
	char str1[x];
	int bW, bH;
	
	strcpy(str1, whereisX);
	
	tok1 = strtok_r(whereisX, "x", &saveptr);
	tok2 = strtok_r(NULL, "x", &saveptr);
	
	if ( tok2 == NULL)
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
           *rW = bW;
           *rH = bH;
           
           return 0;
         }
} //end findX
	

// flip image which way 

int flip_Image_which_way(char *angle, char *xy, int *rW, int *rH)
{ 
	char *tok1, *tok2, *saveptr;
	char str1[40];
	int bW, bH;
	strcpy(str1, xy);
	tok1 = strtok_r(xy, "x", &saveptr);
	tok2 = strtok_r(NULL, "x", &saveptr);
	
	
	
	
	if ( strcmp(angle, "h") == 0)
	{	
		
		
		tok1 = strtok_r(str1, "x", &saveptr);
			tok2 = strtok_r(NULL, "x", &saveptr);
			bW = atoi(tok1);
		    bH = atoi(tok2);
		 
		/* assigning the results to the output */
           *rW =  bW;
           *rH =  bH;
           
			return 4;
		
		
		
		
	}
	if (strcmp(angle, "v") == 0 )
	{
		tok1 = strtok_r(str1, "x", &saveptr);
			tok2 = strtok_r(NULL, "x", &saveptr);
			bW = atoi(tok1);
		    bH = atoi(tok2);
		/* assigning the results to the output */
           *rW =  bW;
           *rH =  bH;
           
		return 5;
	}
	if (strcmp(angle, "d" ) == 0 )
	{
		tok1 = strtok_r(str1, "x", &saveptr);
			tok2 = strtok_r(NULL, "x", &saveptr);
			bW = atoi(tok1);
		    bH = atoi(tok2);
		/* assigning the results to the output */
           *rW =  bW;
           *rH =  bH;
           
		return 6;
	}
	else
	
		return 1; // errored out no good kill it 
	
} //end flip image which way


int main (int argc, char **argv)
{
	Visual *vis;
	Colormap cm;
	Display *_display;
	Imlib_Context *context;
	Imlib_Image image;
	Pixmap pixmap;
	Imlib_Color_Modifier modifier = NULL;
	_display = XOpenDisplay (NULL);
	int width, height, depth, i, alpha;
	
	//char *tok1, *tok2, *saveptr;
	char str1[40];
	char str2[40];
	char str3[40];
	char str4[40];
		
	int w, h;
	w = 0;
	h = 0;
	

	for (screen = 0; screen < ScreenCount (_display); screen++)
	{
		display = XOpenDisplay (NULL);
		
		context = imlib_context_new ();
		imlib_context_push (context);
		
		imlib_context_set_display (display);
		vis = DefaultVisual (display, screen);
		cm = DefaultColormap (display, screen);
		width = DisplayWidth (display, screen);
		height = DisplayHeight (display, screen);
		depth = DefaultDepth (display, screen);
		
		pixmap =
			XCreatePixmap (display, RootWindow (display, screen),
							width, height, depth);
		
		imlib_context_set_visual (vis);
		imlib_context_set_colormap (cm);
		imlib_context_set_drawable (pixmap);
		imlib_context_set_color_range (imlib_create_color_range ());
		
		image = imlib_create_image (width, height);
		imlib_context_set_image (image);
		
		imlib_context_set_color (0, 0, 0, 255);
		imlib_image_fill_rectangle (0, 0, width, height);
		
		imlib_context_set_dither (1);
		imlib_context_set_blend (1);
		
		alpha = 255;


	for (i = 1; i < argc; i++)
	{
		if (modifier != NULL)
		{
			imlib_apply_color_modifier ();
			imlib_free_color_modifier ();
		}
		
	modifier = imlib_create_color_modifier ();
	imlib_context_set_color_modifier (modifier);
		
		if (strcmp (argv[i], "-alpha") == 0)
		{
			if ((++i) >= argc)
			{
				fprintf (stderr, "Missing alpha\n");
				continue;
			}
				if (sscanf (argv[i], "%i", &alpha) == 0)
				{
					fprintf (stderr, "Bad alpha (%s)\n", argv[i]);
					continue;
				}
		}
		else if (strcmp (argv[i], "-solid") == 0)
		{
			Color c;
			
			if ((++i) >= argc)
			{
				fprintf (stderr, "Missing color\n");
				continue;
			}
				if (parse_color (argv[i], &c, alpha) == 1)
				{
					fprintf (stderr, "Bad color (%s)\n", argv[i]);
					continue;
				}
				
			imlib_context_set_color (c.r, c.g, c.b, c.a);
			imlib_image_fill_rectangle (0, 0, width, height);
	}
	else if (strcmp (argv[i], "-clear") == 0)
	{
		imlib_free_color_range ();
		imlib_context_set_color_range (imlib_create_color_range ());
	}
	else if (strcmp (argv[i], "-add") == 0)
	{
		Color c;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing color\n");
			continue;
		}
			if (parse_color (argv[i], &c, alpha) == 1)
			{
				fprintf (stderr, "Bad color (%s)\n", argv[i - 1]);
				continue;
			}
			
		imlib_context_set_color (c.r, c.g, c.b, c.a);
		imlib_add_color_to_color_range (1);
	}
	else if (strcmp (argv[i], "-addd") == 0)
	{
		Color c;
		int distance;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing color\n");
			continue;
		}
			if ((++i) >= argc)
			{
				fprintf (stderr, "Missing distance\n");
				continue;
			}
				if (parse_color (argv[i - 1], &c, alpha) == 1)
				{
					fprintf (stderr, "Bad color (%s)\n", argv[i - 1]);
					continue;
				}
					if (sscanf (argv[i], "%i", &distance) == 0)
					{
						fprintf (stderr, "Bad distance (%s)\n", argv[i]);
						continue;
					}
					
				imlib_context_set_color (c.r, c.g, c.b, c.a);
				imlib_add_color_to_color_range (distance);
	}
	else if (strcmp (argv[i], "-gradient") == 0)
	{
		int angle;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing angle\n");
			continue;
		}
			if (sscanf (argv[i], "%i", &angle) == 0)
			{
				fprintf (stderr, "Bad angle (%s)\n", argv[i]);
				continue;
			}
			
		imlib_image_fill_color_range_rectangle (0, 0, width, height,
												angle);
	} 
	
	   else if (strcmp (argv[i], "-fill") == 0)
          {
            if ((++i) >= argc)
            {
              fprintf (stderr, "Missing image\n");
              continue;
            }
            if (load_image (Fill, argv[i], width, height, alpha, image) ==
              1)
            {
              fprintf (stderr, "Bad image (%s)\n", argv[i]);
              continue;
            }
          }
	
		else if (strcmp (argv[i], "-full") == 0)
        {
			if ((++i) >= argc)
            {
              fprintf (stderr, "Missing image\n");
              continue;
            }
            if (load_image (Full, argv[i], width, height, alpha, image) ==
              1)
            {
              fprintf (stderr, "Bad image (%s)\n", argv[i]);
              continue;
            }
          }
		//Start my code 
	//###############################

	
	else if (strcmp (argv[i], "-dia") == 0)
	{
		if((++i) >= argc)
		{
			fprintf(stderr, "missing Dia, and Image\n");
			continue;
		}	
			strcpy (str1, argv[i]);
			strcpy (str2, str1);
			
			if ( findX(str1, &w, &h) == 1 )
			{
				fprintf(stderr, " Bad Format\n");
				continue;
			}
			else if (findX(str2, &w, &h) == 0 && ((++i) >= argc))
			{
				fprintf(stderr, "Missing Image\n");
				continue;
			}
			else  
			{
				w = w;
				h = h;
			}
			if( load_Mod_image(Dia, argv[i], w, h, alpha, image) == 1 )
			{
			fprintf(stderr, "Bad Image or Bad Image Dimensions \n");
			}
	}
	 // start tTilex
	else if (strcmp (argv[i], "-tileh") == 0)
	{
		printf("in tileh\n");
		if ((++i) >= argc)

			{
			fprintf(stderr, " a Bad format missing args\n");
			continue;
			}
				strcpy (str1, argv[i]);
				strcpy (str2, str1);
				strcpy (str3, str2);
				strcpy (str4, str3);
							
				if (findX(str1, &w, &h) == 0 && ((++i) >= argc))
				{
					fprintf(stderr, "Bad Format: try -tilex <dimensions> <image>\n");
					continue;
				}
					if (findX(str2, &w, &h) == 1 && ((++i) >= argc))
					{
						fprintf(stderr, "Bad Format\n");
						continue;
					}
						if (findX (str3, &w,&h) == 1 && ((++i) >=argc) )
						{ 
							fprintf(stderr, "Bad Format %s\n", argv[i-2]); 
							continue;
						}
							if (findX (str4, &w, &h) == 0 )
							{ //sets users input Dimension for image
								w = w; //set str3 W
								h = h; //set str3 H
							}
			if( tile_in_tile(Tileh, argv[i], w, h, alpha, image ) == 1 )
			{
			fprintf(stderr, "Bad Image <%s> \n", argv[i]);
			}

	} // end tTilex
	
	else if (strcmp (argv[i], "-tilev") == 0)
	{
		printf("in tileh\n");
		if ((++i) >= argc)

			{
			fprintf(stderr, " a Bad format missing args\n");
			continue;
			}
				strcpy (str1, argv[i]);
				strcpy (str2, str1);
				strcpy (str3, str2);
				strcpy (str4, str3);
							
				if (findX(str1, &w, &h) == 0 && ((++i) >= argc))
				{
					fprintf(stderr, "Bad Format: try -tilex <dimensions> <image>\n");
					continue;
				}
					if (findX(str2, &w, &h) == 1 && ((++i) >= argc))
					{
						fprintf(stderr, "Bad Format\n");
						continue;
					}
						if (findX (str3, &w,&h) == 1 && ((++i) >=argc) )
						{ 
							fprintf(stderr, "Bad Format %s\n", argv[i-2]); 
							continue;
						}
							if (findX (str4, &w, &h) == 0 )
							{ //sets users input Dimension for image
								w = w; //set str3 W
								h = h; //set str3 H
							}
			if( tile_in_tile(Tilev, argv[i], w, h, alpha, image ) == 1 )
			{
			fprintf(stderr, "Bad Image <%s> \n", argv[i]);
			}

	} // end tTilev
	
	else if (strcmp (argv[i], "-tilet") == 0)
	{
		printf("in tileh\n");
		if ((++i) >= argc)

			{
			fprintf(stderr, " a Bad format missing args\n");
			continue;
			}
				strcpy (str1, argv[i]);
				strcpy (str2, str1);
				strcpy (str3, str2);
				strcpy (str4, str3);
							
				if (findX(str1, &w, &h) == 0 && ((++i) >= argc))
				{
					fprintf(stderr, "Bad Format: try -tilex <dimensions> <image>\n");
					continue;
				}
					if (findX(str2, &w, &h) == 1 && ((++i) >= argc))
					{
						fprintf(stderr, "Bad Format\n");
						continue;
					}
						if (findX (str3, &w,&h) == 1 && ((++i) >=argc) )
						{ 
							fprintf(stderr, "Bad Format %s\n", argv[i-2]); 
							continue;
						}
							if (findX (str4, &w, &h) == 0 )
							{ //sets users input Dimension for image
								w = w; //set str3 W
								h = h; //set str3 H
							}
			if( tile_in_tile(Tilet, argv[i], w, h, alpha, image ) == 1 )
			{
			fprintf(stderr, "Bad Image <%s> \n", argv[i]);
			}

	} // end tTilet
	
	
	// start of tilex
	
	else if (strcmp (argv[i], "-tilex") == 0)
	{
		
		if ((++i) >= argc)

			{
			fprintf(stderr, " a Bad format missing args\n");
			continue;
			}
				strcpy (str1, argv[i]);
				strcpy (str2, str1);
				strcpy (str3, str2);
				strcpy (str4, str3);
							
				if (findX(str1, &w, &h) == 0 && ((++i) >= argc))
				{
					fprintf(stderr, "Bad Format: try -tilex <dimensions> <image>\n");
					continue;
				}
					if (findX(str2, &w, &h) == 1 && ((++i) >= argc))
					{
						fprintf(stderr, "Bad Format\n");
						continue;
					}
						if (findX (str3, &w,&h) == 1 && ((++i) >=argc) )
						{ 
							fprintf(stderr, "Bad Format %s\n", argv[i-2]); 
							continue;
						}
							if (findX (str4, &w, &h) == 0 )
							{ //sets users input Dimension for image
								w = w; //set str3 W
								h = h; //set str3 H
							}
			if( load_Mod_image(Tilex, argv[i], w, h, alpha, image ) == 1 )
			{
			fprintf(stderr, "Bad Image <%s> \n", argv[i]);
			}

	}
	
	// end of -tilex
	
	//                START 
	//########################################################
	//########################################################
	//########################################################
	
	else if (strcmp (argv[i], "-flipimg") == 0)
	{
		if ((++i) >= argc  )

			{
			fprintf(stderr, " BAD BAD FORMAT\n");
			continue;
			}
				if ((++i) >= argc )
				{
					fprintf(stderr, "BAD FORMAT BAD\n");
					continue;
				}
					if ((++i) >= argc && argc + 1 )
					{
						fprintf(stderr, "NEED BETTER FORMAT CHECK HELP\n");
						continue;
					}
		
				strcpy (str1, argv[i - 2]);
				strcpy (str2, str1);
				strcpy (str3, str2);
						
				char dia1[40];
				char dia2[40];
				char dia3[40];
								
				strcpy(dia1, argv[i - 1]);
				strcpy(dia2, dia1);
				strcpy(dia3,dia2);
				
				
	 if (flip_Image_which_way(str1, dia1 , &w, &h) == 4)
	{ 
			w = w;
			h = h;
		
		if( flip_image_within_screen(FlipImgH, argv[i], w, h, alpha, image) == 1 )
		{
			fprintf(stderr, "Bad Image |%s|\n", argv[i]);
			continue;
		}
	}
		 if (flip_Image_which_way(str2, dia2 , &w, &h) == 5)
		{ 
			w = w;
			h = h;
		
			if( flip_image_within_screen(FlipImgV, argv[i], w, h, alpha, image) == 1 )
			{
				fprintf(stderr, "Bad Image |%s|\n", argv[i]);
				continue;
			}
		}
			 if (flip_Image_which_way(str3, dia3 , &w, &h) == 6)
			{ 
				w = w;
				h = h;
		
				if( flip_image_within_screen(FlipImgD, argv[i], w, h, alpha, image) == 1 )
				{
					fprintf(stderr, "Bad Image |%s|\n", argv[i]);
					continue;
				}
			}
	}

//###################################################################
//###################################################################
//###################################################################


	else if (strcmp (argv[i], "-tile") == 0)
	{
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing image\n");
			continue;
		}
			if (load_image (Tile, argv[i], width, height, alpha, image) == 1)
			{
				fprintf (stderr, "Bad image (%s)\n", argv[i]);
				continue;
			}
	}
	else if (strcmp (argv[i], "-center") == 0)
	{
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing image\n");
			continue;
		}
			if (load_image (Center, argv[i], width, height, alpha, image) == 1 )
			{
				fprintf (stderr, "Bad image (%s)\n", argv[i]);
				continue;
			}
	}
	else if (strcmp (argv[i], "-tint") == 0)
	{
		Color c;
		DATA8 r[256], g[256], b[256], a[256];
		int j;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing color\n");
			continue;
		}
			if (parse_color (argv[i], &c, 255) == 1)
			{
				fprintf (stderr, "Bad color\n");
				continue;
			}

		imlib_get_color_modifier_tables (r, g, b, a);
			
			for (j = 0; j < 256; j++)
			{
				r[j] = (DATA8) (((double) r[j] / 255.0) * (double) c.r);
				g[j] = (DATA8) (((double) g[j] / 255.0) * (double) c.g);
				b[j] = (DATA8) (((double) b[j] / 255.0) * (double) c.b);
			}
			
		imlib_set_color_modifier_tables (r, g, b, a);
	}
	else if (strcmp (argv[i], "-blur") == 0)
	{
		int intval;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing value\n");
			continue;
		}
			if (sscanf (argv[i], "%i", &intval) == 0)
			{
				fprintf (stderr, "Bad value (%s)\n", argv[i]);
				continue;
			}
		imlib_image_blur (intval);
	}
	else if (strcmp (argv[i], "-sharpen") == 0)
	{
		int intval;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing value\n");
			continue;
		}
			if (sscanf (argv[i], "%i", &intval) == 0)
			{
				fprintf (stderr, "Bad value (%s)\n", argv[i]);
				continue;
			}
		imlib_image_sharpen (intval);
	}
	else if (strcmp (argv[i], "-contrast") == 0)
	{
		double dblval;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing value\n");
			continue;
		}
			if (sscanf (argv[i], "%lf", &dblval) == 0)
			{
				fprintf (stderr, "Bad value (%s)\n", argv[i]);
				continue;
			}
		imlib_modify_color_modifier_contrast (dblval);
	}
	else if (strcmp (argv[i], "-brightness") == 0)
	{
		double dblval;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing value\n");
			continue;
		}
			if (sscanf (argv[i], "%lf", &dblval) == 0)
			{
				fprintf (stderr, "Bad value (%s)\n", argv[i]);
				continue;
			}
		imlib_modify_color_modifier_brightness (dblval);
	}
	else if (strcmp (argv[i], "-gamma") == 0)
	{
		double dblval;
		
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing value\n");
			continue;
		}
			if (sscanf (argv[i], "%lf", &dblval) == 0)
			{
				fprintf (stderr, "Bad value (%s)\n", argv[i]);
				continue;
			}
		imlib_modify_color_modifier_gamma (dblval);
	}
	else if (strcmp (argv[i], "-flipv") == 0)
	{
		imlib_image_flip_vertical ();
	}
	else if (strcmp (argv[i], "-fliph") == 0)
	{
		imlib_image_flip_horizontal ();
	}
	else if (strcmp (argv[i], "-flipd") == 0)
	{
		imlib_image_flip_diagonal ();
	}
	else if (strcmp (argv[i], "-write") == 0)
	{
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing filename\n");
			continue;
		}
      imlib_save_image (argv[i]);
	}
	else
	{
		usage (argv[0]);
		imlib_free_image ();
		imlib_free_color_range ();
		
			if (modifier != NULL)
			{
				imlib_context_set_color_modifier (modifier);
				imlib_free_color_modifier ();
				modifier = NULL;
			}
				XFreePixmap (display, pixmap);
				exit (1);
	} // end else
} // end loop off of argc

	if (modifier != NULL)
	{
		imlib_context_set_color_modifier (modifier);
        imlib_apply_color_modifier ();
        imlib_free_color_modifier ();
        modifier = NULL;
	}
	
		imlib_render_image_on_drawable (0, 0);
		imlib_free_image ();
		imlib_free_color_range ();
		
		if (setRootAtoms (pixmap) == 0)
			fprintf (stderr, "Couldn't create atoms...\n");
		
		XKillClient (display, AllTemporary);
		XSetCloseDownMode (display, RetainTemporary);
		
		XSetWindowBackgroundPixmap (display, RootWindow (display, screen), 
									pixmap);
		
		XClearWindow (display, RootWindow (display, screen));
		
		XFlush (display);
		XSync (display, False);
		
		imlib_context_pop ();
		imlib_context_free (context);
		
	} // end for loop off screen  

  return 0;
}
