
include_directories( $ENV{QTDIR}/include
                     $ENV{QTDIR}/include/QtOpenGL
                     ${Boost_INCLUDE_DIR}
                   )

file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)

add_library(ui STATIC ${HEADERS} ${SOURCES})
