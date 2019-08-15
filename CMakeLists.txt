cmake_minimum_required(VERSION 2.8.3)
project(ovc3)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  message_generation
)
find_package(yaml-cpp REQUIRED)

catkin_python_setup()

 generate_messages(
   DEPENDENCIES
   std_msgs
   sensor_msgs
 )

catkin_package(
#  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rospy std_msgs sensor_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${YAML_CPP_INCLUDE_DIR}
)

#############
## Install ##
#############

 install(PROGRAMS
   src/oubsub_both_ovc
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

###################
#for ovc3 server/client
add_executable(set_left_camera_info_server src/left_server.cpp)
target_link_libraries(set_left_camera_info_server ${catkin_LIBRARIES} ${YAML_CPP_LIBRARIES})
add_dependencies(set_left_camera_info_server beginner_tutorials_gencpp)

add_executable(set_right_camera_info_server src/right_server.cpp)
target_link_libraries(set_right_camera_info_server ${catkin_LIBRARIES} ${YAML_CPP_LIBRARIES})
add_dependencies(set_right_camera_info_server beginner_tutorials_gencpp)

