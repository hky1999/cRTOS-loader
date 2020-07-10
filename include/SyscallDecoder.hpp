/* Copyright (c) 2020 Yang, Chung-Fan @ Fixstars corporation
 *                                      <chungfan.yang@fixstars.com>
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
