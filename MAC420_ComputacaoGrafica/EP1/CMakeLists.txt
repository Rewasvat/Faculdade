
# Recquires CMake 2.6 or greater.
cmake_minimum_required (VERSION 2.6)

# General definitions.
project (MAC0420_EP1 CXX C)
set (PROJECT_NAME EP1)
set (PROJECT_RELEASENAME ep1)
set (PROJECT_DEBUGNAME ep1_d)

# Some output.
message ("=== ${PROJECT_NAME} ===")

# If the user did not define the parameteres, use default values.
if (NOT BUILD_TYPE)
    message ("-- No build type was defined, choosing default value: RELEASE")
    set (BUILD_TYPE RELEASE)
else (NOT BUILD_TYPE)
    message ("-- Build type: ${BUILD_TYPE}")
endif (NOT BUILD_TYPE)

if (${BUILD_TYPE} STREQUAL RELEASE)
    set (PROJECT_BUILDNAME ${PROJECT_RELEASENAME})
elseif (${BUILD_TYPE} STREQUAL DEBUG)
    set (PROJECT_BUILDNAME ${PROJECT_DEBUGNAME})
	set (PROJECT_NAME "${PROJECT_NAME} (DEBUG)")
else ()
    message (FATAL_ERROR ">>> Unknown build type ${BUILD_TYPE}.")
endif ()


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC "#define ISMAC")
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()

# Important build locations.
set (SRC_DIR src)
set (BIN_DIR bin)


SET(EP_SRCS
    ./src/main.cc

    ./src/simulation/objects/camera.cc
    ./src/simulation/objects/cylinder.cc
    ./src/simulation/objects/sphere.cc
    ./src/simulation/objects/fieldobject.cc
    ./src/simulation/simulation.cc
    ./src/simulation/vectorfield.cc
    ./src/simulation/loaderutil.cc

    ./src/engine/engine.cc
    ./src/engine/engineconfig.cc
    ./src/engine/eventhandler.cc
    ./src/engine/inputmanager.cc
    ./src/engine/object.cc
    ./src/engine/objectcontainer.cc
    ./src/engine/timer.cc
    ./src/engine/vector3D.cc
)

# #define's que voce queira incluir na compilacao
# separe por espacos
set (DEFS "" CACHE STRING "#define's que voce queira incluir na compilacao; separe por espacos")
if (WIN32)
    set (DEFS "${DEFS} WIN32")
endif (WIN32)

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

include_directories ("src")

# Encontrando as bibliotecas necessarias:
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
#include_directories(${GLUT_INCLUDE_DIR})
if (WIN32)
  set (GLUT_FINAL_LIB ${GLUT_LIBRARY} )
else ()
  set (GLUT_FINAL_LIB ${GLUT_glut_LIBRARY} )
endif ()

#if (NOT GLUT_FOUND)
#  message (FATAL_ERROR ">>> COULD NOT FIND GLUT.")
#endif (NOT GLUT_FOUND)


if (${BUILD_TYPE} STREQUAL DEBUG)
  set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -DDEBUG")
endif (${BUILD_TYPE} STREQUAL DEBUG)

# pre-processor: #define's
if (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} /D${d}")
  endforeach (d)
  
  set (CFLAGS_EXTRA "${CFLAGS_EXTRA} /D_CRT_SECURE_NO_WARNINGS /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast")
  
else (MSVC)
  foreach (d ${DEFS})
    set (CFLAGS_EXTRA "${CFLAGS_EXTRA} -D${d}")
  endforeach (d)
endif  (MSVC)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (EXTRA_LIBRARIES m ${EXTRA_LIBRARIES})
endif (CMAKE_COMPILER_IS_GNUCXX)
if (MINGW)
	set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
	set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES})
endif (WIN32)


add_executable (${PROJECT_BUILDNAME} ${EP_SRCS})

set_target_properties (${PROJECT_BUILDNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

target_link_libraries (${PROJECT_BUILDNAME}
                            ${EXTRA_LIBRARIES}
                            ${OPENGL_LIBRARIES}
                            ${GLUT_FINAL_LIB} )


set_target_properties (${PROJECT_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS} ${CFLAGS_EXTRA}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES PROJECT_NAME "${PROJECT_NAME}")


