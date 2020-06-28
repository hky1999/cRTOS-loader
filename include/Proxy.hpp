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
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>
#include <list>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "MMAP.hpp"
#include "IO.hpp"

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

class Proxy;

struct thread_s {
    pthread_t tid;
    Proxy* tproxy;
    int tpip[2];
};

class Proxy {
    public:
        Proxy(IO &io, std::string shadow, bool verbose);
        virtual ~Proxy();

        void rexec(std::string &path,
                   std::vector<std::string> &argv,
                   std::vector<std::string> &envp);

        void run();
        void send_remote_signal(int signo);

        uint64_t get_slot_num() { return slot_num; };

        void _self_init();
        void self_init();
        void init_sched();
        void init_signal_handling();

    protected:
    private:
        bool is_main;

        IO &io;
        MMAP *kernel_memory;

        std::string shadow;
        size_t shadow_size;
        int fd;
        uint64_t slot_num;

        std::list<struct thread_s*> threads;
        std::list<MMAP*> mappings;

        int priority;
        int policy;

        bool verbose;
};

#endif // __PROXY_HPP__
