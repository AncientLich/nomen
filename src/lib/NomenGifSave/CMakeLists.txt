project(NomenGifSave)



set(NOMEN_GIFSAVE_INCLUDE_PATH
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GIFLIB_SRC}
    ${GIFLIB_SRC}/lib
)



# ---------------------------------------------------
# libraries
# ---------------------------------------------------


IF(WIN32)
   set(NOMEN_GIFSAVE_LINK_LIBS
       Qt5::Widgets
       gif4
   )
ELSE()
   set(NOMEN_GIFSAVE_LINK_LIBS
       Qt5::Widgets
   )
ENDIF()


# ---------------------------------------------------
# dependencies
# ---------------------------------------------------


IF(WIN32)
    set(NOMEN_GIFSAVE_DEPS gif4)
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
#  applying settings
# -----------------------------------------

set(CMAKE_AUTOMOC ON)

IF(WIN32)
    add_library(NomenGifSave SHARED ${NOMEN_GIFSAVE_SRCS})
    set_target_properties(NomenGifSave PROPERTIES PREFIX "")
ELSE()
    add_library(NomenGifSave STATIC ${NOMEN_GIFSAVE_SRCS})
ENDIF()

add_dependencies(NomenGifSave ${NOMEN_GIFSAVE_DEPS}) 

target_include_directories(NomenGifSave PRIVATE ${NOMEN_GIFSAVE_INCLUDE_PATH})
 
target_link_libraries(NomenGifSave ${NOMEN_GIFSAVE_LINK_LIBS})

