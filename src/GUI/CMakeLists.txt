file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)

LINK_DIRECTORIES(/usr/local/lib/OpenMesh)

find_package(OpenGL)
find_package(OpenSceneGraph)
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)

include(${QT_USE_FILE})

include_directories( $ENV{QTDIR}/include
                     $ENV{QTDIR}/include/QtCore
                     $ENV{QTDIR}/include/QtGui
                     $ENV{QTDIR}/include/QtOpenGl
                     ${BOOST_INCLUDE_DIR}
                   )

                 
QT4_WRAP_CPP(MOCFILES ${HEADERS})
                 
add_executable(GUI ${HEADERS} ${SOURCES} ${MOCFILES})

SET(OSG_COMMON_LIBRARIES
  osg osgDB osgUtil osgGA osgViewer osgText
)
SET(QT_LIBRARIES QtCore QtGui QtOpenGL)
SET(OPENMESH_LIBRARIES OpenMeshCore OpenMeshTools)

target_link_libraries(
  GUI
  ${OSG_COMMON_LIBRARIES}
  ${QT_LIBRARIES}
  ${OPENMESH_LIBRARIES}
  patch
  )



