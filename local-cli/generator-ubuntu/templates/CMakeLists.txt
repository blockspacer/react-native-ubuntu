
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 2.8.11)

set(APP_NAME <%= name %>)
set(REACT_BUILD_STATIC_LIB ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native-ubuntu/ReactUbuntu/runtime/src ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native-ubuntu/ReactUbuntu/application/src ${CMAKE_CURRENT_BINARY_DIR}/bin)

configure_file(
  run-app.sh.in
  ${CMAKE_CURRENT_SOURCE_DIR}/run-app.sh
  @ONLY
)

