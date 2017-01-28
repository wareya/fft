#include "fft.hpp"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double lerp(double a, double b, double i)
{
    return a*(1-i) + b*i;
}

enum {
    TEST_IMPULSE
   ,TEST_DC
   ,TEST_NYQUIST
   ,TEST_HALF
   ,TEST_SQUARE
   ,TEST_SQUAREDOUBLE
   ,TEST_SAW
   ,TEST_NOISE
   ,TEST_COMPLEXIMPULSE
   ,TEST_COMPLEXDC
   ,TEST_COMPLEXNOISE
   ,TEST_BLANK
};

#define TEST TEST_HALF

void init(double* in_real, double* in_imag, uint64_t size)
{
    for(uint64_t i = 0; i < size; i++)
    {
        switch(TEST)
        {
        case TEST_IMPULSE:
            in_real[i] = 0;
            in_imag[i] = 0;
            in_real[0] = 1;
            break;
        case TEST_DC:
            in_real[i] = 1;
            in_imag[i] = 0;
            break;
        case TEST_NYQUIST:
            in_real[i] = cos(M_PI*i);
            in_imag[i] = 0;
            break;
        case TEST_HALF:
            in_real[i] = cos(M_PI*i/2);
            in_imag[i] = 0;
            break;
        case TEST_SQUARE:
            in_real[i] = (i<size/2)?1:-1;
            in_imag[i] = 0;
            break;
        case TEST_SQUAREDOUBLE:
            in_real[i] = (i%(size/2)<size/4)?1:-1;
            in_imag[i] = 0;
            break;
        case TEST_SAW:
            in_real[i] = lerp(-1, 1, i/double(size-1)); // subtract 1 from size to avoid DC from incorrect starting phase
            in_imag[i] = 0;
            break;
        case TEST_NOISE:
            in_real[i] = rand()%1024/512.0-1;
            in_imag[i] = 0;
            break;
        case TEST_COMPLEXIMPULSE:
            in_real[i] = 0;
            in_imag[i] = 0;
            in_real[0] = 1;
            in_imag[0] = 1;
            break;
        case TEST_COMPLEXDC:
            in_real[i] = 1;
            in_imag[i] = 1;
            break;
        case TEST_COMPLEXNOISE:
            in_real[i] = rand()%1024/512.0-1;
            in_imag[i] = rand()%1024/512.0-1;
            break;
        case TEST_BLANK:
        default:
            in_real[i] = 0;
            in_imag[i] = 0;
        }
    }
}

int main()
{
    const int SIZE = 16;
    srand(time(NULL));
    double* in_real  = (double*)malloc(sizeof(double)*SIZE);
    double* in_imag  = (double*)malloc(sizeof(double)*SIZE);
    double* out_real = (double*)malloc(sizeof(double)*SIZE);
    double* out_imag = (double*)malloc(sizeof(double)*SIZE);
    init(in_real, in_imag, SIZE);
    
    puts("data");
    puts("sample\treal\timag\tmag");
    for(int i = 0; i < SIZE; i++)
        printf("%d\t%.2f\t%.2f\t%.2f \n", i, in_real[i], in_imag[i], sqrt(in_real[i]*in_real[i]+in_imag[i]*in_imag[i]));
    puts("");
    
    fft(in_real, in_imag, SIZE, out_real, out_imag);
    
    puts("transform");
    puts("bin\treal\timag\tmag");
    for(int i = 0; i < SIZE; i++)
        printf("%d\t%.2f\t%.2f\t%.2f \n", i, out_real[i], out_imag[i], sqrt(out_real[i]*out_real[i]+out_imag[i]*out_imag[i]));
    puts("");
    
    ifft(out_real, out_imag, SIZE, in_real, in_imag);
    
    puts("inverse");
    puts("sample\treal\timag\tmag");
    for(int i = 0; i < SIZE; i++)
        printf("%d\t%.2f\t%.2f\t%.2f \n", i, in_real[i], in_imag[i], sqrt(in_real[i]*in_real[i]+in_imag[i]*in_imag[i]));
    puts("");
}
