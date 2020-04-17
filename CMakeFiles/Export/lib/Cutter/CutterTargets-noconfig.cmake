#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Cutter::Cutter" for configuration ""
set_property(TARGET Cutter::Cutter APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Cutter::Cutter PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/Cutter"
  )

list(APPEND _IMPORT_CHECK_TARGETS Cutter::Cutter )
list(APPEND _IMPORT_CHECK_FILES_FOR_Cutter::Cutter "${_IMPORT_PREFIX}/bin/Cutter" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
