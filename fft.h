// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __FFT_H_
#define __FFT_H_

#include "kiss_fft.h"

template <typename T>
class FFT {
public:
    FFT(int binSize)
        : mBinSize(binSize)
        , mCfg(kiss_fft_alloc(binSize, 0, NULL, NULL))
        , mIn(new kiss_fft_cpx[binSize])
    {
        // ...
    }
    ~FFT() {
        free(mCfg);
        kiss_fft_cleanup();
    }

    void perform(const kiss_fft_cpx *fin, kiss_fft_cpx *fout) {
        kiss_fft(mCfg, fin, fout);
    }

    template <typename T>
    void perform(const T *fin, kiss_fft_cpx *fout) {
        for (int i = 0; i < mBinSize; ++i) {
            mIn[i].r = *fin++;
            mIn[i].i = 0;
        }
        kiss_fft(mCfg, mIn, fout);
    }

private:
    int mBinSize;
    kiss_fft_cfg mCfg;
    kiss_fft_cpx *mIn;
};

#endif // __FFT_H_
