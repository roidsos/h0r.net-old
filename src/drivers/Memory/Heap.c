#include "Heap.h"
#include "PFA.h"
#include <arch/x86_64/essential.h>
struct MemorySegment *first_free;

void InitHeap(uint64_t heaplength) {
  void *heapstart = request_page();
  lock_pages(heapstart, heaplength / 0x1000 + 1);

  first_free = (struct MemorySegment *)heapstart;
  first_free->length = heaplength - sizeof(struct MemorySegment);
  first_free->next = 0;
  first_free->previous = 0;
  first_free->nextfree = 0;
  first_free->previousfree = 0;
  first_free->isfree = true;
}
void *malloc(uint64_t size) {
  uint64_t remainder = size % 8;
  size -= remainder;
  if (remainder != 0)
    size += 8;
  struct MemorySegment *current = first_free;
  while (true) {
    if (current->length >= size) {
      if (current->length > size + sizeof(struct MemorySegment)) {
        struct MemorySegment *newSegment =
            (struct MemorySegment *)((uint64_t)current +
                                     sizeof(struct MemorySegment) +
                                     size); // making a new segment
        newSegment->isfree = true;          // giving it values
        newSegment->length =
            (((uint64_t)current->length) - sizeof(struct MemorySegment) + size);
        newSegment->next = current->next;
        newSegment->nextfree = current->nextfree;
        newSegment->previous = current;
        newSegment->previousfree = current->previousfree;

        current->nextfree = newSegment; // setting current's next segment
        current->next = newSegment;

        current->length = size; // giving it the new size
      }
      if (current == first_free) {
        first_free = current->nextfree;
      }
      current->isfree = false; // allocating it

      if (current->previousfree != 0)
        current->previousfree->nextfree =
            current->nextfree; // skipping the current backwards for the frees
      if (current->nextfree != 0)
        current->nextfree->previousfree =
            current
                ->previousfree; // skipping the current forwards for the frees
      if (current->previous != 0)
        current->previous->nextfree =
            current->nextfree; // skipping the current backwards
      if (current->next != 0)
        current->next->previousfree =
            current->previousfree; // skipping the current forwards

      return current + 1; // sizeof(struct MemorySegment);
    }
    if (current->nextfree == 0) {
      return 0; // out of memory
    }
    current = current->nextfree;
  }
  return 0; // this line is just for decoration lol
}
void CombineSegments(struct MemorySegment *a, struct MemorySegment *b) {
  if (a == 0 || b == 0) {
    return;
  }
  if (a < b) {
    // set up a's properties
    a->length += b->length + sizeof(struct MemorySegment);
    a->next = b->next;
    a->nextfree = b->nextfree;
    // make the previous of the next concistant
    b->next->previous = a;
    // if its free modify the nextfrees
    if (a->isfree) {
      b->next->previousfree = a;
      b->nextfree->previousfree = a;
    }
  }
  if (a > b) {
    // set up b's properties
    b->length += a->length + sizeof(struct MemorySegment);
    b->next = a->next;
    b->nextfree = a->nextfree;
    // make the previous of the next concistant
    a->next->previous = b;
    // if its free modify the nextfrees
    if (a->isfree) {
      a->next->previousfree = b;
      a->nextfree->previousfree = b;
    }
  }
}
void free(void *tofree) {
  struct MemorySegment *current;
  register struct AlignedMemorySegment *ams =
      ((struct AlignedMemorySegment *)tofree) - 1;
  if (ams->IsAligned) {
    current =
        (struct MemorySegment *)(uint64_t)
            ams->MemorySegmentAddr; // establishing the target but its aligned
  } else {
    current = ((struct MemorySegment *)tofree) - 1; // establishing the target
  }
  current->isfree = true; // freeing the segment

  if (current < first_free)
    first_free = current; // checking if this is the first one

  // correcting potencial issues for the frees
  if (current->nextfree != 0) {
    if (current->nextfree->previousfree < current) {
      current->nextfree->previousfree = current;
    }
  }
  if (current->previousfree != 0) {
    if (current->previousfree->nextfree < current) {
      current->previousfree->nextfree = current;
    }
  }
  // correcting potencial issues
  if (current->next != 0) {
    if (current->next->previous < current) {
      current->next->previous = current;
      if (current->next->isfree)
        CombineSegments(current, current->next); // removing fragmentation
    }
  }
  if (current->previous != 0) {
    if (current->previous->next < current) {
      current->previous->next = current;
      if (current->previous->isfree)
        CombineSegments(current, current->previous); // removing fragmentation
    }
  }
}
void *calloc(uint64_t size) {
  void *mallocVal = malloc(size);
  memset(mallocVal, 0, size);
  return mallocVal;
}
void *realloc(void *old, uint64_t size) {
  struct MemorySegment *oldseg; // establishing the target
  register struct AlignedMemorySegment *ams =
      ((struct AlignedMemorySegment *)old) - 1;
  if (ams->IsAligned) {
    oldseg =
        (struct MemorySegment *)(uint64_t)
            ams->MemorySegmentAddr; // establishing the target but its aligned
  } else {
    oldseg = ((struct MemorySegment *)old) - 1; // establishing the target
  }
  // recording the smaller size
  uint64_t smallersize = size;
  if (oldseg->length < size)
    smallersize = oldseg->length;

  free(old); // freeing it before so there will be less airgaps
  void *newmem = malloc(size);      // mallocing the new aegment
  memcpy(newmem, old, smallersize); // copying the data there

  return newmem;
}

void *aligned_alloc(uint64_t alignment, uint64_t size) {
  // keeping the full size
  uint64_t fullsize = size;

  // alligning the size
  uint64_t remainder = size % 8;
  size -= remainder;
  if (remainder != 0)
    size += 8;

  // alligning the allignment
  uint64_t aremainder = alignment % 8;
  alignment -= aremainder;
  if (aremainder != 0)
    alignment += 8;

  // malloc the segment
  void *mallocval = malloc(fullsize);
  uint64_t address = (uint64_t)mallocval;

  // allign the address
  uint64_t remainder2 = address % alignment;
  address -= remainder2;
  // TODO: the AMS might be in another memory segment fix
  if (remainder2 != 0) {
    address += alignment;
    // creating an aligned segment
    struct AlignedMemorySegment *ams = (struct AlignedMemorySegment *)address -
                                       sizeof(struct AlignedMemorySegment);
    ams->IsAligned = true;
    ams->MemorySegmentAddr =
        (unsigned long long)mallocval - sizeof(struct MemorySegment);
  }
  return (void *)address;
}