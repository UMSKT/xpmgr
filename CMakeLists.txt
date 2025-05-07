cmake_minimum_required(VERSION 3.15)
project(xpmgr CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES xpmgr.cpp)

# ==== x86 build ====
set(RES_X86 ${CMAKE_BINARY_DIR}/icon_x86.res)

# Create a custom target for the x86 resource compilation
add_custom_command(
        OUTPUT ${RES_X86}
        COMMAND windres -F pe-i386 -O coff ${CMAKE_SOURCE_DIR}/icon.rc -o ${RES_X86}
        DEPENDS ${CMAKE_SOURCE_DIR}/icon.rc ${CMAKE_SOURCE_DIR}/icon.ico
        COMMENT "Compiling icon.rc for x86"
)

add_custom_target(xpmgr_x86_res ALL DEPENDS ${RES_X86})

# Now define the executable and link dependencies
add_executable(xpmgr_x86 ${SOURCES})
add_dependencies(xpmgr_x86 xpmgr_x86_res)

target_compile_options(xpmgr_x86 PRIVATE -m32)
target_link_options(xpmgr_x86 PRIVATE -m32)

# Link .res file as a resource
target_link_libraries(xpmgr_x86 PRIVATE ole32 oleaut32 uuid)
target_link_options(xpmgr_x86 PRIVATE -Wl,--subsystem,console ${RES_X86})

# ==== x64 build ====
set(RES_X64 ${CMAKE_BINARY_DIR}/icon_x64.res)

# Create a custom target for the x64 resource compilation
add_custom_command(
        OUTPUT ${RES_X64}
        COMMAND windres -O coff ${CMAKE_SOURCE_DIR}/icon.rc -o ${RES_X64}
        DEPENDS ${CMAKE_SOURCE_DIR}/icon.rc ${CMAKE_SOURCE_DIR}/icon.ico
        COMMENT "Compiling icon.rc for x64"
)

add_custom_target(xpmgr_x64_res ALL DEPENDS ${RES_X64})

# Now define the executable and link dependencies
add_executable(xpmgr_x64 ${SOURCES})
add_dependencies(xpmgr_x64 xpmgr_x64_res)

target_compile_options(xpmgr_x64 PRIVATE -m64)
target_link_options(xpmgr_x64 PRIVATE -m64)

# Link .res file as a resource
target_link_libraries(xpmgr_x64 PRIVATE ole32 oleaut32 uuid)
target_link_options(xpmgr_x64 PRIVATE -Wl,--subsystem,console ${RES_X64})
