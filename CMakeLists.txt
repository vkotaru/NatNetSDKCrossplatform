cmake_minimum_required(VERSION 3.5)
project(natnet_sdk_crossplatform)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs std_msgs 
  roscpp  rospy
  message_generation
)

find_package(Boost 1.65 REQUIRED COMPONENTS system thread)
# Enable C++14 and warnings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


## Generate messages in the 'msg' folder
add_message_files(
  FILES
   PointArray.msg
   PointVelocityStamped.msg
)
generate_messages(
  DEPENDENCIES
  geometry_msgs
  std_msgs
)


catkin_package(
 INCLUDE_DIRS include
 LIBRARIES NatNetSDKCrossplatform
 CATKIN_DEPENDS geometry_msgs roscpp rospy std_mgs 
#  DEPENDS system_lib
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

# Executables

## natnettest
add_executable(natnettest
  src/main.cpp
)
target_link_libraries(natnettest
  ${Boost_LIBRARIES}
)


# ## natnettest
# add_executable(mcr
#   src/multicast_receiver.cpp
#   src/PacketClientHelper.cpp
# )
# target_link_libraries(mcr
#   ${Boost_LIBRARIES}
# )
# set_property(TARGET mcr PROPERTY C_STANDARD 11)


add_executable(natnet_multicast_node 
    src/nn_multicast_node.cpp
    # src/PacketClientHelper.cpp
)
add_dependencies(natnet_multicast_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(natnet_multicast_node
  ${catkin_LIBRARIES}
)
set_property(TARGET natnet_multicast_node PROPERTY C_STANDARD 11)
