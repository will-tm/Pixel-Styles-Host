/*
 * hsv_helper.cpp - Colors manipulations helpers
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#include "hsv_helper.h"

rgb_color ColorRed = { 0xFF, 0x00, 0x00, 0x00 };
rgb_color ColorGreen = { 0x00, 0xFF, 0x00, 0x00 };
rgb_color ColorBlue = { 0x00, 0x00, 0xFF, 0x00 };
rgb_color ColorYellow = { 0xFF, 0xFF, 0x00, 0x00 };
rgb_color ColorMagenta = { 0xFF, 0x00, 0xFF, 0x00 };
rgb_color ColorAqua = { 0x00, 0xFF, 0xFF, 0x00 };
rgb_color ColorBlack = { 0x00, 0x00, 0x00, 0x00 };
rgb_color ColorWhite = { 0xFF, 0xFF, 0xFF, 0x00 };

rgb_color hsv_to_rgb(hsv_color hsvColor)
{
#define d (255*60)
	
	int a;
	int hh;
	int p, q, t;
	int vs;
	rgb_color rgbColor;
	
	if ((hsvColor.S == 0) || (hsvColor.V == 0))
	{
		rgbColor.R = hsvColor.V;
		rgbColor.G = hsvColor.V;
		rgbColor.B = hsvColor.V;
	}
	else
	{
		hh = hsvColor.H % 360;
		a = hh % 60;
		hh = hh / 60;
		vs = hsvColor.V * hsvColor.S;
		p = hsvColor.V - vs / 255;
		q = hsvColor.V - (vs * a) / d;
		t = hsvColor.V - (vs * (60 - a)) / d;
		
		switch (hh)
		{
			case 0:
				rgbColor.R = hsvColor.V;
				rgbColor.G = t;
				rgbColor.B = p;
			break;
			case 1:
				rgbColor.R = q;
				rgbColor.G = hsvColor.V;
				rgbColor.B = p;
			break;
			case 2:
				rgbColor.R = p;
				rgbColor.G = hsvColor.V;
				rgbColor.B = t;
			break;
			case 3:
				rgbColor.R = p;
				rgbColor.G = q;
				rgbColor.B = hsvColor.V;
			break;
			case 4:
				rgbColor.R = t;
				rgbColor.G = p;
				rgbColor.B = hsvColor.V;
			break;
			case 5:
				rgbColor.R = hsvColor.V;
				rgbColor.G = p;
				rgbColor.B = q;
			break;
			default:
				rgbColor.R = 0;
				rgbColor.G = 0;
				rgbColor.B = 0;
			break;
		}
	}
	
	return rgbColor;
}

rgb_color hue_to_rgb(uint16_t Hue)
{
	hsv_color Color;
	
	Color.H = Hue;
	Color.S = 255;
	Color.V = 255;
	
	return (hsv_to_rgb(Color));
}

rgb_color hue_float_to_rgb(float &Hue)
{
	hsv_color Color;
	
	while (Hue >= 360.0f)
		Hue = Hue - 360.0f;
	while (Hue < 0.0f)
		Hue = Hue + 360.0f;
	
	Color.H = (uint16_t) Hue;
	Color.S = 255;
	Color.V = 255;
	
	return (hsv_to_rgb(Color));
}

rgb_color hue_val_float_to_rgb(float &Hue, float &Val)
{
	hsv_color Color;
	
	while (Hue >= 360.0f)
		Hue = Hue - 360.0f;
	while (Hue < 0.0f)
		Hue = Hue + 360.0f;
	
	Color.H = (uint16_t) Hue;
	Color.S = 255;
	Color.V = Val;
	
	return (hsv_to_rgb(Color));
}

hsv_color hue_to_hsv(uint16_t Hue)
{
	hsv_color Color;
	
	Color.H = Hue;
	Color.S = 255;
	Color.V = 255;
	
	return (Color);
}

uint16_t rgb_to_hue(rgb_color rgbColor)
{
	return rgb_to_hsv(rgbColor).H;
}

hsv_color rgb_to_hsv(rgb_color rgbColor)
{
	int delta;
	int min;
	hsv_color hsvColor;
	
	min = min_of(rgbColor.R, min_of(rgbColor.G, rgbColor.B));
	hsvColor.V = max_of(rgbColor.R, max_of(rgbColor.G, rgbColor.B));
	delta = hsvColor.V - min;
	
	if (hsvColor.V == 0)
		hsvColor.S = 0;
	else hsvColor.S = (delta * 255) / hsvColor.V;
	
	if (hsvColor.S == 0)
		hsvColor.H = 0;
	else
	{
		if (rgbColor.R == hsvColor.V)
		{
			hsvColor.H = ((rgbColor.G - rgbColor.B) * 60) / delta;
		}
		else if (rgbColor.G == hsvColor.V)
		{
			hsvColor.H = 120 + ((rgbColor.B - rgbColor.R) * 60) / delta;
		}
		else if (rgbColor.B == hsvColor.V)
		{
			hsvColor.H = 240 + ((rgbColor.R - rgbColor.G) * 60) / delta;
		}
	}
	if (hsvColor.H < 0)
		hsvColor.H = hsvColor.H + 360;
	
	return hsvColor;
}
