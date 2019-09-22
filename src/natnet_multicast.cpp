// 
// Natnet multicast ros node to publish the markers & rigidbodies

// based on https://github.com/whoenig/NatNetSDKCrossplatform
// 

// #include "ros/ros.h"
#include "ros/ros.h"
#include "packet_client_helper.h"

/* msgs */
#include "natnet_sdk_crossplatform/PointArray.h"
#include "natnet_sdk_crossplatform/PointVelocityStamped.h"
#include "geometry_msgs/Point.h"


float prev_pos[3] = {0.0, 0.0, 0.0};
float vel[3] = {0.0, 0.0, 0.0};
float prev_time = 0.0;

natnet_sdk_crossplatform::PointArray _msg;
natnet_sdk_crossplatform::PointVelocityStamped pos_vel_msg;
ros::Publisher chatter_pub;


int main(int argc, char **argv)
{

    // Connect to command port to query version
    if (argc != 2)
    {
        std::cerr << "Usage: blocking_udp_echo_client <host>\n";
        return 1;
    }

    // ros::init(argc, argv, "natnet_multicast_node");
    // ros::NodeHandle nh, private_nh("~");
    // chatter_pub = nh.advertise<natnet_sdk_crossplatform::PointArray>("/mocap/points", 3);

    // /* time variables */
    // double current_time = ros::Time::now().toSec();
    // double previous_time = ros::Time::now().toSec();
    // double dt = 0.0;

    // // while(ros::ok()) 
    // {
    //     /* time update */
    //     prev_time = ros::Time::now().toSec();
    //     // dt = current_time-previous_time;
    //     // ROS_INFO("FREQ: %f", (1/dt));
    //     // previous_time = current_time;

    //     try
    //     {
    //         boost::asio::io_service io_service_cmd;

    //         udp::socket socket_cmd(io_service_cmd, udp::endpoint(udp::v4(), 0));

    //         udp::resolver resolver_cmd(io_service_cmd);
    //         udp::endpoint endpoint_cmd = *resolver_cmd.resolve({udp::v4(), argv[1], std::to_string(PORT_COMMAND)});

    //         std::vector<char> connectCmd;
    //         buildConnectPacket(connectCmd);
    //         socket_cmd.send_to(boost::asio::buffer(connectCmd.data(), connectCmd.size()), endpoint_cmd);

    //         std::vector<char> reply(MAX_PACKETSIZE);
    //         udp::endpoint sender_endpoint;
    //         size_t reply_length = socket_cmd.receive_from(
    //             boost::asio::buffer(reply, MAX_PACKETSIZE), sender_endpoint);

    //         UnpackCommand(reply.data());

    //         // Listen on multicast address
    //         boost::asio::io_service io_service;
    //         receiver r(io_service,
    //             boost::asio::ip::address::from_string("0.0.0.0"),
    //             boost::asio::ip::address::from_string(MULTICAST_ADDRESS));
    //         io_service.run();
    //     }
    //     catch (std::exception& e)
    //     {
    //         std::cerr << "Exception: " << e.what() << "\n";
    //     }
    // }
    // ros::shutdown();

}
