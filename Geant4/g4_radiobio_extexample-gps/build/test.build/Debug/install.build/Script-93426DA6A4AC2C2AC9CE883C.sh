#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi

