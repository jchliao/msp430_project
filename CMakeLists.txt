cmake_minimum_required(VERSION 3.20)
project(example C)

set(CMAKE_C_STANDARD 99)

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/Core/Src/*.c
        ${PROJECT_SOURCE_DIR}/Core/Inc/*.h
        ${PROJECT_SOURCE_DIR}/Device/Src/*.c
        ${PROJECT_SOURCE_DIR}/Device/Inc/*.h
        ${PROJECT_SOURCE_DIR}/Device/SD/*.*
        #${PROJECT_SOURCE_DIR}/Device/UART/*.*
        ${PROJECT_SOURCE_DIR}/Device/LCD/*.*
        ${PROJECT_SOURCE_DIR}/Device/ScopeADC/*.*
        ${PROJECT_SOURCE_DIR}/MyApp/*.*)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

set(DEVICE msp430g2553)
set(MAP "${CMAKE_PROJECT_NAME}.map")

set(MAP430_GCC "/Users/liaojincheng/ti/msp430-gcc")

set(CMAKE_C_COMPILER    "${MAP430_GCC}/bin/msp430-elf-gcc")
set(CMAKE_CXX_COMPILER  "${MAP430_GCC}/bin/msp430-elf-g++")

#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

include_directories(${MAP430_GCC}/include Core/Inc Device/Inc Device/SD Device/LCD Device/ScopeADC MyApp)
link_directories(${MAP430_GCC}/include)

set(CMAKE_C_FLAGS "-mmcu=${DEVICE} -Og -Wall ")
set(CMAKE_C_LINK_FLAGS "-Wl,-Map,${MAP},--gc-sections")

set(CMAKE_BUILD_TYPE Debug)