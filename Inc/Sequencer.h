/*
 * Sequencer.h
 *
 *  Created on: 2018/11/27
 *      Author: NishiAsakusa Audio Developments / oxxxide / Akikazu Iwasa
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "stm32f4xx.h"
#include "String.h"

typedef enum {
	SEQ_IDLING = 0, SEQ_RUNNING = 1
} SequencerStatus;

typedef struct {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
} Notes;

typedef struct {
	uint8_t chance;
	uint8_t mute;
	uint8_t decay;
	uint8_t grain;
	uint8_t hold_time;
} PlayFx;

typedef struct {
	uint8_t gate;
	uint8_t total_clk;
	uint8_t rpt_clk;
	uint8_t total_cnt;
	uint8_t rpt_cnt;
	float decay_cof;
} PlayFxStatus;

typedef struct {
	int16_t bpm;
	uint8_t step[4];
	uint8_t clock_cnt;
	uint8_t step_length_array[4];
	Notes sequenceData[16];
	SequencerStatus status;
	int8_t cursor_index;
	uint8_t playFxEnabled;
	PlayFx playfx[4];
	PlayFxStatus pfx_status[4];
} Sequencer;

void StartSequencer(Sequencer* seq);

void StopSequencer(Sequencer* seq);

void ClockSequencer(Sequencer* seq);


void OnBeatRdmzer(Sequencer* seq, int index);
void tickPlayFx(Sequencer* seq, int index);

void InitSequencer(Sequencer* seq);

void ChangeBPM(Sequencer* seq, int add);

void SetBPM(Sequencer* seq, int16_t bpm);

void ON_PROGRESS_SEQUENCER_CLOCK();

void ON_START_SEQUENCER();

void ON_STOP_SEQUENCER();

void SEQUENCER_BEAT_CALLBACK(uint8_t * step);

#endif /* SEQUENCER_H_ */
