#ifndef UTILZ_H
#define UTILZ_H

#include <Arduino.h>

struct Band {
    unsigned char bass;
    unsigned char mid;
    unsigned char treble;
} ;

// https://stackoverflow.com/questions/31836598/subsampling-an-array-of-numbers

// std::array<int, OUT> sampling(const std::array<int, IN>& a)
// {
//      std::array<int, OUT> res;
 
//      for (int i = 0; i != OUT - 1; ++i) {
//          int index = i * (IN - 1) / (OUT - 1);
//          int p = i * (IN - 1) % (OUT - 1);
 
//          res[i] = ((p * a[index + 1]) + (((OUT - 1) - p) * a[index])) / (OUT - 1);
//     }
//     res[OUT - 1] = a[IN - 1]; // done outside of loop to avoid out of bound access (0 * a[IN])
//     return res;
// }

void sampling(double* inBuffer, uint16_t IN, double* outBuffer, uint16_t OUT, double* iBuffer = NULL) {
    // std::array<int, OUT> res;
 
     for (int i = 0; i != OUT - 1; ++i) {
         int index = i * (IN - 1) / (OUT - 1);
         int p = i * (IN - 1) % (OUT - 1);
 
         outBuffer[i] = ((p * inBuffer[index + 1]) + (((OUT - 1) - p) * inBuffer[index])) / (OUT - 1);
         if (iBuffer != NULL) {
             iBuffer[i] = 0.0; // FFT Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
         }
    }
    outBuffer[OUT - 1] = inBuffer[IN - 1]; // done outside of loop to avoid out of bound access (0 * a[IN])
    if (iBuffer != NULL) {
        iBuffer[OUT - 1] = 0.0;
    }
    // return res;
}

void sampling(int32_t* inBuffer, uint16_t IN, double* outBuffer, uint16_t OUT, double* iBuffer = NULL) {
    // std::array<int, OUT> res;
 
     for (int i = 0; i != OUT - 1; ++i) {
         int index = i * (IN - 1) / (OUT - 1);
         int p = i * (IN - 1) % (OUT - 1);
 
         outBuffer[i] = ((p * inBuffer[index + 1]) + (((OUT - 1) - p) * inBuffer[index])) / (OUT - 1);
         if (iBuffer != NULL) {
             iBuffer[i] = 0.0; // FFT Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
         }
    }
    outBuffer[OUT - 1] = inBuffer[IN - 1]; // done outside of loop to avoid out of bound access (0 * a[IN])
    if (iBuffer != NULL) {
        iBuffer[OUT - 1] = 0.0;
    }
    // return res;
}

#endif