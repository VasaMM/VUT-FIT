/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xmarti76
 */

#include <cstdlib>
#include "neuron.h"

float evalNeuron(size_t inputSize, const float* input, const float* weight, float bias) {
	// TODO: Step0 - Fill in the implementation, all the required arguments are passed.
	//               If you don't use them all you are doing something wrong!

	float output = bias;

	#pragma omp simd simdlen(16) collapse(2) linear(input, weight) reduction(+:output)
	for (size_t i = 0; i < inputSize; ++i) {
		output += weight[i] * input[i];
	}

	return output > 0.0f ? output : 0.0f;
}
