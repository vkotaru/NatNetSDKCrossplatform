#ifndef __NATNET_MULTICAST_H__
#define __NATNET_MULTICAST_H__
#include "ros/ros.h"
#include "packet_client_helper.h"

/* msgs */
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"


#include <array>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <inttypes.h>
#include <stdio.h>

// constexpr char* MULTICAST_ADDRESS = "239.255.42.99";
// constexpr int PORT_COMMAND = 1510;
// constexpr int PORT_DATA = 1511;
// constexpr int MAX_PACKETSIZE = 100000;  // max size of packet (actual packet size is dynamic)

using boost::asio::ip::udp;
PacketClientHelper packet_client;

class MulticastReceiver {
public:
    MulticastReceiver(ros::NodeHandle &nh, boost::asio::io_service& io_service,
        const boost::asio::ip::address& listen_address,
        const boost::asio::ip::address& multicast_address)
        : nh(nh), socket_(io_service)
        , sender_endpoint_()
        , data_(20000)
    {
        // Ros 
        pub_rigid_bobies = nh.advertise<geometry_msgs::PoseArray>("/mocap/rigid_bodies", 3);
        pub_unlabeled_points = nh.advertise<geometry_msgs::PoseArray>("/mocap/unlabeled_points", 3);

        // Create the socket so that multiple may be bound to the same address.
        boost::asio::ip::udp::endpoint listen_endpoint(
            listen_address, PORT_DATA);
        socket_.open(listen_endpoint.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);

        // Join the multicast group.
        socket_.set_option(
            boost::asio::ip::multicast::join_group(multicast_address));

        do_receive();
    }


private:
    void do_receive()
    {
        socket_.async_receive_from(
            boost::asio::buffer(data_.data(), data_.size()), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t length)
            {
            if (!ec)
            {
                // std::cout.write(data_.data(), length);
                // std::cout << std::endl;

                packet_client.Unpack(data_.data(), msg_rigid_bodies, msg_unlabeled_points);

                // publishing the msgs
                pub_rigid_bobies.publish(msg_rigid_bodies);
                pub_unlabeled_points.publish(msg_unlabeled_points);

                if (ros::ok()) {
                    do_receive();
                }            
            }
            });
    }

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    std::vector<char> data_;

    // ros handle
    ros::NodeHandle nh;
    ros::Publisher pub_rigid_bobies, pub_unlabeled_points;
    geometry_msgs::PoseArray msg_rigid_bodies, msg_unlabeled_points;

};

#endif /* __NATNET_MULTICAST_H__ */