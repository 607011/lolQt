// DFT (Discrete Fourier Transform)
// http://paulbourke.net/miscellaneous/dft/

#ifndef __FFT_H_
#define __FFT_H_

#include <QtGlobal>

class FFT
{
public:
    FFT(int binSize);

    void DFT(qreal *x1, qreal *y1);

private:
    int m;
};

#endif // __FFT_H_
