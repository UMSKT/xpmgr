cmake_minimum_required(VERSION 3.15)
project(xpmgr CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES xpmgr.cpp)
set(LIB_SOURCES xpmgr_lib.cpp)

# x86 is freestanding Pentium code. TDM-GCC's libgcc/libstdc++/CRT are built
# for a newer CPU and import XP SP2 APIs, so the x86 link does not use them.
set(X86_ARCH_FLAGS
        -m32
        -march=pentium
        -mtune=pentium
        -mfpmath=387
        -mno-mmx
        -mno-sse
        -mno-sse2
        -mno-3dnow
        -fno-tree-vectorize
        -fno-tree-loop-distribute-patterns
        -fno-exceptions
        -fno-rtti
        -fno-threadsafe-statics
        -fno-use-cxa-atexit
        -fno-stack-protector
        -fno-asynchronous-unwind-tables
        -fno-unwind-tables
        -ffreestanding
)
# Office XP's earliest hosts are Windows 98 and NT 4 SP6a. Keep the headers
# at that floor so Windows 2000 and XP APIs cannot become static imports.
set(X86_ARCH_DEFS
        WINVER=0x0400
        _WIN32_WINNT=0x0400
        NTDDI_VERSION=0x04000000
        ENVIRONMENT32
)

# ==== x86 build ====

add_library(xpmgr_static_x86 STATIC ${LIB_SOURCES})
target_compile_options(xpmgr_static_x86 PRIVATE ${X86_ARCH_FLAGS})
target_compile_definitions(xpmgr_static_x86 PRIVATE ${X86_ARCH_DEFS})
target_link_libraries(xpmgr_static_x86 PRIVATE ole32 oleaut32)

set(RES_X86 ${CMAKE_BINARY_DIR}/icon_x86.res)

add_custom_command(
        OUTPUT ${RES_X86}
        COMMAND windres -F pe-i386 -O coff ${CMAKE_SOURCE_DIR}/icon.rc -o ${RES_X86}
        DEPENDS ${CMAKE_SOURCE_DIR}/icon.rc ${CMAKE_SOURCE_DIR}/icon.ico
        COMMENT "Compiling icon.rc for x86"
)

add_custom_target(xpmgr_x86_res ALL DEPENDS ${RES_X86})

add_executable(xpmgr_x86 ${SOURCES} crt_win32.cpp)
add_dependencies(xpmgr_x86 xpmgr_x86_res)

target_compile_options(xpmgr_x86 PRIVATE ${X86_ARCH_FLAGS})
target_compile_definitions(xpmgr_x86 PRIVATE ${X86_ARCH_DEFS})
target_link_options(xpmgr_x86 PRIVATE
        -m32
        -nostdlib
        -nostartfiles
        -nodefaultlibs
        -Wl,--subsystem,console:4.0
        -Wl,--major-os-version,4
        -Wl,--minor-os-version,0
        -Wl,--major-subsystem-version,4
        -Wl,--minor-subsystem-version,0
        "SHELL:-Xlinker --stack -Xlinker 1048576,4096"
        -Wl,--disable-auto-import
        -Wl,-e,_mainCRTStartup
        ${RES_X86}
)
target_link_libraries(xpmgr_x86 PRIVATE xpmgr_static_x86 ole32 oleaut32 kernel32)

get_filename_component(XPMGR_GCC_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
find_program(XPMGR_OBJDUMP NAMES objdump HINTS "${XPMGR_GCC_BIN}")
if(NOT XPMGR_OBJDUMP)
    message(FATAL_ERROR "objdump is required to verify the Pentium / Office XP host floor")
endif()

add_custom_command(TARGET xpmgr_x86 POST_BUILD
        COMMAND powershell.exe -NoProfile -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/tools/check-x86-baseline.ps1" -Exe "$<TARGET_FILE:xpmgr_x86>" -Objdump "${XPMGR_OBJDUMP}"
        COMMENT "Checking xpmgr_x86 for a Pentium and the Office XP host floor"
        VERBATIM
)

# ==== x64 build ====

add_library(xpmgr_static_x64 STATIC ${LIB_SOURCES})
target_compile_options(xpmgr_static_x64 PRIVATE -m64)
target_link_libraries(xpmgr_static_x64 PRIVATE ole32 oleaut32 uuid)

set(RES_X64 ${CMAKE_BINARY_DIR}/icon_x64.res)

add_custom_command(
        OUTPUT ${RES_X64}
        COMMAND windres -O coff ${CMAKE_SOURCE_DIR}/icon.rc -o ${RES_X64}
        DEPENDS ${CMAKE_SOURCE_DIR}/icon.rc ${CMAKE_SOURCE_DIR}/icon.ico
        COMMENT "Compiling icon.rc for x64"
)

add_custom_target(xpmgr_x64_res ALL DEPENDS ${RES_X64})

add_executable(xpmgr_x64 ${SOURCES})
add_dependencies(xpmgr_x64 xpmgr_x64_res)

target_compile_options(xpmgr_x64 PRIVATE -m64)
target_link_options(xpmgr_x64 PRIVATE -m64)

target_link_libraries(xpmgr_x64 PRIVATE ole32 oleaut32 uuid xpmgr_static_x64)
target_link_options(xpmgr_x64 PRIVATE -Wl,--subsystem,console ${RES_X64})

# Install targets
install(TARGETS xpmgr_static_x86 xpmgr_static_x64
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES xpmgr.h DESTINATION include)
