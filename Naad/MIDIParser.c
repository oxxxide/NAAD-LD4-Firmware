/*
 * MIDIParser.c
 *
 *  Created on: 2018/09/26
 *      Author: NishiAsakusa Audio Developments / oxxxide / Akikazu Iwasa
 */

#include "MIDIParser.h"

static void parseSignal(uint8_t b);


uint32_t calcTicksForOneClock(uint32_t bpm){
	return (60.0f / (float)bpm / 24.0f)* 10000;
}

void MidiParser_PushByte(uint8_t byte) {

	parseSignal(byte);
}

void parseSignal(uint8_t b) {

	uint8_t status = b >> 4;
	static uint8_t firstByte = 0;
	static uint8_t secondByte = 0;
	static unsigned int counter = 0;
	static uint8_t rawBytes[3];

	if (status & 0b1000) {
		//Status Byte
		if (counter >= 1) {
			counter = 0;
			//over again
			parseSignal(b);
			return;
		}
		switch (b) {
		case 0xF8:
			ON_RECEIVE_CLOCK();
			break;
		case 0xFA:
			ON_RECEIVE_START();
			break;
		case 0xFB:
			ON_RECEIVE_CONTINUE();
			break;
		case 0xFC:
			ON_RECEIVE_STOP();
			break;
		}
	} else {
		//Data Byte
		if (!counter) {
			if ((firstByte >> 4) == 0xF) {
				//ignore system message
				return;
			} else {
				//running status
				counter++;
			}
		}
	}


	switch (counter) {
	case 0: {
		counter++;
		firstByte = b;
		if (status & 0b1000) {
			rawBytes[0] = b;
			MIDI_RAW_MESSAGE_CALLBACK(rawBytes, 1);
		}
		break;
	}
	return;
	case 1: {
		switch (firstByte >> 4) {
		case 0xC:
			ON_RECEIVE_PROGRAM_CHANGE(firstByte & 0xF, b);
			counter = 0;
			break;
		case 0xD:
			counter = 0;
			break;
		default:
			counter++;
			secondByte = b;
			break;
		}
	}
		if (counter == 0) {
			rawBytes[0] = firstByte;
			rawBytes[1] = b;
			MIDI_RAW_MESSAGE_CALLBACK(rawBytes, 2);
		}
	return;
	case 2:{
		const uint8_t channel = firstByte & 0xF;
		counter = 0;
		switch (firstByte >> 4) {
		case 0xF:
			break;
		case 0x8:
			//NOTE OFF
			ON_RECEIVE_NOTE_OFF(channel, secondByte, b);
			break;
		case 0x9:
			//NOTE ON
			if (b) {
				ON_RECEIVE_NOTE_ON(channel, secondByte, b);
			} else {
				// 0 Velocity
				ON_RECEIVE_NOTE_OFF(channel, secondByte, 0);
			}
			break;
		case 0xA:
			//PolyphonicKeyPressure
			break;
		case 0xB:
			//ControlChange
			ON_RECEIVE_CONTROL_CHANGE(channel, secondByte, b);
			break;
		case 0xC:
			//ProgramChange
			break;
		case 0xD:
			//ChannelAfterTouch
			break;
		case 0xE:
			//PitchWheelChange
			break;
		}
		rawBytes[0] = firstByte;
		rawBytes[1] = secondByte;
		rawBytes[2] = b;
		MIDI_RAW_MESSAGE_CALLBACK(rawBytes, 3);
		return;
	}
	}
}

void SEND_TIMING_CLOCK(UART_HandleTypeDef* huart) {
	const static uint8_t CLOCK = 0xF8;
	HAL_UART_Transmit(huart, (uint8_t*)&CLOCK, 1, 1000);
}

__attribute__((weak)) void ON_RECEIVE_NOTE_ON(uint8_t ch, uint8_t note,
		uint8_t velocity) {
}
__attribute__((weak)) void ON_RECEIVE_NOTE_OFF(uint8_t ch, uint8_t note,
		uint8_t velocity) {
}
__attribute__((weak)) void ON_RECEIVE_CONTROL_CHANGE(uint8_t ch, uint8_t no,
		uint8_t value) {
}
__attribute__((weak)) void ON_RECEIVE_PROGRAM_CHANGE(uint8_t ch,
		uint8_t program) {
}
__attribute__((weak)) void ON_RECEIVE_CLOCK() {
}
__attribute__((weak)) void ON_RECEIVE_START() {
}
__attribute__((weak)) void ON_RECEIVE_CONTINUE() {
}
__attribute__((weak)) void ON_RECEIVE_STOP() {
}

__attribute__((weak)) void MIDI_RAW_MESSAGE_CALLBACK(uint8_t *bytes,
		uint16_t size) {
}
