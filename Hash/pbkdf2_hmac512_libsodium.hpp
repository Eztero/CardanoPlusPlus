/**
MIT License

Copyright (c) 2022 Eztero

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Documentation:
https://www.rfc-editor.org/rfc/rfc8018.html#section-5.2
https://libsodium.gitbook.io/doc/advanced/hmac-sha2
https://github.com/jedisct1/libsodium/blob/master/src/libsodium/crypto_pwhash/scryptsalsa208sha256/pbkdf2-sha256.c
**/

#ifndef PBKDF2_HMAC512_LIBSODIUM_H
#define PBKDF2_HMAC512_LIBSODIUM_H

#include <cstdint>
#include <cstring>
#include <cmath>
#include <sodium.h>



bool pbkdf2_hmac512_libsodium( std::uint8_t const *const key, std::uint8_t const key_len,
                               std::uint8_t const *const salt, std::uint8_t const salt_len,
                               std::uint32_t const iterations,
                               std::uint32_t const out_len , std::uint8_t *const out );

#endif
