/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xmarti76
 */

#include <cstdlib>
#include "neuron.h"

float evalNeuron(size_t inputSize, const float* input, const float* weight, float bias) {
	float output = bias;

	#pragma omp simd simdlen(32) linear(input, weight) reduction(+:output)
	for (size_t i = 0; i < inputSize; ++i) {
		output += weight[i] * input[i];
	}

	return output > 0.0f ? output : 0.0f;
}
