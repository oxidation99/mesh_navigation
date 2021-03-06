cmake_minimum_required(VERSION 2.8.3)
project(mesh_client)

set(CMAKE_CXX_STANDARD 14)
find_package(CURL REQUIRED)

find_package(catkin REQUIRED COMPONENTS roscpp)
find_package(CURL REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(LVR2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES mesh_client
  DEPENDS CURL EIGEN3 LVR2 JSONCPP
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${LVR_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ${CURL_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/mesh_client.cpp
)

link_libraries(${PROJECT_NAME}
  ${LVR2_LIBRARIES}
  ${JSONCPP_LIBRARIES}
  ${CURL_LIBRARIES}
)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

