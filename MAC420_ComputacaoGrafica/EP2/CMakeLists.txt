
# Recquires CMake 2.6 or greater.
cmake_minimum_required (VERSION 2.6)

# General definitions.
project (MAC0420_EP2 CXX C)
set (PROJECT_NAME MundoIME)
set (PROJECT_BUILDNAME MundoIME)

# Some output.
message ("=== ${PROJECT_NAME} ===")

# If the user did not define the parameteres, use default values.
if (NOT BUILD_TYPE)
    message ("-- No build type was defined, choosing default value: RELEASE")
    set (BUILD_TYPE RELEASE)
else (NOT BUILD_TYPE)
    message ("-- Build type: ${BUILD_TYPE}")
endif (NOT BUILD_TYPE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC "#define ISMAC")
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()


# CFLAGS: flags que voce queira enviar ao compilador
# LDFLAGS: flags que voce queira enviar ao ligador
set (CFLAGS "" CACHE STRING "flags enviadas ao compilador")
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    set (CFLAGS "${CFLAGS} -Wall -ansi -O3 -U_FORTIFY_SOURCE")
    if (${BUILD_TYPE} STREQUAL DEBUG)
        set (CFLAGS "${CFLAGS} -g")
    endif (${BUILD_TYPE} STREQUAL DEBUG)
endif (UNIX OR MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set (LDFLAGS "${LDFLAGS} -mmacosx-version-min=10.5")
endif ()



# Important build locations.
set (SRC_DIR src)

SET(EP_SRCS
    ./src/main.cc

    ./src/mundoime/mundoime.cc
    ./src/mundoime/loaderutil.cc
    ./src/mundoime/physicsmanager.cc
    ./src/mundoime/objects/model.cc

    ./src/engine/engine.cc
    ./src/engine/engineconfig.cc
    ./src/engine/eventhandler.cc
    ./src/engine/inputmanager.cc
    ./src/engine/light.cc
    ./src/engine/object.cc
    ./src/engine/objectcontainer.cc
    ./src/engine/timer.cc
    ./src/engine/vector3D.cc
    ./src/engine/objload/objload.cpp
    ./src/engine/texture/texture.cpp
    ./src/engine/texture/bmpLoader.c
    ./src/engine/texture/tgaLoader.c
)

include_directories ("src")


# Encontrando as bibliotecas necessarias:
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

subdirs(BulletLib)
set(PROJECT_BULLET_LIBS BulletDynamics BulletCollision  LinearMath)
include_directories(BulletLib)

find_package(GLUT REQUIRED)
if (WIN32)
  set (GLUT_FINAL_LIB ${GLUT_LIBRARY} )
else ()
  set (GLUT_FINAL_LIB ${GLUT_glut_LIBRARY} )
endif ()



add_executable (${PROJECT_BUILDNAME} ${EP_SRCS})

target_link_libraries (${PROJECT_BUILDNAME}
                            ${OPENGL_LIBRARIES}
                            ${GLUT_FINAL_LIB}
                            ${PROJECT_BULLET_LIBS} )


set_target_properties (${PROJECT_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES PROJECT_NAME "${PROJECT_NAME}")


