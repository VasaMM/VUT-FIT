/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xmarti76
 */

#include <cstdlib>
#include "neuron.h"

float evalNeuron(size_t inputSize, size_t neuronCount, const float* input, const float* weights, float bias, size_t neuronId) {
	// TODO: Step0 - Fill in the implementation, all the required arguments are passed.
	//               If you don't use them all you are doing something wrong!

	float output = bias;

	for (size_t i = 0; i < inputSize; ++i) {
		output += weights[neuronCount * i + neuronId] * input[i];
	}

	return output > 0.0f ? output : 0.0f;
}
