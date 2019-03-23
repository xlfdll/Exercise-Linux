#ifndef WAVE_H
#define WAVE_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>

#include <sndfile.h>
#include <fftw3.h>

int GetWaveSampleRate(const char * path);
double * ReadWaveSamples(const char * path, int sampleCount);
fftw_complex * PerformFFT(double * buffer, int sampleCount);
void DisplayFFTValues(const fftw_complex * spectrum, int sampleRate, int sampleCount);
void WriteFFTValuesToFile(const char * path, const fftw_complex * spectrum, int sampleRate, int sampleCount);

#endif
