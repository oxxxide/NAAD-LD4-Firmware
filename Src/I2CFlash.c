/*
 * I2CFlash.c
 *
 *  Created on: 2018/09/23
 *      Author: NishiAsakusa Audio Developments / oxxxide / Akikazu Iwasa
 */

#include "I2CFlash.h"
#include "Tone.h"

#define TIMEOUT_MSEC_EEPROM 2000

void I2CFlash_Init(I2C_EEPROM* instance, I2C_HandleTypeDef *hi2c) {
	instance->pI2C = hi2c;
	instance->devAddress = 80 << 1;
	instance->memAddSize = I2C_MEMADD_SIZE_16BIT;
}

HAL_StatusTypeDef I2CFlash_Write(I2C_EEPROM* instance, uint16_t memAddress,
		uint8_t *pData, uint16_t size) {
	HAL_StatusTypeDef ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
		return ret;
	}
	return HAL_I2C_Mem_Write(instance->pI2C, instance->devAddress, memAddress,
			instance->memAddSize, pData, size, TIMEOUT_MSEC_EEPROM);
}

HAL_StatusTypeDef I2CFlash_Read(I2C_EEPROM* instance, uint16_t memAddress,
		uint8_t *pData, uint16_t size) {
	HAL_StatusTypeDef ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
		return ret;
	}
	return HAL_I2C_Mem_Read(instance->pI2C, instance->devAddress, memAddress,
			instance->memAddSize, pData, size, TIMEOUT_MSEC_EEPROM);

}

HAL_StatusTypeDef I2CFlash_SaveCvMappingConfig(I2C_EEPROM* instance, CV_ASSIGN* data) {
	uint16_t size = sizeof(CV_ASSIGN) * 4;
	return I2CFlash_Write(instance, ROM_ADDRESS_CV_MAPPING,
				(uint8_t*) data, size);
}

HAL_StatusTypeDef I2CFlash_ReadCvMappingConfig(I2C_EEPROM* instance, CV_ASSIGN* data) {
	uint16_t size = sizeof(CV_ASSIGN) * 4;
	return I2CFlash_Read(instance,ROM_ADDRESS_CV_MAPPING, (uint8_t*)data, size);
}

HAL_StatusTypeDef I2CFlash_SaveMidiConfig(I2C_EEPROM* instance,
		MidiConfig* midiconfig) {

	uint16_t size = sizeof(MidiConfig);
	HAL_StatusTypeDef ret = I2CFlash_Write(instance, ROM_ADDRESS_MIDICONFIG,
			(uint8_t*) midiconfig, size);
	return ret;
}

HAL_StatusTypeDef I2CFlash_SaveSequenceData(I2C_EEPROM* instance,
		Sequencer* seq) {
	const uint16_t size = sizeof(Notes);
	Notes *p = &(seq->sequenceData[0]);
	HAL_StatusTypeDef ret;
	for(int i=0;i<16;i++){
		ret = I2CFlash_Write(instance, ROM_ADDRESS_SEQUENCE_DATA + (i*size), (uint8_t*)&p[i] ,size);
		if(ret != HAL_OK){
			return ret;
		}
		ret = waitUntilReady(instance);
	}
	if(ret != HAL_OK){
		return ret;
	}
	ret = I2CFlash_Write(instance, ROM_ADDRESS_SEQUENCE_BPM, (uint8_t*)&seq->bpm , 2);
	if(ret != HAL_OK){
		return ret;
	}
	ret = waitUntilReady(instance);
	if(ret != HAL_OK){
			return ret;
	}
	ret = I2CFlash_Write(instance, ROM_ADDRESS_SEQUENCE_STEPLENGTH,
			&seq->step_length_array[0], 4);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = I2CFlash_Write(instance, ROM_ADDRESS_SEQUENCE_PLAYFX_ENABLED,
			&seq->playFxEnabled, 1);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = I2CFlash_Write(instance, ROM_ADDRESS_SEQUENCE_BEATREPEAT,
			(uint8_t*)seq->playfx, sizeof(PlayFx) * 4);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = waitUntilReady(instance);
	return ret;
}


HAL_StatusTypeDef I2CFlash_LoadSequenceData(I2C_EEPROM* instance,
		Sequencer* seq) {
	uint16_t size = sizeof(Notes);
	Notes *p = &(seq->sequenceData[0]);
	HAL_StatusTypeDef ret;
	for (int i = 0; i < 16; i++) {
		ret = I2CFlash_Read(instance, ROM_ADDRESS_SEQUENCE_DATA + (i * size),
				(uint8_t*) &p[i], size);
		if (ret != HAL_OK) {
			return ret;
		}
		ret = waitUntilReady(instance);
	}
	if (ret != HAL_OK) {
		return ret;
	}
	ret = I2CFlash_Read(instance, ROM_ADDRESS_SEQUENCE_BPM,
			(uint8_t*) &seq->bpm, 2);
	if (ret != HAL_OK) {
		return ret;
	}
	SetBPM(seq, seq->bpm);
	ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = I2CFlash_Read(instance, ROM_ADDRESS_SEQUENCE_STEPLENGTH,
			&seq->step_length_array[0], 4);
	seq->step_length_array[0] = LIMIT(seq->step_length_array[0], 16, 1);
	seq->step_length_array[1] = LIMIT(seq->step_length_array[1], 16, 1);
	seq->step_length_array[2] = LIMIT(seq->step_length_array[2], 16, 1);
	seq->step_length_array[3] = LIMIT(seq->step_length_array[3], 16, 1);
	if (ret != HAL_OK) {
		return ret;
	}
	ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
			return ret;
	}
	ret = I2CFlash_Read(instance, ROM_ADDRESS_SEQUENCE_PLAYFX_ENABLED,
			(uint8_t*)&seq->playFxEnabled, 1);
	if (ret != HAL_OK) {
			return ret;
	}
	seq->playFxEnabled = seq->playFxEnabled ? 1 : 0;
	ret = waitUntilReady(instance);
	if (ret != HAL_OK) {
			return ret;
	}
	ret = I2CFlash_Read(instance, ROM_ADDRESS_SEQUENCE_BEATREPEAT,
			(uint8_t*)seq->playfx, sizeof(PlayFx) * 4);
	if (ret != HAL_OK) {
		return ret;
	}
	seq->playfx[0].chance = LIMIT(seq->playfx[0].chance, 7, 0);
	seq->playfx[1].chance = LIMIT(seq->playfx[1].chance, 7, 0);
	seq->playfx[2].chance = LIMIT(seq->playfx[2].chance, 7, 0);
	seq->playfx[3].chance = LIMIT(seq->playfx[3].chance, 7, 0);
	return ret;
}

HAL_StatusTypeDef I2CFlash_FactoryReset(I2C_EEPROM* instance) {
	MidiConfig mc;
	InitMidiConfig(&mc);
	HAL_StatusTypeDef ret = I2CFlash_Write(instance, ROM_ADDRESS_MIDICONFIG,
			(uint8_t*) &mc, sizeof(MidiConfig));
	if (ret != HAL_OK) {
		return ret;
	}
	ret = waitUntilReady(instance);

	Tone t;
	InitTone(&t);
	for (int i = 0; i < 128; i++) {
		ret = I2CFlash_Write(instance, ROM_ADDRESS_TONE_USER + i * 64,
				(uint8_t*) &t, sizeof(Tone));
		if (ret != HAL_OK) {
			return ret;
		}
		ret = waitUntilReady(instance);
		if (ret != HAL_OK) {
			return ret;
		}
	}

	//clear temporary
	for (int i = 0; i < 4; i++) {
		ret = I2CFlash_Write(instance, ROM_ADDRESS_TONE_TMP + i * 64,
				(uint8_t*) &t, sizeof(Tone));
		if (ret != HAL_OK) {
			return ret;
		}
		ret = waitUntilReady(instance);
		if (ret != HAL_OK) {
			return ret;
		}
	}

	Sequencer seq;
	InitSequencer(&seq);
	I2CFlash_SaveSequenceData(instance, &seq);

	CV_ASSIGN cvass[4];
	InitCvAssignements(cvass,4);
	I2CFlash_SaveCvMappingConfig(instance,cvass);
	return ret;
}

HAL_StatusTypeDef I2CFlash_SaveVersion(I2C_EEPROM* instance, uint16_t* version) {
	return I2CFlash_Write(instance, ROM_ADDRESS_SYS_V_INFO,
					(uint8_t*) version, sizeof(uint16_t));
}

HAL_StatusTypeDef I2CFlash_LoadVersion(I2C_EEPROM* instance, uint16_t* version) {
	return I2CFlash_Read(instance, ROM_ADDRESS_SYS_V_INFO,
					(uint8_t*) version, sizeof(uint16_t));
}

HAL_StatusTypeDef waitUntilReady(I2C_EEPROM* instance) {
	int cnt = 0;
	do {
		if (cnt > 500) {
			return HAL_TIMEOUT;
		}
		cnt++;
		HAL_Delay(7);
	} while (HAL_I2C_GetState(instance->pI2C) != HAL_I2C_STATE_READY);
	return HAL_OK;
}
