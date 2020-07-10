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

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef __MMAP_HPP__
#define __MMAP_HPP__

#define ATTR_NONE 0x0
#define ATTR_MORE 0x1

#define SHADOW_START          0x0
#define KERNELMEM_SIZE        0x08000000
#define KERNELMEM_PHYS_START  SHADOW_START
#define KERNELMEM_VIRT_START (KERNELMEM_PHYS_START + 0x100000000)

class MMAP {
    public:
        MMAP(int fd, size_t size);
        MMAP(int fd, void* start, size_t size, off_t offset);
        virtual ~MMAP();

        void* memory();

    private:
        size_t size;
        void* start;
        off_t offset;

        void* data_segment;
};

#endif // __MMAP_HPP__
