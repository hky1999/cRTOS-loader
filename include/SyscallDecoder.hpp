/* Copyright (c) 2020 Yang, Chung-Fan <chungfan.yang@fixstars.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the project's author nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#ifndef __SYSCALL_DECODER_HPP__
#define __SYSCALL_DECODER_HPP__

struct syscall_entry {
    char name[33];
    char param_format[16];
    int error_code;
};

class SyscallDecoder {
    public:
        SyscallDecoder(const SyscallDecoder&) = delete;
        SyscallDecoder& operator=(const SyscallDecoder&) = delete;
        SyscallDecoder(SyscallDecoder&&) = delete;
        SyscallDecoder& operator=(SyscallDecoder&&) = delete;

        static SyscallDecoder& get_instance()
        {
            static SyscallDecoder instance(true);
            return instance;
        }

        void decode(uint64_t nbr, uint64_t *params, uint64_t ret);

    protected:
    private:
        SyscallDecoder(bool verbose);
        virtual ~SyscallDecoder();

        bool verbose;
        struct syscall_entry syscall_table[350];
};

#endif // __SYSCALL_DECODER_HPP__
