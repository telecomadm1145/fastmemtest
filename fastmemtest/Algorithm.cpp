#include "Algorithm.h"
namespace nt
{
    namespace hs
    {
        unsigned long long crc_table[256] = { 0 };
        bool inited_crc_table = false;
        void init_crc_table()
        {
            if (!inited_crc_table)
            {
                for (int i = 0; i < 256; i++)
                {
                    StringHash CRC = i;
                    for (int j = 0; j < 8; j++)
                    {
                        if (CRC & 1)
                            CRC = (CRC >> 1) ^ 0xEDB8832048AD6240;
                        else
                            CRC >>= 1;
                    }
                    crc_table[i] = CRC;
                }
                inited_crc_table = true;
            }
        }
        StringHash calc_crc(const unsigned char* buf, size_t len)
        {
            init_crc_table();
            auto res = (StringHash)-1;
            for (size_t i = 0; i != len; ++i)
            {
                auto t = (res ^ buf[i]) & 0xFF;
                res = (res >> 8) ^ crc_table[t];
            }
            return res;
        }
    }
    using namespace hs;
    StringHash _get_crc(const unsigned char* buf, size_t len)
    {
        return calc_crc(buf,len);
    }
}