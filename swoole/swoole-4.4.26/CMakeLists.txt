language: php 

php:
  - 5.3
  - 5.4
  - 5.5

notifications:
  email: false
   
env:
  - REPORT_EXIT_STATUS=1 NO_INTERACTION=1

#Compile
before_script:
    - ./travis/compile.sh

script:
    - exit 0


i
/config.guess
/config.h
/config.h.in
/config.log
/config.nice
/config.status
/config.sub
/configure
/configure.in
/Makefile.fragments
/Makefile.global
/Makefile.objects
/install-sh
/libtool
/ltmain.sh
/missing
/mkinstalldirs
/swoole.la
/acinclude.m4
/aclocal.m4
/run-tests.php
/autom4te.cache
/build
/examples/async/data.txt
/examples/.idea
/examples/recv_file.jpg
/modules
FORE ./include ./)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#libary
add_library(swoole_shared SHARED ${SRC_LIST})
add_library(swoole_static STATIC ${SRC_LIST})

set_target_properties(swoole_shared PROPERTIES OUTPUT_NAME "swoole" VERSION ${SWOOLE_VERSION})
set_target_properties(swoole_static PROPERTIES OUTPUT_NAME "swoole" VERSION ${SWOOLE_VERSION})

target_link_libraries(swoole_shared pthread rt dl)
target_link_libraries(swoole_static pthread rt dl)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#test_server
set(TEST_SRC_LIST examples/test_server.c)
add_executable(test_server ${TEST_SRC_LIST};${SRC_LIST})
target_link_libraries(test_server pthread rt dl ssl crypt crypto)

#unittest
file(GLOB_RECURSE UNITTEST_SRC_LIST FOLLOW_SYMLINKS tests/*.c)
add_executable(unittest ${UNITTEST_SRC_LIST};${SRC_LIST})
target_link_libraries(unittest pthread rt dl)

#add_dependencies(test_server swoole_static swoole_shared)
#TARGET_LINK_LIBRARIES(test_server swoole)

#install
INSTALL(CODE "MESSAGE(\"Are you run command using root user?\")")
INSTALL(TARGETS swoole_shared swoole_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
INSTALL(FILES ${HEAD_FILES} DESTINATION include/swoole)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/swoole_config.h DESTINATION include/swoole)
