// DFT (Discrete Fourier Transform)
// http://paulbourke.net/miscellaneous/dft/

#ifndef __FFT_H_
#define __FFT_H_

#include <QtGlobal>

class FFT
{
public:
    FFT(int binSize);

    static const int Forward = 1;
    static const int Inverse = -1;

    void DFT(qreal *x1, qreal *y1, int dir = Forward);

private:
    int m;
};

#endif // __FFT_H_
