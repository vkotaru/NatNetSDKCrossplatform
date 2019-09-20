#ifndef __NATNET_MULTICAST_H__
#define __NATNET_MULTICAST_H__

#include "ros/ros.h"


#include <array>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <inttypes.h>
#include <stdio.h>

constexpr char* MULTICAST_ADDRESS = "239.255.42.99";
constexpr int PORT_COMMAND = 1510;
constexpr int PORT_DATA = 1511;
constexpr int MAX_PACKETSIZE = 100000;  // max size of packet (actual packet size is dynamic)

void Unpack(char* pData);
void buildConnectPacket(std::vector<char>& buffer);
void UnpackCommand(char* pData);

using boost::asio::ip::udp;

#endif /* __NATNET_MULTICAST_H__ */