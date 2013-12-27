/*
 * Credits for hsetroot:
 * Authors for hsetroot:
 * Hyriand <hyriand@thegraveyard.org>
 * 
 * everybody that had anything to do with the fluxbox version of bbsetroot
 * everybody that had anything to do with creating imlib2
 * 
 * *****************************************************************
 * 
 * 
 * mhsetroot.c
 * 
 * Copyright 2013 Michael Heras   <userxbw@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 *
 * complie with this line
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

#include "functions.h"



// Globals:
Display *display;
int screen;


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

	
	char str1[40];
	char str2[40];
	char str3[40];
	char str4[40];
	char str5[40];
	int ck0;
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
					if (sscanf (argv[i], "%i", &distance) == 1)
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
			if (sscanf (argv[i], "%i", &angle) == 1)
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
		if ( load_Mod_image(Fill, argv[i], width, height, alpha, image, ck0) == 1)
		{
		  fprintf (stderr, "Bad image (%s)\n", argv[i]);
		  continue;
		}
	 } //added to allow user to set their own image size to the wallpaper
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
				//if format is correct then assign a number for
				//load_Mod_Image to check
				ck0 = -2;
				w = w;
				h = h;
			}
			if( load_Mod_image(Dia, argv[i], w, h, alpha, image, ck0) == 1 )
			{
			fprintf(stderr, "Bad Image or Bad Image Dimensions \n");
			}
	} // modified to allow user the choice of either
	  //either setting image at orginal size or to the users choice
	else if (strcmp (argv[i], "-tile") == 0)
	{
		if ((++i) >= argc)

			{
			fprintf(stderr, "format 0 missing \n");
			continue;
			}
				strcpy (str1, argv[i]);
				strcpy (str2, str1);
				strcpy (str3, str2);
				strcpy (str4, str3);
				strcpy (str5, str4);


			if ( findX(str1, &w, &h) == 3 &&  ((++i) >= argc))
			{ 
				fprintf(stderr, "missing Image\n");
				continue;
			} //check to see if format is -tile 0 
			else if (findX(str2, &w, &h) == 3)
			{
				ck0 = 3;
				if( load_Mod_image(Tile, argv[i], width, height, alpha, image, ck0) == 1 )
				{
					fprintf(stderr, "Bad Image or Bad Image Dimensions \n");
					continue;
				}
			}

			if (findX(str3, &w, &h) == 1)
			{
				fprintf(stderr, "bad format\n");
				continue;
			}
			 if (findX(str4, &w, &h) == 0 && ((++i) >= argc))
			{
				fprintf(stderr, "missing something again\n");
				continue;
			}
			if (findX (str5, &w, &h) == 0 )
			{
				ck0 = 2;
				w = w;
				h = h;
				
			}
			if( load_Mod_image(Tile, argv[i], w, h, alpha, image, ck0) == 1 )
			{
			fprintf(stderr, "Bad Image or Bad Image Dimension\n");
			}

	}

	else if (strcmp (argv[i], "-center") == 0)
	{
		if ((++i) >= argc)
		{
			fprintf (stderr, "Missing image\n");
			continue;
		}
			if (load_Mod_image (Center, argv[i], width, height, alpha, image, ck0) == 1)
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
			if (sscanf (argv[i], "%i", &intval) == 1)
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
			if (sscanf (argv[i], "%i", &intval) == 1)
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
			if (sscanf (argv[i], "%lf", &dblval) == 1)
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
			if (sscanf (argv[i], "%lf", &dblval) == 1)
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
			if (sscanf (argv[i], "%lf", &dblval) == 1)
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
