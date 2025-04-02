//
//  FFT.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 1.04.25.
//  Copyright © 2025 Miraslau Parafeniuk. All rights reserved.
//

namespace FFT
{

// TODO: calculate arrays with dynamic Q
const int Q = 998244353;
const vint P = {1, 998244352, 911660635, 372528824, 929031873, 452798380, 922799308, 781712469, 476477967, 166035806, 258648936, 584193783, 63912897, 350007156, 666702199, 968855178, 629671588, 24514907, 996173970, 363395222, 565042129, 733596141, 267099868, 15311432}; // P[i] is root with period 2^i
const vint P_inv = {1, 998244352, 86583718, 509520358, 337190230, 87557064, 609441965, 135236158, 304459705, 685443576, 381598368, 335559352, 129292727, 358024708, 814576206, 708402881, 283043518, 3707709, 121392023, 704923114, 950391366, 428961804, 382752275, 469870224};
const vint pw = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608};
const vint pw_inv = {1, 499122177, 748683265, 873463809, 935854081, 967049217, 982646785, 990445569, 994344961, 996294657, 997269505, 997756929, 998000641, 998122497, 998183425, 998213889, 998229121, 998236737, 998240545, 998242449, 998243401, 998243877, 998244115, 998244234};

void FFT (vint &a, bool invert, int pw_size) { // actual size is 2^pw_size
    int n = pw[pw_size]; // a.size()
 
    for (int i=1, j=0; i<n; i++) {
        int s = pw_size - 1;
        for (; j>=pw[s]; s--) j -= pw[s];
        j += pw[s];
        if (i < j) swap(a[i],a[j]);
    }
    
    vint W(pw_size);
    W[0] = invert ? P_inv[pw_size] : P[pw_size];
    for (int s=1; s<pw_size; s++) W[s] = (ll)W[s-1] * W[s-1] % Q;
    
    for (int s=1; s<=pw_size; s++) {
        for (int i=0, m=pw[s-1]; i<n; i+=pw[s]) {
            for (int j=0, w=1; j<m; j++) {
                int u = a[i+j];
                int v = (ll)a[i+j+m]*w % Q;
                a[i+j] = u+v < Q ? u+v : u+v-Q;
                a[i+j+m] = u-v >= 0 ? u-v : u-v+Q;
                w = (ll)w * W[pw_size-s] % Q;
            }
        }
    }
    if (invert) {
        for (int i=0; i<n; i++) {
            a[i] = (ll)a[i] * pw_inv[pw_size] % Q;
        }
    }
}

void multiply (vint &fa, vint &fb, int pw_size)
{
    int n = pw[pw_size];
    fa.resize(n);
    fb.resize(n);
    
    FFT(fa, false, pw_size);
    FFT(fb, false, pw_size);
    for (int i=0; i<n; i++) fa[i] = (ll)fa[i]*fb[i] % Q;
    FFT(fa, true, pw_size);
}

}
