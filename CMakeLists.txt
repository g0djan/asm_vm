cmake_minimum_required(VERSION 3.13)
project(asm_vm C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(GTEST googletest/googletest)

include_directories(${GTEST} ${GTEST}/include)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(zfake_vm vm/vm.c vm/constants.h vm/process.c vm/process.h vm/instructions.c vm/instructions.h vm/fetcher.c vm/fetcher.h)

add_executable(zfake_asm asm/asm.c asm/labels_table.c asm/labels_table.h asm/parser.c asm/parser.h asm/constants.h asm/encoder.c asm/encoder.h)

add_executable(zfake_tests ${GTEST}/src/gtest_main.cc ${GTEST}/src/gtest-all.cc tests/check.cpp)

########################################
# Custom Makefile
set(GTEST ${asm_vm_SOURCE_DIR}/googletest/googletest)

add_custom_target(ASM
        WORKING_DIRECTORY ${asm_vm_SOURCE_DIR}/build
        COMMAND make -f ${asm_vm_SOURCE_DIR}/Makefile asm
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        LD=${CMAKE_CXX_COMPILER}
        GTEST=${GTEST}
        SRCD=${asm_vm_SOURCE_DIR})

add_custom_target(VM
        WORKING_DIRECTORY ${asm_vm_SOURCE_DIR}/build
        COMMAND make -f ${asm_vm_SOURCE_DIR}/Makefile vm
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        LD=${CMAKE_CXX_COMPILER}
        GTEST=${GTEST}
        SRCD=${asm_vm_SOURCE_DIR})

add_custom_target(Check
        WORKING_DIRECTORY ${asm_vm_SOURCE_DIR}/build
        COMMAND make -f ${asm_vm_SOURCE_DIR}/Makefile check
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        LD=${CMAKE_CXX_COMPILER}
        GTEST=${GTEST}
        SRCD=${asm_vm_SOURCE_DIR})

add_custom_target(Clean
        WORKING_DIRECTORY ${asm_vm_SOURCE_DIR}/build
        COMMAND make -f ${asm_vm_SOURCE_DIR}/Makefile clean
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        LD=${CMAKE_CXX_COMPILER}
        GTEST=${GTEST}
        SRCD=${asm_vm_SOURCE_DIR})