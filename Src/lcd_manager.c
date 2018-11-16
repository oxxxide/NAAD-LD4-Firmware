/*
 * lcd_manager.c
 *
 *  Created on: 2018/06/23
 *      Author: NishiAsakusa Audio Developments / oxxxide / Akikazu Iwasa
 */

#include "lcd_manager.h"
#include <string.h>

volatile LCDBUFF lcd_text_buf1;
volatile LCDBUFF lcd_text_buf2;

const char* LCDM_EDIT_GROUP_TEXT[NUM_OF_GROUP] = {"OSCILATOR","AMPLIFIER","MODULATOR","BEND","NOISE","FILTER","LFO"};

const char* LCDM_PARAMETER_TEXT[NUM_OF_GROUP][NUM_OF_COLUMN] = {
		{ "Wave", "Pitch", "Mod-Depth", "Mod-Type" },
		{ "Level", "Attack", "Hold", "Release" },
		{ "Frequency", "Attack", "Hold", "Release" },
		{ "Amount", "Attack", "Hold", "Release" },
		{ "Level", "Attack", "Hold", "Release" },
		{ "Type", "Cutoff", "Resonance", "Env-Amount" },
		{ "Dest", "Wave", "Speed", "Depth" }
};

const char* LCDM_PARAMETER_TEXT_SHIFT[NUM_OF_GROUP][NUM_OF_COLUMN] = {
		{ "Wave", "Fine Tune", "Mod-Depth", "Mod-Type" },
		{ "PAN", "Attack", "Slope", "Release" },
		{ "Frequency", "Attack", "Slope", "Release" },
		{ "Vel-Sense", "Attack", "Slope", "Release" },
		{ "Level", "Attack", "Slope", "Release" },
		{ "Type", "Cutoff", "Resonance", "Env-Decay" },
		{ "Dest", "Wave", "Speed", "Depth" }
};

void InitLcdManager() {
	static uint8_t tb1[17];
	static uint8_t tb2[17];
	lcd_text_buf1.dirt = 0;
	lcd_text_buf1.text = (char*)tb1;
	lcd_text_buf1.length = 0;
	lcd_text_buf2.dirt = 0;
	lcd_text_buf2.text = (char*)tb2;
	lcd_text_buf2.length = 0;
}

void lcdWriteText(int row, const char* str,int length) {
	switch (row) {
	case 0:
		memcpy(lcd_text_buf1.text, str,length);
		lcd_text_buf1.length = length;
		lcd_text_buf1.dirt++;
		break;
	case 1:
		memcpy(lcd_text_buf2.text, str,length);
		lcd_text_buf2.length = length;
		lcd_text_buf2.dirt++;
		break;
	}
}
