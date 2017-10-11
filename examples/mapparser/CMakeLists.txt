add_executable(mapparser main.cpp MapParser.cpp)

target_compile_options(mapparser PRIVATE -fsanitize=fuzzer,address)
target_link_libraries(mapparser -fsanitize=fuzzer,address)

target_link_libraries(mapparser rapidcheck)
