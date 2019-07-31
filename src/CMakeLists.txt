#  Nomen - a New Opensource Mugen Editor by Nobun
#
#  Copyright (C) 2011-2019  Nobun
#  http://mugenrebirth.forumfree.it
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program (GPL.txt).  If not, see <http://www.gnu.org/licenses/>.
#
#############################


cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
 
project(nomen)

# giflib4 is builded as an external project in all *NIX system.
# under windows, instead, giflib4 will be builded with a custom CMakeLists.txt
# this why the external project will be not executed on Windows
IF(NOT WIN32)
   include(ExternalProject)
   ExternalProject_Add(giflib4_ext
       SOURCE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rd-party/giflib
       BINARY_DIR   ${CMAKE_BINARY_DIR}/lib/3rd-party/giflib
       INSTALL_DIR  ${CMAKE_BINARY_DIR}/lib/3rd-party/giflib/giflib4_ext
       CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rd-party/giflib/configure --prefix ${CMAKE_BINARY_DIR}/lib/3rd-party/giflib
       BUILD_COMMAND     "make"
   )
   set(GIFLIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rd-party/giflib)
   set(GIFLIB_LIB ${CMAKE_BINARY_DIR}/lib/3rd-party/giflib/lib/.libs/libgif.a)
ELSE()
   set(GIFLIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rd-party/giflib)
   set(GIFLIB_LIB giflib4)
ENDIF()

set(NOMEN_GIFSAVE_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/NomenGifSave)
set(QTPCX_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rd-party/libpcx)




find_package(Qt5Widgets 5.2.0 REQUIRED)



add_subdirectory(lib)


