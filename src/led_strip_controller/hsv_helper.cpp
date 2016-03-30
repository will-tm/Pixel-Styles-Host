/*
 * hsv_helper.cpp - Colors manipulations helpers
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
 *
 */

#include "hsv_helper.h"

#include <stdio.h>
#include <boost/lexical_cast.hpp>

rgb_color ColorRed = { 0xFF, 0x00, 0x00, 0xFF };
rgb_color ColorGreen = { 0x00, 0xFF, 0x00, 0xFF };
rgb_color ColorBlue = { 0x00, 0x00, 0xFF, 0xFF };
rgb_color ColorYellow = { 0xFF, 0xFF, 0x00, 0xFF };
rgb_color ColorMagenta = { 0xFF, 0x00, 0xFF, 0xFF };
rgb_color ColorAqua = { 0x00, 0xFF, 0xFF, 0xFF };
rgb_color ColorBlack = { 0x00, 0x00, 0x00, 0xFF };
rgb_color ColorWhite = { 0xFF, 0xFF, 0xFF, 0xFF };
rgb_color ColorClear = { 0x00, 0x00, 0x00, 0x00 };

rgb_color hsv_to_rgb(hsv_color pHsvColor)
{
#define d (255*60)
	
	int a;
	int hh;
	int p, q, t;
	int vs;
	rgb_color rgbColor;
	
	rgbColor.A = pHsvColor.A;
	
	if ((pHsvColor.S == 0) || (pHsvColor.V == 0))
	{
		rgbColor.R = pHsvColor.V;
		rgbColor.G = pHsvColor.V;
		rgbColor.B = pHsvColor.V;
	}
	else
	{
		hh = pHsvColor.H % 360;
		a = hh % 60;
		hh = hh / 60;
		vs = pHsvColor.V * pHsvColor.S;
		p = pHsvColor.V - vs / 255;
		q = pHsvColor.V - (vs * a) / d;
		t = pHsvColor.V - (vs * (60 - a)) / d;
		
		switch (hh)
		{
			case 0:
				rgbColor.R = pHsvColor.V;
				rgbColor.G = t;
				rgbColor.B = p;
			break;
			case 1:
				rgbColor.R = q;
				rgbColor.G = pHsvColor.V;
				rgbColor.B = p;
			break;
			case 2:
				rgbColor.R = p;
				rgbColor.G = pHsvColor.V;
				rgbColor.B = t;
			break;
			case 3:
				rgbColor.R = p;
				rgbColor.G = q;
				rgbColor.B = pHsvColor.V;
			break;
			case 4:
				rgbColor.R = t;
				rgbColor.G = p;
				rgbColor.B = pHsvColor.V;
			break;
			case 5:
				rgbColor.R = pHsvColor.V;
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

rgb_color hue_to_rgb(uint16_t pHue)
{
	hsv_color Color;
	
	Color.H = pHue;
	Color.S = 255;
	Color.V = 255;
	Color.A = 255;
	
	return (hsv_to_rgb(Color));
}

rgb_color hue_float_to_rgb(float &pHue)
{
	hsv_color Color;
	
	while (pHue >= 360.0f)
		pHue = pHue - 360.0f;
	while (pHue < 0.0f)
		pHue = pHue + 360.0f;
	
	Color.H = (uint16_t) pHue;
	Color.S = 255;
	Color.V = 255;
	Color.A = 255;
	
	return (hsv_to_rgb(Color));
}

rgb_color hue_val_float_to_rgb(float &pHue, float &pVal)
{
	hsv_color Color;
	
	while (pHue >= 360.0f)
		pHue = pHue - 360.0f;
	while (pHue < 0.0f)
		pHue = pHue + 360.0f;
	
	Color.H = (uint16_t) pHue;
	Color.S = 255;
	Color.V = pVal;
	Color.A = 255;
	
	return (hsv_to_rgb(Color));
}

hsv_color hue_to_hsv(uint16_t pHue)
{
	hsv_color Color;
	
	Color.H = pHue;
	Color.S = 255;
	Color.V = 255;
	Color.A = 255;
	
	return (Color);
}

uint16_t rgb_to_hue(rgb_color pRgbColor)
{
	return rgb_to_hsv(pRgbColor).H;
}

hsv_color rgb_to_hsv(rgb_color pRgbColor)
{
	int delta;
	int min;
	hsv_color hsvColor;
	
	hsvColor.A = pRgbColor.A;
	
	min = min_of(pRgbColor.R, min_of(pRgbColor.G, pRgbColor.B));
	hsvColor.V = max_of(pRgbColor.R, max_of(pRgbColor.G, pRgbColor.B));
	delta = hsvColor.V - min;
	
	if (hsvColor.V == 0)
		hsvColor.S = 0;
	else hsvColor.S = (delta * 255) / hsvColor.V;
	
	if (hsvColor.S == 0)
		hsvColor.H = 0;
	else
	{
		if (pRgbColor.R == hsvColor.V)
		{
			hsvColor.H = ((pRgbColor.G - pRgbColor.B) * 60) / delta;
		}
		else if (pRgbColor.G == hsvColor.V)
		{
			hsvColor.H = 120 + ((pRgbColor.B - pRgbColor.R) * 60) / delta;
		}
		else if (pRgbColor.B == hsvColor.V)
		{
			hsvColor.H = 240 + ((pRgbColor.R - pRgbColor.G) * 60) / delta;
		}
	}
	if (hsvColor.H < 0)
		hsvColor.H = hsvColor.H + 360;
	
	return hsvColor;
}

rgb_color alpha_blend(rgb_color pColor1, rgb_color pColor2)
{
	rgb_color result;
	
	if (pColor1.A == 0)
		result = pColor2;
	else if (pColor2.A == 0)
		result = pColor1;
	else
	{
		result.A = (uint8_t) (((uint32_t) pColor2.A + (uint32_t) pColor1.A * (255 - (uint32_t) pColor2.A)) / 255);
		result.R = (uint8_t) (((uint32_t) pColor2.R * (uint32_t) pColor2.A + (uint32_t) pColor1.R * (255 - (uint32_t) pColor2.A)) / 255);
		result.G = (uint8_t) (((uint32_t) pColor2.G * (uint32_t) pColor2.A + (uint32_t) pColor1.G * (255 - (uint32_t) pColor2.A)) / 255);
		result.B = (uint8_t) (((uint32_t) pColor2.B * (uint32_t) pColor2.A + (uint32_t) pColor1.B * (255 - (uint32_t) pColor2.A)) / 255);
	}
	//printf("alpha_blend - pColor1={%d %d %d %d} pColor2={%d %d %d %d} result={%d %d %d %d}\n", pColor1.R, pColor1.G, pColor1.B, pColor1.A, pColor2.R, pColor2.G, pColor2.B, pColor2.A, result.R, result.G, result.B, result.A);
	return result;
}

rgb_color inc_hue_of_color(rgb_color pRgbColor, uint16_t pInc)
{
	hsv_color hsv = rgb_to_hsv(pRgbColor);
	hsv.H = (hsv.H + pInc) % 360;
	return hsv_to_rgb(hsv);
}

uint32_t rgb_to_int(rgb_color pRgbColor)
{
	uint32_t *ptr = (uint32_t*)&pRgbColor;
	return (uint32_t)*ptr;
}

rgb_color int_to_rgb(uint32_t pIntColor)
{
	rgb_color *ptr = (rgb_color*)&pIntColor;
	return (rgb_color)*ptr;
}

hsv_color int_to_hsv(uint32_t pIntColor)
{
	return rgb_to_hsv(int_to_rgb(pIntColor));
}

std::string rgb_to_string(rgb_color pRgbColor)
{
	uint32_t *ptr = (uint32_t*)&pRgbColor;
	return boost::lexical_cast<std::string>((uint32_t)*ptr);
}

rgb_color random_rgb()
{
	return hue_to_rgb(rand()%360);
}
