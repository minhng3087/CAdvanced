project(code C)

set(CMAKE_C_STANDARD 99)

add_executable(K63 K63.c)
add_executable(2015 2015.c)
add_executable(2016 2016.c)
add_executable(2018 2018.c)
add_executable(K62 K62.c)

target_link_libraries(2015 fdr)
target_link_libraries(2016 fdr)
target_link_libraries(2018 fdr)
target_link_libraries(K62 fdr)
target_link_libraries(K63 fdr)

