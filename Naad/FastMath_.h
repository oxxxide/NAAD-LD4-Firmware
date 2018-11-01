/*
 * FastMath.h
 *
 *  Created on: 2017/12/19
 *      Author: devox
 */

#ifndef FASTMATH_H_
#define FASTMATH_H_

#include <stdlib.h>
#include <sys/_stdint.h>
#include <math.h>

#define POW_TABLE_SIZE 1025
#define FASTMATH_TABLE_SIZE 1024
#define FASTMATH_TABLE_MASK 1023
#define OCTAVE_RANGE 4
#define FORCE_INLINE __attribute__((always_inline)) inline

typedef float real;

extern const real Table_Sawtooth[FASTMATH_TABLE_SIZE];
extern const real Table_Sin[FASTMATH_TABLE_SIZE];
extern const real Table_Exponential[FASTMATH_TABLE_SIZE];
extern const real Table_Exponential_ex2[2048];
extern const real Table_square[FASTMATH_TABLE_SIZE];

//real* __sin;
//real* __cos;
//real* __pow2;

void FastMath_Init();

void MATH_Destroy_WaveTables();

//real FastMath_sinf(real rad);

//real FastMath_cosf(real rad);

real FastMath_toPitchCoefficient(real octave);

real saturateValue(int index);

#endif /* FASTMATH_H_ */
