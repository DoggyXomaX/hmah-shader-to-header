cmake_minimum_required(VERSION 3.14)
project(hmah_shader_to_header)

set(SOURCES
  src/main.c
  src/input_args.c
  src/stream.c
  src/string_builder.c
)

set(ROOT_DIR "${CMAKE_BINARY_DIR}/..")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/bin")

add_executable(hsth ${SOURCES})