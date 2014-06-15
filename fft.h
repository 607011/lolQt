// DFT (Discrete Fourier Transform)
// http://paulbourke.net/miscellaneous/dft/

#ifndef __FFT_H_
#define __FFT_H_

#include <QtGlobal>

class FFT
{
public:
    FFT(void);

    static const int Forward = 1;
    static const int Inverse = -1;

    static bool DFT(int dir, long m, qreal *x1, qreal *y1);
};

#endif // __FFT_H_
