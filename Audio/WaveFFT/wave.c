#include "wave.h"

int GetWaveSampleRate(const char * path)
{
    SF_INFO waveInfo;

    memset(&waveInfo, 0, sizeof(SF_INFO));

    SNDFILE * file = sf_open(path, SFM_READ, &waveInfo);

    if (!file)
    {
        return -1;
    }

    int sampleRate = waveInfo.samplerate;

    sf_close(file);

    return sampleRate;
}

double * ReadWaveSamples(const char * path, int sampleCount)
{
    SF_INFO waveInfo;

    memset(&waveInfo, 0, sizeof(SF_INFO));

    SNDFILE * file = sf_open(path, SFM_READ, &waveInfo);
    double * buffer = (double *)malloc(sizeof(double) * sampleCount);

    if (!file || !buffer)
    {
        if (!file)
        {
            printf("ERROR: %s\n", sf_strerror(file));
        }
        else if (!buffer)
        {
            perror("ERROR: ");
        }

        return NULL;
    }
    else if (waveInfo.channels > 1)
    {
        printf("WARNING: input file is not in mono format.\n");
    }

    sf_read_double(file, buffer, sampleCount);

    sf_close(file);

    return buffer;
}

fftw_complex * PerformFFT(double * buffer, int sampleCount)
{
    fftw_complex * spectrum = (fftw_complex *)malloc(sizeof(fftw_complex) * (sampleCount / 2 + 1));
    fftw_plan plan = fftw_plan_dft_r2c_1d(sampleCount, buffer, spectrum, FFTW_ESTIMATE);

    fftw_execute(plan);

    fftw_destroy_plan(plan);

    return spectrum;
}

void DisplayFFTValues(const fftw_complex * spectrum, int sampleRate, int sampleCount)
{
    for (int i = 0; i < (sampleCount / 2 + 1); i++)
    {
        printf("%.2f,%f\n", i * ((double)sampleRate / (double)sampleCount), sqrt(pow(creal(spectrum[i]), 2) + pow(cimag(spectrum[i]), 2)));
    }
}

void WriteFFTValuesToFile(const char * path, const fftw_complex * spectrum, int sampleRate, int sampleCount)
{
    FILE * fp = fopen(path, "w");

    if (fp)
    {
        fprintf(fp, "Hz,Mag\n");

        for (int i = 0; i < (sampleCount / 2 + 1); i++)
        {
            fprintf(fp, "%.2f,%f\n", i * ((double)sampleRate / (double)sampleCount), sqrt(pow(creal(spectrum[i]), 2) + pow(cimag(spectrum[i]), 2)));
        }

        fclose(fp);
    }
}
