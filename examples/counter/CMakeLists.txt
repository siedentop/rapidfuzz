add_executable(counter main.cpp)

target_compile_options(counter PRIVATE -fsanitize=fuzzer)
target_link_libraries(counter
    PRIVATE -fsanitize=fuzzer
)

target_link_libraries(counter PUBLIC rapidcheck)
#target_include_directories(counter PRIVATE ../../ext/catch/include)
