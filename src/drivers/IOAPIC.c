#include "IOAPIC.h"
#include "core/memory.h"
#include "vendor/printf.h"


void *ioapicaddr = NULL;
uint8_t vector_count;

uint32_t read(uint32_t reg)
{
   uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
   ioapic[0] = (reg & 0xff);
   return ioapic[4];
}
 
void write( uint32_t reg, uint32_t value)
{
   uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
   ioapic[0] = (reg & 0xff);
   ioapic[4] = value;
}

bool init_ioapic(){
    //TODO: do not assume HART 0
    for(size_t i = 0; i < num_madt_entries;i++){
        if(entry_ptrs[i]->h.type == MADT_ENTRY_TYPE_IOAPIC){
            ioapicaddr = (void*)PHYS_TO_VIRT(entry_ptrs[i]->the_meat.ioapic.adress);
            break;
        }
    }
    if(ioapicaddr == NULL){
        return false;
    }
    vector_count = ((read(IOAPIC_VER) >> 16) & 0xFF);

    // mask all and set 
    for (uint32_t i = 0; i <= vector_count; ++i) {
        write(IOAPIC_REDTBL+2*i, 0x00010000 | (32 + i));
        write(IOAPIC_REDTBL+2*i+1, 0); // redir cpu
    }

    return false;
}

void mask(uint32_t id){
    write(IOAPIC_REDTBL+2*id,0x00010000 | 1 << 11 | (32 + id));
    write(IOAPIC_REDTBL+2*id+1, 1 << 23); // redir cpu
}
void unmask(uint32_t id){
    write(IOAPIC_REDTBL+2*id,1 << 11 | (32 + id));
    write(IOAPIC_REDTBL+2*id+1, 1 << 23); // redir cpu
}
