cmake_minimum_required(VERSION 3.25)
project(netreplay)

set(CMAKE_CXX_STANDARD 11)
set(LIBS pcap pthread)

add_executable(netreplay src/netreplay.cpp src/settings.cpp src/settings.h src/include/cxxopts/cxxopts.hpp src/plugins/pluginchain.cpp src/plugins/pluginchain.h src/plugins/input_cap.cpp src/plugins/input_cap.h src/plugins/output_stdout.cpp src/plugins/output_stdout.h src/util/util.cpp src/util/util.h src/capture/capture.cpp src/capture/capture.h)

include_directories(/usr/local/include)
target_link_libraries(netreplay ${LIBS})
