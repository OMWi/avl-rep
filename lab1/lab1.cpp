#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <cstdio>

// F[i] = (A[i] + B[i])*(C[i] - D[i])

int main() {
    int8_t a[8]{1, 47, -103, 90, -77, -117, -48, 18};
    int8_t b[8]{2, -1, -16, 68, -17, 127, -82, -44};
    int8_t c[8]{3, -96, 54, 44 -100, -6, 111, 3};
    int16_t d[8]{2, -25088, 30000, -8192, 256, 23264, 29440, 29999};
    int32_t f[8]{0};
    int32_t f2[8]{0};
    __m64 vec1, vec2, leftRes, rightRes, loMul, hiMul, res;
    __m64 zero = _mm_setzero_si64();
    for (int i = 0; i < 8; i++) {
        int answer = (a[i]+b[i])*(c[i]-d[i]);
        f2[i] = answer;
        printf("%d) %d\n", i+1, answer);
    }
    printf("\n");

    for (int i = 0; i < 2; i++) {
        vec1 = _mm_set_pi16(a[4*i+3], a[4*i+2], a[4*i+1], a[4*i]);
        vec2 = _mm_set_pi16(b[4*i+3], b[4*i+2], b[4*i+1], b[4*i]);
        leftRes = _mm_add_pi16(vec1, vec2); // left brackets
        vec1 = _mm_set_pi16(c[4*i+3], c[4*i+2], c[4*i+1], c[4*i]);
        vec2 = _mm_set_pi16(d[4*i+3], d[4*i+2], d[4*i+1], d[4*i]);
        rightRes = _mm_sub_pi16(vec1, vec2); // right brackets
        loMul = _mm_mullo_pi16(leftRes, rightRes);
        hiMul = _mm_mulhi_pi16(leftRes, rightRes);
        res = _mm_unpacklo_pi16(loMul, hiMul);
        f[4*i] = _m_to_int(res);
        f[4*i+1] = _m_to_int(_mm_unpackhi_pi32(res, zero));
        res = _mm_unpackhi_pi16(loMul, hiMul);
        f[4*i+2] = _m_to_int(res);
        f[4*i+3] = _m_to_int(_mm_unpackhi_pi32(res, zero));
    }
        
    for (int i = 0; i < 8; i++) {
        printf("F[%d] = %d\n", i+1, f[i]);
    }    
    return 0;
}