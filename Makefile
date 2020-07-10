# Copyright (c) 2020 Yang, Chung-Fan @ Fixstars corporation
#                                      <chungfan.yang@fixstars.com>
# All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
