#cmake_policy (SET CMP0022 NEW)
#cmake_policy (SET CMP0028 NEW)

set (SOURCE_FILES "actions.cpp" "printer.cpp" "permutation_state.cpp" "compact_state.cpp" "thistlethwaite.cpp")

add_library                (rubik-shared SHARED     ${SOURCE_FILES})
target_include_directories (rubik-shared PRIVATE    ${CMAKE_SOURCE_DIR}/src/include)
set_target_properties      (rubik-shared PROPERTIES SOVERSION "0.0.1"
                                                      OUTPUT_NAME "rubik"
                                                      EXPORT_NAME "rubik")

