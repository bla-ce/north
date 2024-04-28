cmake_minimum_required (VERSION 3.27)
project (north LANGUAGES CXX)

set(SOURCES src/main.cpp)

add_executable(north ${SOURCES})
target_include_directories(north PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_compile_features(north PRIVATE cxx_std_17)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(north INTERFACE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused; -pedantic; -Werrors; -Wconversion>"
)
