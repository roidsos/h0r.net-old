#include <drivers/memory/memory.h>

void memset(void* start, uint_64 value, uint_64 num) {
	//set it byte-by-byte if the length is under 8
	if (num <= 8) {
		uint_8* valPtr = (uint_8*)&value;
		for (uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + num); ptr++) {
			*ptr = *valPtr;
			valPtr++;
		}

		return;
	}
	//calculate the proceeding bytes
	uint_64 proceedingBytes = num % 8;
	uint_64 newnum = num - proceedingBytes;

	//set the memory 8 bytes at the time
	for (uint_64* ptr = (uint_64*)start; ptr < (uint_64*)((uint_64)start + newnum); ptr++) {
		*ptr = value;
	}
	//set the proceeding bytes byte-by-byte
	uint_8* valPtr = (uint_8*)&value;
	for (uint_8* ptr = (uint_8*)((uint_64)start + newnum); ptr < (uint_8*)((uint_64)start + num); ptr++)
	{
		*ptr = *valPtr;
		valPtr++;
	}
}
void memcpy(void* dest,void* src,uint_64 num){
	//copy it byte-by-byte if the length is under 8
   	if(num <= 8){
        uint_8* srcptr = (uint_8*)src;
        for(uint_8* destptr = (uint_8*)dest;destptr < (uint_8*)((uint_64)dest + num);destptr++){
            *destptr = *srcptr;
            srcptr++;
        }  
       return;
    } 
	//calculate the proceeding bytes and the source address as a uint_64*
    uint_64 proceedingBytes = num % 8;
    uint_64 newnum          = num - proceedingBytes;
    uint_64* srcptr          =(uint_64*)src;
    
	//copy the memory 8 bytes at the time
    for(uint_64* destptr =(uint_64*)dest; destptr < (uint_64*)((uint_64)dest + newnum);destptr++){
        *destptr = *srcptr;
        srcptr++;
    }
	//copy the proceeding bytes byte-by-byte
    uint_8* srcptr8 = (uint_8*)src;
    for(uint_8* destptr = (uint_8*)((uint_64*)dest + newnum); destptr < (uint_8*)((uint_64)dest + num);destptr++){
        *destptr = *srcptr8;
        destptr++;
    }

} 
int memcmp(void* aptr, void* bptr, unsigned int n){
	//record the pointers
	char* a = aptr,*b = bptr;
	//iterating trough all bytes to evaluate if its bigger or smaller
	for (unsigned int i = 0; i < n; i++)
	{
		if(a[i] < b[i]) return -1;
		else if(a[i] > b[i]) return 1;
	}
	return 0;
	
}