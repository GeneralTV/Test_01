#pragma once
#include <vector>

struct FileData {
    const unsigned char* data;
    size_t size;
};

// Макрос для преобразования строки в байты
#define STR_TO_BYTES(s) reinterpret_cast<const unsigned char*>(s)
