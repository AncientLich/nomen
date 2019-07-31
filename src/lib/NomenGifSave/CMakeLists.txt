project(NomenGifSave)



if(NOT WIN32)
   set(NOMEN_GIFSAVE_GIFLIB_INCLUDE
       ${GIFLIB_SRC}
       ${GIFLIB_SRC}/lib
   )
endif()


set(NOMEN_GIFSAVE_INCLUDE_PATH
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${NOMEN_GIFSAVE_GIFLIB_INCLUDE}
)



# ---------------------------------------------------
# libraries
# ---------------------------------------------------

# ${PYTHON_LIBRARIES}
set(NOMEN_GIFSAVE_LINK_LIBS
    Qt5::Widgets
)


# ---------------------------------------------------
# dependencies
# ---------------------------------------------------


IF(WIN32)
    set(NOMEN_GIFSAVE_DEPS giflib4)
ELSE()
    set(NOMEN_GIFSAVE_DEPS giflib4_ext)
ENDIF()


# ---------------------------------------------------
# sources
# ---------------------------------------------------


set(NOMEN_GIFSAVE_SRCS
    NomenGifSave.cpp
)
 

# -----------------------------------------
#  appling settings
# -----------------------------------------

set(CMAKE_AUTOMOC ON)

IF(WIN32)
    message("WIN32")
    add_library(NomenGifSave ${NOMEN_GIFSAVE_SRCS})
ELSE()
    message("NON-WIN32")
    add_library(NomenGifSave STATIC ${NOMEN_GIFSAVE_SRCS})
ENDIF()

add_dependencies(NomenGifSave ${NOMEN_GIFSAVE_DEPS}) 

target_include_directories(NomenGifSave PRIVATE ${NOMEN_GIFSAVE_INCLUDE_PATH})
 
target_link_libraries(NomenGifSave ${NOMEN_GIFSAVE_LINK_LIBS})

