/*
 * hsv_helper.c - Colors manipulations helpers
 *
 * Copyright (C) 2013 William Markezana <william.markezana@me.com>
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
 */

#ifndef __HSV_HELPER_H__
#define __HSV_HELPER_H__

#include <stdlib.h>
#include <stdint.h>

#if !defined(min_of)
    #define min_of(A,B)	( A < B ? A : B )
#endif

#if !defined(max_of)
    #define max_of(A,B)	( A < B ? B : A )
#endif

typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t reserved;
}rgb_color;

typedef struct
{
	uint16_t H;
	uint8_t  S;
	uint8_t  V;
}hsv_color;

rgb_color hsv_to_rgb(hsv_color hsvColor);
rgb_color hue_to_rgb(uint16_t Hue);
rgb_color hue_float_to_rgb(float &Hue);
rgb_color hue_val_float_to_rgb(float &Hue, float &Sat);
hsv_color hue_to_hsv(uint16_t Hue);
uint16_t rgb_to_hue(rgb_color rgbColor);
hsv_color rgb_to_hsv(rgb_color rgbColor);

extern rgb_color ColorRed;
extern rgb_color ColorGreen;
extern rgb_color ColorBlue;
extern rgb_color ColorYellow;
extern rgb_color ColorMagenta;
extern rgb_color ColorAqua;
extern rgb_color ColorBlack;
extern rgb_color ColorWhite;


#endif
