#include "bit_ops.h"

// Возвращает бит номер n числа x.
// Предполагается 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    // YOUR CODE HERE
    return ((x >> n) & 1);
    // Взврат -1 бессмысленный и сделан для правильной компиляции
    return -1;
}
// Выставляет значение v в бите номер n
// Предполагается что 0 <= n <= 31 и v равно 0 или 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    /*
    v <<= n;
    // if v == 1
    *x = (*x | v) ;
    //if v == 0
    
    */
    // YOUR CODE HERE
}
// Меняет значение бита номер n на противоположное.
// Предполагается что 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    // YOUR CODE HERE
    *x ^= (1 << n);    
}

