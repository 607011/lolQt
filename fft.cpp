// DFT (Discrete Fourier Transform)
// http://paulbourke.net/miscellaneous/dft/

#include <qmath.h>
#include "fft.h"

FFT::FFT(int binSize)
    : m(binSize)
{
    // ...
}


void FFT::DFT(qreal *x1, qreal *y1)
{
    qreal *x2 = new qreal[m];
    qreal *y2 = new qreal[m];

    for (int i = 0 ; i < m ; ++i) {
        x2[i] = 0;
        y2[i] = 0;
        qreal arg = -2 * M_PI * qreal(i) / qreal(m);
        for (int k = 0; k < m; ++k) {
            qreal c = qCos(k * arg);
            qreal s = qSin(k * arg);
            x2[i] += (x1[k] * c - y1[k] * s);
            y2[i] += (x1[k] * s + y1[k] * c);
        }
    }

    for (int i = 0 ; i < m ; ++i) {
        x1[i] = x2[i] / qreal(m);
        y1[i] = y2[i] / qreal(m);
    }

    delete [] x2;
    delete [] y2;
}

