// 
// Natnet multicast ros node to publish the markers & rigidbodies

// based on https://github.com/whoenig/NatNetSDKCrossplatform
// 

#include "natnet_multicast.h"
#include "natnet_sdk_crossplatform/PointArray.h"

class receiver
{
public:
  receiver(boost::asio::io_service& io_service,
      const boost::asio::ip::address& listen_address,
      const boost::asio::ip::address& multicast_address)
    : socket_(io_service)
    , sender_endpoint_()
    , data_(20000)
  {
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
            Unpack(data_.data());

            do_receive();
          }
        });
  }

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  std::vector<char> data_;
};

natnet_sdk_crossplatform::PointArray point_array_msg;

int main(int argc, char **argv)
{

    // Connect to command port to query version
    if (argc != 2)
    {
        std::cerr << "Usage: blocking_udp_echo_client <host>\n";
        return 1;
    }

    ros::init(argc, argv, "natnet_multicast_node");
    ros::NodeHandle nh, private_nh("~");

    /* time variables */
    double current_time = ros::Time::now().toSec();
    double previous_time = ros::Time::now().toSec();
    double dt = 0.0;

    while(ros::ok()) {
        /* time update */
        current_time = ros::Time::now().toSec();
        dt = current_time-previous_time;
        ROS_INFO("FREQ: %f", (1/dt));
        previous_time = current_time;

        try
        {
            boost::asio::io_service io_service_cmd;

            udp::socket socket_cmd(io_service_cmd, udp::endpoint(udp::v4(), 0));

            udp::resolver resolver_cmd(io_service_cmd);
            udp::endpoint endpoint_cmd = *resolver_cmd.resolve({udp::v4(), argv[1], std::to_string(PORT_COMMAND)});

            std::vector<char> connectCmd;
            buildConnectPacket(connectCmd);
            socket_cmd.send_to(boost::asio::buffer(connectCmd.data(), connectCmd.size()), endpoint_cmd);

            std::vector<char> reply(MAX_PACKETSIZE);
            udp::endpoint sender_endpoint;
            size_t reply_length = socket_cmd.receive_from(
                boost::asio::buffer(reply, MAX_PACKETSIZE), sender_endpoint);

            UnpackCommand(reply.data());

            // Listen on multicast address
            boost::asio::io_service io_service;
            receiver r(io_service,
                boost::asio::ip::address::from_string("0.0.0.0"),
                boost::asio::ip::address::from_string(MULTICAST_ADDRESS));
            io_service.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }
    ros::shutdown();

}
