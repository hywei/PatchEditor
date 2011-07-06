file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)

add_library(patch STATIC ${HEADERS} ${SOURCES})