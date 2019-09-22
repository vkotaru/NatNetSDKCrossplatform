#ifndef __PACKET_CLIENT_HELPER_H__
#define __PACKET_CLIENT_HELPER_H__
/* 
Copyright © 2012 NaturalPoint Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

/*

PacketClient.cpp

Decodes NatNet packets directly.

Usage [optional]:

	PacketClient [ServerIP] [LocalIP]

	[ServerIP]			IP address of server ( defaults to local machine)
	[LocalIP]			IP address of client ( defaults to local machine)

*/

// Adopted from PacketClient.cpp (NatNetSDK 3.1)
// Windows Socket related functions removed

#include "ros/ros.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"

#include <inttypes.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <cmath>

#define MAX_NAMELENGTH              256

// NATNET message ids
#define NAT_CONNECT                 0 
#define NAT_SERVERINFO              1
#define NAT_REQUEST                 2
#define NAT_RESPONSE                3
#define NAT_REQUEST_MODELDEF        4
#define NAT_MODELDEF                5
#define NAT_REQUEST_FRAMEOFDATA     6
#define NAT_FRAMEOFDATA             7
#define NAT_MESSAGESTRING           8
#define NAT_UNRECOGNIZED_REQUEST    100
#define UNDEFINED                   999999.9999

#define MAX_PACKETSIZE				100000	// max size of packet (actual packet size is dynamic)

// This should match the multicast address listed in Motive's streaming settings.
#define MULTICAST_ADDRESS		"239.255.42.99"    

// NatNet Command channel
#define PORT_COMMAND            1510

// NatNet Data channel
#define PORT_DATA  			    1511                


// sender
typedef struct
{
    char szName[MAX_NAMELENGTH];            // sending app's name
    unsigned char Version[4];               // sending app's version [major.minor.build.revision]
    unsigned char NatNetVersion[4];         // sending app's NatNet version [major.minor.build.revision]

} sSender;

typedef struct
{
    unsigned short iMessage;                // message ID (e.g. NAT_FRAMEOFDATA)
    unsigned short nDataBytes;              // Num bytes in payload
    union
    {
        unsigned char  cData[MAX_PACKETSIZE];
        char           szData[MAX_PACKETSIZE];
        unsigned long  lData[MAX_PACKETSIZE/4];
        float          fData[MAX_PACKETSIZE/4];
        sSender        Sender;
    } Data;                                 // Payload incoming from NatNet Server

} sPacket;


class PacketClientHelper
{
private:
    /* data */
    // NatNetVersion: 240.164.53.0
    // ServerVersion: 3.0.0.0
    int NatNetVersion[4] = {240,164,53,0};
    int ServerVersion[4] = {3,0,0,0};



    /* functions */
    int strcpy_s(char* dest, size_t destsz, const char *src);
    template<typename... Args> int sprintf_s(char * buffer, size_t bufsz, const char * format, Args... args);
    bool DecodeTimecode(unsigned int inTimecode, unsigned int inTimecodeSubframe, int* hour, int* minute, int* second, int* frame, int* subframe);
    bool TimecodeStringify(unsigned int inTimecode, unsigned int inTimecodeSubframe, char *Buffer, int BufferSize);
    void DecodeMarkerID(int sourceID, int* pOutEntityID, int* pOutMemberID);

public:
    PacketClientHelper(/* args */);
    ~PacketClientHelper();

    void buildConnectPacket(std::vector<char>& buffer);
    void UnpackCommand(char* pData);
    void Unpack(char* pData, geometry_msgs::PoseArray &rmsg, geometry_msgs::PoseArray &umsg);

};









#endif /*__PACKET_CLIENT_HELPER_H__*/