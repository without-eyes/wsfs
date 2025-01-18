#ifndef FILE_H
#define FILE_H

#include <stdint.h>

struct FileAttributes {
 	char type;
  	uint64_t size;
    uint8_t hourCreatedAt;
    uint8_t minuteCreatedAt;
    char* name;
};

#endif //FILE_H