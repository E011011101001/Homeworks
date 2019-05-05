#ifndef __BITIO_H_EOL
#define __BITIO_H_EOL

#include <string>
#include <cassert>
#include <fstream>

struct OBitStream
{
    std::string filename;
    std::ofstream fout;
    unsigned char buf;
    size_t cnt;
};

OBitStream& OBStream_create(const std::string& filename);
OBitStream& operator <<(OBitStream& bout, const std::string& str);
OBitStream& OBStream_flush(OBitStream& bout);
void OBStream_destroy(OBitStream& bout);

#endif // __BITIO_H_EOL

