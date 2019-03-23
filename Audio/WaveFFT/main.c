#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fftw3.h>

#include "wave.h"

#define SAMPLE_COUNT 1024

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: WaveFFT <WAVE file name>\n");
    }
    else
    {
        printf("Reading file: %s...\n", argv[1]);

        int sampleRate = GetWaveSampleRate(argv[1]);

        if (sampleRate == -1)
        {
            printf("Error reading file %s\n.", argv[1]);

            exit(1);
        }

        printf("Sample rate: %d Hz\n", sampleRate);

        double * buffer = ReadWaveSamples(argv[1], SAMPLE_COUNT);

        printf("Performing FFT...\n");

        fftw_complex * spectrum = PerformFFT(buffer, SAMPLE_COUNT);

        printf("\n");
        printf("FFT values:\n");
        printf("(Index,Abs,Arg)\n");
        printf("\n");

        DisplayFFTValues(spectrum, sampleRate, SAMPLE_COUNT);
        WriteFFTValuesToFile("a.csv", spectrum, sampleRate, SAMPLE_COUNT);

        fftw_free(spectrum);
        free(buffer);

        printf("\n");
        printf("Done.\n");
        printf("\n");
    }

    return 0;
}
