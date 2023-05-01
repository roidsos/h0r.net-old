#include "util/math.h"

uint_8 sqrt(uint_8 x){
    if(x == 0 || x == 1){
        return x;
    }

    uint_8 start = 1, end = (uint_8)256;

    while (start + 1 < end){
        uint_8 mid = start + (end - start) / 2;
        if(mid == x/mid){
            return mid;
        } else if(mid > x/mid){
            end = mid;
        } else{
            start = mid;
        }
    }      
    return start;
}

uint_8 abs(int_8 input){
    return (uint_8) input < 0 ? input * -1 : input;   
}

uint pow(uint_8 base, uint_8 exponent) {
    uint8_t result = 1;
    for (uint8_t i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

uint8_t factorial(uint8_t n) {
    uint8_t result = 1;
    for (uint8_t i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}