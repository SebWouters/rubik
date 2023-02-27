add_executable(state1 state1.cpp)
add_executable(state2 state2.cpp)
add_executable(key1 key1.cpp)
add_executable(key2 key2.cpp)
add_executable(key3 key3.cpp)
add_executable(key4 key4.cpp)
add_executable(solve solve.cpp)

target_include_directories(state1 PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(state2 PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(key1   PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(key2   PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(key3   PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(key4   PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_include_directories(solve  PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

target_link_libraries(state1 rubik-shared)
target_link_libraries(state2 rubik-shared)
target_link_libraries(key1   rubik-shared)
target_link_libraries(key2   rubik-shared)
target_link_libraries(key3   rubik-shared)
target_link_libraries(key4   rubik-shared)
target_link_libraries(solve  rubik-shared)

add_test(NAME state1 COMMAND state1) # state correct
add_test(NAME state2 COMMAND state2) # state performance
add_test(NAME key1   COMMAND key1) # key G0 to G1
add_test(NAME key2   COMMAND key2) # key G1 to G2
add_test(NAME key3   COMMAND key3) # key G2 to G3
add_test(NAME key4   COMMAND key4) # key G3 to G4
add_test(NAME solve  COMMAND solve) # thistlethwaite in action

