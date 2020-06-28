# Copyright (c) 2020 Yang, Chung-Fan <chungfan.yang@fixstars.com>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the project's author nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CXX = g++-9
LD = /usr/lib/gcc/x86_64-linux-gnu/9/collect2

CXXFLAGS = -std=c++1z -Iinclude -O2

LDFLAGS = -pthread -Wl,--Ttext-segment,0x40000000

BIN = loader

OBJEXT = .o
SRCS = src/IO.cpp src/MMAP.cpp src/Proxy.cpp src/SyscallDecoder.cpp
OBJS = $(SRCS:.cpp=$(OBJEXT))

# Build targets

all: $(BIN)
.PHONY: clean

src/raw_syscall.o: src/raw_syscall.S
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(OBJS): %$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(BIN): $(OBJS) src/main.cpp src/raw_syscall.o
	$(CXX) $(CXXFLAGS) -c -o src/main.o src/main.cpp 
	$(CXX) -o $@ $(OBJS) src/raw_syscall.o src/main.o $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN)
	rm -f src/*.o
