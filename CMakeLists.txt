cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Packets")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -Wall -Wextra")
    
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")
    
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
    
  set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
endif()

add_library (packetslib "packet.cpp" "packet.h" "socket.h" "socket.cpp" "configuration.h" "address.h" "address.cpp")

set_target_properties(packetslib PROPERTIES
  DEBUG_POSTFIX "_d"
  RELEASE_POSTFIX ""
  RELWITHDEBINFO_POSTFIX "_rd"
  MINSIZEREL_POSTFIX "_min"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET packetslib PROPERTY CXX_STANDARD 20)
endif()

install(TARGETS packetslib
  CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel
  DESTINATION lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES Packet.h DESTINATION include)