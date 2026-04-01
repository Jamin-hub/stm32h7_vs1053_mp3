#ifndef _gbk2utf8_h_
#define _gbk2utf8_h_

#include "stdint.h"


// 定义联合体用于字节操作
typedef union {
    uint16_t val;
    uint8_t buf[2];
} gbk_union_t;

typedef union {
    uint32_t val;
    uint8_t buf[4];
} utf8_union_t;



//extern const gbk2utf8_TypeDef gbk2utf8_table[21946];

void str_gbk2utf8(char *gbk, char *utf8);

#endif /* _gbk2utf8_h_ */
