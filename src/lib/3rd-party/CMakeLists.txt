add_subdirectory(libpcx)


# giflib is an external project on *NIX systems (See "root" CMakeLists.txt). 
# giflib will be compiled as an internal cmake target only under windows
IF(WIN32)
   add_subdirectory(giflib)
ENDIF()
