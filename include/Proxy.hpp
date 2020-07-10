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
