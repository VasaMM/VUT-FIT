/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xmarti76
 */

/**
 * @brief Returns output of the neuron as product of inputs, sums and bias
 * @param inputSize   - number of inputs the neuron
 * @param input       - pointer to neuron input array (identical for all neurons in the layer)
 * @param weight      - pointer to weights for all neurons in the layer
 * @param bias        - bias value of the neuron
 * @return Output of the neuron
 */

// #pragma omp declare simd
// #pragma omp declare simd notinbranch uniform(inputSize, bias) simdlen(8) aligned(input) linear(input,weight)
float evalNeuron(
  size_t inputSize,
  const float* input,
  const float* weight,
  float bias
);
