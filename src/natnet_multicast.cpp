// 
// Natnet multicast ros node to publish the markers & rigidbodies

// based on https://github.com/whoenig/NatNetSDKCrossplatform
// 
#include "ros/ros.h"
#include "natnet_multicast.h"

float prev_pos[3] = {0.0, 0.0, 0.0};
float vel[3] = {0.0, 0.0, 0.0};
float prev_time = 0.0;


int main(int argc, char **argv)
{

    // Connect to command port to query version
    if (argc != 2)
    {
        std::cerr << "Usage: blocking_udp_echo_client <host>\n";
        return 1;
    }

    ros::init(argc, argv, "natnet_multicast_receiver");
    ros::NodeHandle nh, private_nh("~");

    /* time variables */
    double current_time = ros::Time::now().toSec();
    double previous_time = ros::Time::now().toSec();
    double dt = 0.0;

    /****************************************************/
    /******* boost asio and udp initialization **********/
    /****************************************************/
    boost::asio::io_service io_service_cmd;

    udp::socket socket_cmd(io_service_cmd, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver_cmd(io_service_cmd);
    udp::endpoint endpoint_cmd = *resolver_cmd.resolve({udp::v4(), argv[1], std::to_string(PORT_COMMAND)});

    std::vector<char> connectCmd;
    packet_client.buildConnectPacket(connectCmd);
    socket_cmd.send_to(boost::asio::buffer(connectCmd.data(), connectCmd.size()), endpoint_cmd);

    std::vector<char> reply(MAX_PACKETSIZE);
    udp::endpoint sender_endpoint;
    size_t reply_length = socket_cmd.receive_from(
        boost::asio::buffer(reply, MAX_PACKETSIZE), sender_endpoint);

    packet_client.UnpackCommand(reply.data());

    // Listen on multicast address
    boost::asio::io_service io_service;
    MulticastReceiver receiver(nh, io_service,
        boost::asio::ip::address::from_string("0.0.0.0"),
        boost::asio::ip::address::from_string(MULTICAST_ADDRESS));
    io_service.run();

    ros::shutdown();

}
