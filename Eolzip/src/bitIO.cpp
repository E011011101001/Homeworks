#include "bitIO.hpp"


OBitStream& OBStream_create(const std::string& filename)
{
    auto* bout = new OBitStream;
    bout->filename = filename;
    bout->fout.open(filename);
    bout->buf = 0;
    bout->cnt = 0;
    return *bout;
}

OBitStream& operator <<(OBitStream& bout, const std::string& str)
{
    for (const auto& ch : str)
    {
        assert(ch == '1' || ch == '0');
        if (bout.cnt >= 8)
            OBStream_flush(bout);
        bout.buf <<= 1;
        bout.buf |= ch - '0';
        ++bout.cnt;
    }
    return bout;
}

OBitStream& OBStream_flush(OBitStream& bout)
{
    if (bout.cnt)
    {
        bout.buf <<= (8 - bout.cnt);
        bout.fout << bout.buf;
        bout.buf = 0;
        bout.cnt = 0;
    }
    return bout;
}

void OBStream_destroy(OBitStream& bout)
{
    OBStream_flush(bout);
    bout.fout.close();
    delete &bout;
}

