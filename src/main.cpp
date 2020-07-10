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
#include <signal.h>

#include "MMAP.hpp"
#include "IO.hpp"
#include "Proxy.hpp"
#include "verbose.hpp"

#define __VERBOSE verbose

static std::string get_filename(const std::string& s)
{
   char sep = '/';

   size_t i = s.rfind(sep, s.length());
   if (i != std::string::npos) {
      return(s.substr(i + 1, s.length() - i));
   }

   return("");
}

std::string readenv(std::string in, std::string def)
{
    if (std::getenv(in.c_str()) != NULL) {
        return std::string(std::getenv(in.c_str()));
    } else {
        return def;
    }
    return std::string("NULL");
}

void print_usage()
{
    std::cout << "Usage: loader <optional env> <elf file> ..." << std::endl;
    std::cout << "Environment variable APPMEM must be set to corresponding uio device" << std::endl;
}

int main(int argc, char *argv[], char *envp[]) {
    bool verbose = (readenv("VERBOSE", "0") == "1");
    bool magic = (readenv("MAGIC", "0") == "1");

    std::vector<std::string> llargv;
    std::vector<std::string> llenvv;

    int i;

    /* Early declaration for additional arguments or environs */

    for (i = 1; i < argc; i++) {
        auto curr = std::string(argv[i]);
        if (curr.find('=') != std::string::npos) {
            // Take a string with '=' as env
            llenvv.push_back(curr);
        } else {
            break;
        }
    }

    /* Nothing left? */

    if ( argc - i < 1 ) {
        print_usage();
        exit(1);
    }

    /* Construct the argv and envv list */

    std::string elf_filename(argv[i++]);

    llargv.push_back(elf_filename);
    for ( ; i < argc; i++) {
        llargv.push_back(std::string(argv[i]));
    }

    for (i = 0; envp[i]; i++) {
        llenvv.push_back(std::string(envp[i]));
    }

    /* Create the TCP/IP and Shadow channel
     * Test the connection and spawn the Proxy */

    std::string shadow = readenv("SHADOWPROC", "/dev/shadow-process0");

    auto io = IO("172.16.0.2", 42, verbose);

    VERBOSE("Testing Communication...");
    io.ping();

    auto pxy = Proxy(io, shadow, verbose);

    /* Reserve the Lower 1GB of the memory, used by application
     * to prevent libc and kernel being funky
     */
    if (!mmap((void*)0x0, 0x40000000,
              PROT_NONE, MAP_SHARED, 0, 0)) {
        throw std::runtime_error("Unable to reserve the low memory space!");
    }

    /* Reserve the Upper portion of the memory, used by Nuttx kernel
     * to prevent libc and kernel being funky
     */
    if (!mmap((void*)KERNELMEM_VIRT_START, KERNELMEM_SIZE,
             PROT_NONE, MAP_SHARED, 0, 0)) {
        throw std::runtime_error("Unable to reserve the low memory space!");
    }

    pxy.rexec(elf_filename, llargv, llenvv);

    return 0;
}
