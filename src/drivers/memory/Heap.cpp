#include "drivers/memory/Heap.h"
#include "drivers/memory/memory.h"

MemorySegment* first_free;

void InitHeap(uint_64 heapstart ,uint_64 heaplength){
    first_free = (MemorySegment*)heapstart;
    first_free->length          = heaplength - sizeof(MemorySegment);
    first_free->next            = 0;
    first_free->previous        = 0;
    first_free->nextfree        = 0;
    first_free->previousfree    = 0;
    first_free->isfree          = true;
}
void* malloc(uint_64 size){
    uint_64 remainder = size % 8;
    size -= remainder;
    if (remainder != 0) size +=8;
    MemorySegment* current = first_free;
    while(true){
        if (current->length >= size){
            if (current->length > size + sizeof(MemorySegment)){
                MemorySegment* newSegment = (MemorySegment*)((uint_64)current + sizeof(MemorySegment) + size);//making a new segment
                newSegment->isfree            = true;//giving it values
                newSegment->length          = (((uint_64)current->length) - sizeof(MemorySegment) +size);
                newSegment->next            = current->next;
                newSegment->nextfree        = current->nextfree;
                newSegment->previous        = current;
                newSegment->previousfree    = current->previousfree;

                current->nextfree = newSegment;// setting current's next segment
                current->next     = newSegment;

                current->length  = size;// giving it the new size
            }
            if(current == first_free){
                first_free = current->nextfree;
            }
            current->isfree = false;//allocating it
            

            if(current->previousfree != 0) current->previousfree->nextfree = current->nextfree;// skipping the current backwards for the frees
            if(current->nextfree != 0) current->nextfree->previousfree = current->previousfree;// skipping the current forwards for the frees
            if(current->previous != 0) current->previous->nextfree = current->nextfree;// skipping the current backwards
            if(current->next != 0) current->next->previousfree = current->previousfree;// skipping the current forwards

            return current + 1;//sizeof(MemorySegment);
        }
         if(current->nextfree == 0)
        {
        return 0; //out of memory
        }
        current = current->nextfree;
    }
 return 0;//this line is just for decoration lol
}
void CombineSegments(MemorySegment* a,MemorySegment* b){
    if(a == 0 || b == 0){
        return;
    }
    if(a < b){
        // set up a's properties
        a->length += b->length + sizeof(MemorySegment);
        a->next = b->next;
        a->nextfree = b->nextfree;
        //make the previous of the next concistant
        b->next->previous = a;
        // if its free modify the nextfrees
        if(a->isfree){
        b->next->previousfree = a;
        b->nextfree->previousfree = a;
        }
    }
    if(a > b){
        // set up b's properties
        b->length += a->length + sizeof(MemorySegment);
        b->next = a->next;
        b->nextfree = a->nextfree;
        //make the previous of the next concistant
        a->next->previous = b;
        // if its free modify the nextfrees
        if(a->isfree){
        a->next->previousfree = b;
        a->nextfree->previousfree = b;
        }
    }
}
void free(void* tofree){
    MemorySegment* current;
    register AlignedMemorySegment* ams = ((AlignedMemorySegment*)tofree) - 1;
    if(ams->IsAligned){
    current = (MemorySegment*)(uint_64)ams->MemorySegmentAddr;//establishing the target but its aligned
    }
    else{
    current = ((MemorySegment*)tofree) - 1;//establishing the target
   
    }
    current->isfree = true;//freeing the segment

    if(current < first_free) first_free = current;//checking if this is the first one

    //correcting potencial issues for the frees
    if (current->nextfree != 0){
        if(current->nextfree->previousfree < current){
            current->nextfree->previousfree = current;
        }
    }
     if (current->previousfree != 0){
        if(current->previousfree->nextfree < current){
            current->previousfree->nextfree = current;
        }
    }
    //correcting potencial issues
     if (current->next != 0){
        if(current->next->previous < current){
            current->next->previous = current;
            if(current->next->isfree) CombineSegments(current,current->next);//removing fragmentation
        }
    }
     if (current->previous != 0){
        if(current->previous->next < current){
            current->previous->next = current;
            if(current->previous->isfree) CombineSegments(current,current->previous);//removing fragmentation
        }
    }

}
void* calloc(uint_64 size){
 void* mallocVal = malloc(size);
 memset(mallocVal,0,size);
 return mallocVal;
}
void* realloc(void* old,uint_64 size){
    MemorySegment* oldseg;//establishing the target
    register AlignedMemorySegment* ams = ((AlignedMemorySegment*)old) - 1;
    if(ams->IsAligned){
        oldseg = (MemorySegment*)(uint_64)ams->MemorySegmentAddr;//establishing the target but its aligned
    }
    else{
    oldseg = ((MemorySegment*)old) - 1;//establishing the target
    }
    //recording the smaller size
    uint_64 smallersize = size;
    if (oldseg->length < size) smallersize = oldseg->length;

    free(old);// freeing it before so there will be less airgaps
    void* newmem = malloc(size);//mallocing the new aegment
    memcpy(newmem,old,smallersize);//copying the data there
    
    return newmem;
}

void* aligned_alloc(uint_64 alignment,uint_64 size)
{
    //keeping the full size
    uint_64 fullsize = size;

    //alligning the size
    uint_64 remainder = size % 8;
    size -= remainder;
    if (remainder != 0) size +=8;

    //alligning the allignment
    uint_64 aremainder = alignment % 8;
    alignment -= aremainder;
    if (aremainder != 0) alignment +=8;
    
    
    //malloc the segment
    void* mallocval = malloc(fullsize);
    uint_64 address = (uint_64)mallocval;

    //allign the address
    uint_64 remainder2 = address % alignment;
    address -= remainder2;
    // TODO: the AMS might be in another memory segment fix
    if (remainder2 != 0){
        address += alignment;
        //creating an aligned segment
        AlignedMemorySegment* ams = (AlignedMemorySegment*)address -1;
        ams->IsAligned           = true;
        ams->MemorySegmentAddr   = (unsigned long long)mallocval - sizeof(MemorySegment);
    }
    return (void*)address;
}
