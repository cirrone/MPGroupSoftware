#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  make -f /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  make -f /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  make -f /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build
  make -f /Users/cirrone/Dropbox/script_e_software/MPGroupSoftware/Geant4/g4_radiobio_extexample-gps/build/CMakeScripts/ReRunCMake.make
fi
