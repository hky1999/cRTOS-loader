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

#include "MMAP.hpp"

MMAP::MMAP(int fd, size_t size) :
    MMAP(fd, (void*)-1, size, 0)
{
    return;
}

MMAP::MMAP(int fd, void* start, size_t size, off_t offset) :
    size(size),
    start(start)
{

    if( start == (void*)-1 ) {
        this->data_segment = (uint8_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    } else {
        this->data_segment = (uint8_t*)mmap(start, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, offset);
    }

    if (this->data_segment == MAP_FAILED) {
        perror("mmap failed");
        std::cout << "Start: " << std::hex << start << "  Offset: " << std::hex << offset << "  Size: " << std::hex << size << std::endl;
        throw std::runtime_error("Failed to map data segment");
    }
}

MMAP::~MMAP() {
    munmap(this->data_segment, this->size);
    return;
}

void* MMAP::memory() {
    return this->data_segment;
}
