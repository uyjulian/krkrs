include($ENV{DEVKITPRO}/switch.cmake)

# Workaround for compilation / linking test failure
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fPIE")
