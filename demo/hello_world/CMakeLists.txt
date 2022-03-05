# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/application/components/custom_console
 						 $ENV{IDF_PATH}/application/components/esp_hw_timer
 						 $ENV{IDF_PATH}/application/components/led
 						 $ENV{IDF_PATH}/application/components/common_event
 						 $ENV{IDF_PATH}/application/components/esp_modem)

project(hello_world)
