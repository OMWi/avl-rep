#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <cstdio>
#include <time.h>
#include <stdlib.h>

// F[i] = (A[i] + B[i])*(C[i] - D[i])

int main() {
    int8_t a[8], b[8], c[8];
    int16_t d[8];
    int32_t f[8], ans[8];
    int testNum = 50000;
    srand(time(nullptr));
    for (int test = 0; test < testNum; test++) {
        for (int i = 0; i < 8; i++) {
            a[i] = rand() % 100 - 50;
            b[i] = rand() % 100 - 50;
            c[i] = rand() % 100 - 50;
            d[i] = rand() % 60000 - 30000;
            ans[i] = (a[i] + b[i]) * (c[i] - d[i]);
        }

        __m64 leftRes, rightRes, loMul, hiMul, res;
        __m64 zero = _mm_setzero_si64(); __m64 one_pi8 = _mm_set1_pi8(1);
        __m64 vec1 = _mm_set_pi8(a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0]);
        __m64 vec2 = _mm_set_pi8(b[7], b[6], b[5], b[4], b[3], b[2], b[1], b[0]);
        leftRes = _mm_add_pi8(vec1, vec2);
            
        __m64 mask = _mm_cmpgt_pi8(zero, leftRes);
        __m64 tempLeft = _mm_unpacklo_pi8(leftRes, mask);

        vec1 = _mm_set_pi16(c[3], c[2], c[1], c[0]);
        vec2 = _mm_set_pi16(d[3], d[2], d[1], d[0]);
        rightRes = _mm_sub_pi16(vec1, vec2);
        loMul = _mm_mullo_pi16(tempLeft, rightRes);
        hiMul = _mm_mulhi_pi16(tempLeft, rightRes);
        res = _mm_unpacklo_pi16(loMul, hiMul);
        f[0] = _m_to_int(res);
        f[1] = _m_to_int(_mm_unpackhi_pi32(res, zero));
        res = _mm_unpackhi_pi16(loMul, hiMul);
        f[2] = _m_to_int(res);
        f[3] = _m_to_int(_mm_unpackhi_pi32(res, zero));

        tempLeft = _mm_unpackhi_pi8(leftRes, mask);
        vec1 = _mm_set_pi16(c[7], c[6], c[5], c[4]);
        vec2 = _mm_set_pi16(d[7], d[6], d[5], d[4]);
        rightRes = _mm_sub_pi16(vec1, vec2);
        loMul = _mm_mullo_pi16(tempLeft, rightRes);
        hiMul = _mm_mulhi_pi16(tempLeft, rightRes);
        res = _mm_unpacklo_pi16(loMul, hiMul);
        f[4] = _m_to_int(res);
        f[5] = _m_to_int(_mm_unpackhi_pi32(res, zero));
        res = _mm_unpackhi_pi16(loMul, hiMul);
        f[6] = _m_to_int(res);
        f[7] = _m_to_int(_mm_unpackhi_pi32(res, zero));

        bool correct = true;
        for (int i = 0; i < 8; i++) {
            if (ans[i] != f[i]) {
                correct = false;
                break;
            }
        }

        if (correct) {
            printf("test %d correct\n", test+1);
        }
        else {
            printf("test %d error\n", test+1);
            printf("correct array:\n");
            for (int i = 0; i < 8; i++) {
                printf("%d ", ans[i]);
            }
            printf("\n");
            printf("my array\n");
            for (int i = 0; i < 8; i++) {
                printf("%d ", f[i]);
            }
            printf("\n");
            return -1;
        }
    }
}