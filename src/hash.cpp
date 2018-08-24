// Copyright (c) 2013-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "hash.h"
#include "crypto/common.h"
#include "crypto/hmac_sha512.h"
#include "crypto/scrypt/scrypt.h"
#include "crypto/cryptonight/libcryptonight.h"
#include "crypto/sph/sph_cubehash.h"
#include "crypto/sph/sph_echo.h"

#include "pubkey.h"


inline uint32_t ROTL32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

unsigned int MurmurHash3(unsigned int nHashSeed, const std::vector<unsigned char>& vDataToHash)
{
    // The following is MurmurHash3 (x86_32), see http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
    uint32_t h1 = nHashSeed;
    if (vDataToHash.size() > 0)
    {
        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        const int nblocks = vDataToHash.size() / 4;

        //----------
        // body
        const uint8_t* blocks = &vDataToHash[0] + nblocks * 4;

        for (int i = -nblocks; i; i++) {
            uint32_t k1 = ReadLE32(blocks + i*4);

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        //----------
        // tail
        const uint8_t* tail = (const uint8_t*)(&vDataToHash[0] + nblocks * 4);

        uint32_t k1 = 0;

        switch (vDataToHash.size() & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
        };
    }

    //----------
    // finalization
    h1 ^= vDataToHash.size();
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

void BIP32Hash(const ChainCode &chainCode, unsigned int nChild, unsigned char header, const unsigned char data[32], unsigned char output[64])
{
    unsigned char num[4];
    num[0] = (nChild >> 24) & 0xFF;
    num[1] = (nChild >> 16) & 0xFF;
    num[2] = (nChild >>  8) & 0xFF;
    num[3] = (nChild >>  0) & 0xFF;
    CHMAC_SHA512(chainCode.begin(), chainCode.size()).Write(&header, 1).Write(data, 32).Write(num, 4).Finalize(output);
}

void CPowHash256::Finalize(unsigned char hash[OUTPUT_SIZE]) {
    unsigned char buf[CSHA256::OUTPUT_SIZE];
    sha.Finalize(buf);
    // sha256
    sha.Reset().Write(buf, CSHA256::OUTPUT_SIZE).Finalize(hash);

    // scrypt
    char scrypt_input[80];
    memcpy(scrypt_input +  0, hash, 32);
    memcpy(scrypt_input + 32, hash, 32);
    memcpy(scrypt_input + 64, hash, 16);
    scrypt_1024_1_1_256(&scrypt_input[0], (char *)hash);

    // cryptonight
    if (false) cryptonight_hash(hash, hash, 32);

    // sph_cubehash
    uint512 cube_hash512;
    sph_cubehash512_context ctx_cubehash;
    sph_cubehash512_init(&ctx_cubehash);
    sph_cubehash512 (&ctx_cubehash, static_cast<const void*>(hash), 32);
    sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&cube_hash512));

    // sph_echo
    uint512 echo_hash512;
    sph_echo512_context ctx_echo;
    sph_echo512_init(&ctx_echo);
    sph_echo512 (&ctx_echo, static_cast<const void*>(&cube_hash512), 64);
    sph_echo512_close(&ctx_echo, static_cast<void*>(&echo_hash512));

    uint256 res = echo_hash512.trim256();
    memcpy(hash, (const unsigned char *)&res, 32);
}
