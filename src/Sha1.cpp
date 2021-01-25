#include "Sha1.hpp"

#include <cstring>
#include <sstream>
#include <iomanip>

namespace
{
    uint32_t circular_rotate_left(uint32_t word, unsigned short n_bits)
    {
        return (word << n_bits) | (word >> (32 - n_bits));
    }
}

namespace Sha1
{
    std::string sha1_encrypt(const std::string& data)
    {
        std::vector< uint8_t > data_as_vector(data.length());
        if(!data_as_vector.empty())
        {
            (void)memcpy(data_as_vector.data(), data.c_str(), data_as_vector.size());
        }
        return sha1_encrypt(data_as_vector);
    }

    std::string sha1_encrypt(const std::vector< uint8_t >& data)
    {
        const auto message_digest = sha1_encrypt_into_bytes(data);
        std::ostringstream digest_string_builder;
        digest_string_builder << std::hex << std::setfill('0');
        for(auto digest_byte : message_digest)
        {
            digest_string_builder << std::setw(2) << (int)digest_byte;
        }
        return digest_string_builder.str();
    }

    std::vector< uint8_t > sha1_encrypt_into_bytes(const std::string& data)
    {   
        // convert given data string to data bytes vector.
        std::vector< uint8_t > raw_bytes;
        for(auto character : data)
        {
            raw_bytes.push_back(character);
        }
        return sha1_encrypt_into_bytes(raw_bytes); // return type is wrong!!!
    }
    
    std::vector< uint8_t > sha1_encrypt_into_bytes(const std::vector< uint8_t >& data)
    {
        // This a straightforward implementation of the pseudocode found in the Wikipedia page for SHA-1
        // (https://en.wikipedia.org/wiki/SHA-1).
        uint8_t chunk[64];  // 512-bit chunk
        uint32_t w[80];     // 5*16*32 = 80 * 32 (extend words from 16 to 80)
        uint32_t h0 = 0x67452301;
        uint32_t h1 = 0xEFCDAB89;
        uint32_t h2 = 0x98BADCFE;
        uint32_t h3 = 0x10325476;
        uint32_t h4 = 0xC3D2E1F0;
        uint64_t ml = (uint64_t)data.size() * 8;    // message length
        // why +9 ? 
        // offset at most equals to ( data.size() + 8 ) bytes.
        // offset is the position of byte. 64*8 = 512
        for (size_t offset = 0; offset < data.size() + 9; offset += 64) {
            if (offset + 64 <= data.size()) {
                (void)memcpy(chunk, &data[offset], 64);
            } else { 
                // offset + 64 > data.size(), which means the last block is less than 512-bit.
                // In this case, we need to add padding.

                (void)memset(chunk, 0, 64); // Set 512-bit to '0's
                // First, add original data to chunk buffer.
                if (offset < data.size()) {
                    (void)memcpy(chunk, &data[offset], data.size() - offset);
                }
                // Second, add '1000 0000'.
                if (offset <= data.size()) {
                    chunk[data.size() - offset] = 0x80;
                }
                // Finally, if padding is greater than or equal to 8*9=72 bit, 
                // add original message length to the last 8 byte of chunk/message. 
                if (offset + 64 - data.size() >= 9) {
                    // set message length to last 64-bit of the last block(512-bit);
                    // cast uint64_t to uint8_t is reserver the rightmost byte?
                    chunk[56] = (uint8_t)(ml >> 56);
                    chunk[57] = (uint8_t)(ml >> 48);
                    chunk[58] = (uint8_t)(ml >> 40);
                    chunk[59] = (uint8_t)(ml >> 32);
                    chunk[60] = (uint8_t)(ml >> 24);
                    chunk[61] = (uint8_t)(ml >> 16);
                    chunk[62] = (uint8_t)(ml >> 8);
                    chunk[63] = (uint8_t)ml;
                }
            }
            for (size_t i = 0; i < 16; ++i) {
                w[i] = (
                    ((uint32_t)chunk[i * 4 + 0] << 24)
                    | ((uint32_t)chunk[i * 4 + 1] << 16)
                    | ((uint32_t)chunk[i * 4 + 2] << 8)
                    | (uint32_t)chunk[i * 4 + 3]
                );
            }
            for (size_t i = 16; i < 80; ++i) {
                w[i] = circular_rotate_left(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
            }
            uint32_t a = h0;
            uint32_t b = h1;
            uint32_t c = h2;
            uint32_t d = h3;
            uint32_t e = h4;
            for (size_t i = 0; i < 80; ++i) {
                uint32_t f, k;
                if (i < 20) {
                    f = (b & c) | ((~b) & d);
                    k = 0x5A827999;
                } else if (i < 40) {
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                } else if (i < 60) {
                    f = (b & c) | (b & d) | (c & d);
                    k = 0x8F1BBCDC;
                } else {
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                }
                uint32_t temp = circular_rotate_left(a, 5) + f + e + k + w[i];
                e = d;
                d = c;
                c = circular_rotate_left(b, 30);
                b = a;
                a = temp;
            }
            h0 += a;
            h1 += b;
            h2 += c;
            h3 += d;
            h4 += e;
        }
        return {
            /**
             * Return Message Digest.
             * 5x32 = 160-bit
             */
            (uint8_t)((h0 >> 24) & 0xff),
            (uint8_t)((h0 >> 16) & 0xff),
            (uint8_t)((h0 >> 8) & 0xff),
            (uint8_t)(h0 & 0xff),

            (uint8_t)((h1 >> 24) & 0xff),
            (uint8_t)((h1 >> 16) & 0xff),
            (uint8_t)((h1 >> 8) & 0xff),
            (uint8_t)(h1 & 0xff),
            
            (uint8_t)((h2 >> 24) & 0xff),
            (uint8_t)((h2 >> 16) & 0xff),
            (uint8_t)((h2 >> 8) & 0xff),
            (uint8_t)(h2 & 0xff),
            
            (uint8_t)((h3 >> 24) & 0xff),
            (uint8_t)((h3 >> 16) & 0xff),
            (uint8_t)((h3 >> 8) & 0xff),
            (uint8_t)(h3 & 0xff),
            
            (uint8_t)((h4 >> 24) & 0xff),
            (uint8_t)((h4 >> 16) & 0xff),
            (uint8_t)((h4 >> 8) & 0xff),
            (uint8_t)(h4 & 0xff),
        };
    }
}