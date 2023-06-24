cmake_minimum_required(VERSION 3.25)
project(netreplay)

set(CMAKE_CXX_STANDARD 11)
set(LIBS pcap pthread)

add_executable(netreplay src/netreplay.cpp src/settings.cpp src/settings.h src/include/cxxopts/cxxopts.hpp src/plugin/plugin_chain.cpp src/plugin/plugin_chain.h src/plugin/input_cap.cpp src/plugin/input_cap.h src/plugin/output_stdout.cpp src/plugin/output_stdout.h src/util/util.hpp src/capture/capture.cpp src/capture/capture.h src/message/tcp/parser.cpp src/message/tcp/parser.h src/message/tcp/message.cpp src/message/tcp/message.h src/message/tcp/packet.cpp src/message/tcp/packet.h src/emitter/emitter.cpp src/emitter/emitter.h src/util/shared_resource.hpp)

include_directories(/usr/local/include)
target_link_libraries(netreplay ${LIBS})
