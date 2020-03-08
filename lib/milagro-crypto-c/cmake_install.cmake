# Install script for directory: /home/jrml/devel/lua-multiparty/lib/milagro-crypto-c

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/lib/libamcl_core.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/amcl.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/arch.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/version.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/utils.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/randapi.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/config_test.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/pbc_support.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/ecdh_support.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/lib/libamcl_paillier.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/amcl" TYPE FILE FILES
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/config_big_1024_58.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/config_ff_2048.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/big_1024_58.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/ff_2048.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/config_big_512_60.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/config_ff_4096.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/big_512_60.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/ff_4096.h"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/include/paillier.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/amcl.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake"
         "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/CMakeFiles/Export/lib/cmake/amcl/AMCLTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl/AMCLTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/CMakeFiles/Export/lib/cmake/amcl/AMCLTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/CMakeFiles/Export/lib/cmake/amcl/AMCLTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amcl" TYPE FILE FILES
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/AMCLConfig.cmake"
    "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/AMCLConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/jrml/devel/lua-multiparty/lib/milagro-crypto-c/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
