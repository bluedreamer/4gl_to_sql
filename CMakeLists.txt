project(4gl_to_sql)

set(CMAKE_C_COMPILER  /usr/bin/clang)
set(CMAKE_CXX_COMPILER  /usr/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES config.h     database.h  field.h   index.h  main.cc   sequence.cc  table.cc  util.cc
        database.cc  field.cc    index.cc  lexer.l  parser.y  sequence.h   table.h   util.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
