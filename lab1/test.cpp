#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <cstdio>

static void Test() {
    u_int16_t n1 = 5;
    u_int16_t n2 = 2;
    __m64 vec1, vec2,res;
    vec1 = _mm_set_pi16(n1, n1, n1, n1);
    vec2 = _mm_set_pi16(n2, n2, n2, n2);
    
    res = _mm_add_pi16(vec1, vec2);

    // u_int16_t res0 = _mm_extract_pi16(res, 0);
    // u_int16_t res1 = _mm_extract_pi16(res, 1);
    // u_int16_t res2 = _mm_extract_pi16(res, 2);
    // u_int16_t res3 = _mm_extract_pi16(res, 3);

    printf("%u %u %u %u \n", _mm_extract_pi16(vec1, 0),
        _mm_extract_pi16(vec1, 1), _mm_extract_pi16(vec1, 2), _mm_extract_pi16(vec1, 3));

    printf("%u %u %u %u \n", _mm_extract_pi16(vec2, 0),
        _mm_extract_pi16(vec2, 1), _mm_extract_pi16(vec2, 2), _mm_extract_pi16(vec2, 3));

    printf("%u %u %u %u \n", _mm_extract_pi16(res, 0),
        _mm_extract_pi16(res, 1), _mm_extract_pi16(res, 2), _mm_extract_pi16(res, 3));

    _mm_empty();    
}

int main() {
    Test();
    return 0;
}