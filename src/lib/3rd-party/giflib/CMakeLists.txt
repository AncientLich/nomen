# ------------------------------------
#  CMakeLists.txt for WINDOWS build
#  Written by Nobun, 2019
#  mugenrebirth.forumfree.it
# ------------------------------------

project(gif4)


add_definitions(-fPIC -O2 -DWIN32 -D_MBCS -D_LIB -D_OPEN_BINARY -DHAVE_IO_H -DHAVE_FCNTL_H -DHAVE_STDARG_H -DHAVE_BASETSD_H)



set(GIFLIB_INCLUDE_PATH
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)



# ---------------------------------------------------
# sources
# ---------------------------------------------------


set(GIFLIB_SRCS
    lib/dev2gif.c
    lib/dgif_lib.c
    lib/egif_lib.c
    lib/getarg.c
    lib/gif_err.c
    lib/gif_font.c
    lib/gifalloc.c
    lib/qprintf.c
    lib/quantize.c
    lib/gif_hash.c
)
 

# -----------------------------------------
#  appling settings
# -----------------------------------------


add_library(gif4 SHARED ${GIFLIB_SRCS})

target_include_directories(gif4 PRIVATE ${GIFLIB_INCLUDE_PATH})

