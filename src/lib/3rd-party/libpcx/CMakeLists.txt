# ------------------------------------
#  This CMakeLists.txt was
#  Written by Nobun, 2019
#  mugenrebirth.forumfree.it
# ------------------------------------

project(qtpcx)


add_definitions(-DQT_STATICPLUGIN -DQT_PLUGIN)


set(LIBPCX_INCLUDE_PATH
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# ---------------------------------------------------
# libraries
# ---------------------------------------------------

# ${PYTHON_LIBRARIES}
set(LIBPCX_LINK_LIBS
    Qt5::Widgets
)

# ---------------------------------------------------
# sources
# ---------------------------------------------------


set(LIBPCX_SRCS
    pcx.cpp
)
 


# -----------------------------------------
#  appling settings
# -----------------------------------------

 
include_directories(${VXAPO_INCLUDE_PATH})

set(CMAKE_AUTOMOC ON)

add_library(qtpcx STATIC ${LIBPCX_SRCS})
 
target_link_libraries(qtpcx ${LIBPCX_LINK_LIBS})

