#include <util/math.h>

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