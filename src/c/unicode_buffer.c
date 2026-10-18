#include <stdint.h>
#include <errno.h>
#include <stdio.h>

uint64_t utf8_to_unicode(uint8_t *src, uint64_t src_len, uint32_t *dst, uint64_t dst_len){
    uint64_t dst_i = 0;
    uint64_t src_i = 0;
    uint64_t byte_to_analyze;
    uint32_t char_unicode;
    
    while(src_i < src_len){

        char_unicode = 0;
        
        if(src[src_i] < 0x80){
            byte_to_analyze = 1;
            char_unicode = src[src_i];                         
        }
        else if(src[src_i] < 0xe0){
            byte_to_analyze = 2;
            char_unicode = (src[src_i] & 0b00011111) << 6;
        }
        else if(src[src_i] < 0xf0){
            byte_to_analyze = 3;
            char_unicode = (src[src_i] & 0b00001111) << 12;
        }
        else{
            byte_to_analyze = 4;
            char_unicode = (src[src_i] & 0b00000111) << 18;
        }

        src_i ++;

        for(uint64_t byte_seen = 1; byte_seen < byte_to_analyze; byte_seen++){
            char_unicode = char_unicode + ((src[src_i] & 0b00111111) << ((byte_to_analyze - byte_seen - 1) * 6));
            src_i++;
        }

        if(dst_i < dst_len){
            dst[dst_i] = char_unicode;
        }

        dst_i++;
    }

    return dst_i;    
}

