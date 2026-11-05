#include <stdint.h>

uint64_t normalize_unicode(uint32_t* string_src, uint64_t len_src, uint32_t* string_out) { 
    uint32_t special_under = 9601;
    string_out[0] = special_under;
    for (uint64_t counter_src = 0; counter_src < len_src; counter_src++){
        if (string_src[counter_src] == ' '){ 
            string_out[counter_src + 1] = special_under; 
        }
        else{
            string_out[counter_src + 1] = string_src[counter_src];

        }
    }
    return len_src + 1;
}


