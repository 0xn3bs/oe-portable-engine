#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "Packets.h"

namespace OE
{
	namespace Networking
	{
		class NetworkManager
		{
		private:
			static boost::asio::io_service _io_Service;
			static boost::asio::ip::udp::socket _udp_Socket;

		public:
			static void Update()
			{
				//	std::cout << "Updating..." << std::endl;
			}

			static void Listen(int port)
			{
				boost::asio::ip::udp::socket _udp_Socket = boost::asio::ip::udp::socket(_io_Service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
			}

			static void Connect(char* ip, char* port)
			{
				boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "127.0.0.1", "4847");
				boost::asio::ip::udp::resolver _udp_Resolver(_io_Service);
				boost::asio::ip::udp::endpoint receiver_endpoint = *_udp_Resolver.resolve(query);
				boost::asio::ip::udp::socket _udp_Socket = boost::asio::ip::udp::socket(_io_Service);
			}

			NetworkManager()
			{
				//boost::thread thrd(&Update);
				//thrd.join();
				/*
				try
				{
					using namespace boost::asio::ip;

					udp::resolver resolver(_io_Service);
					udp::resolver::query query(udp::v4(), "127.0.0.1", "4847");

					udp::endpoint receiver_endpoint = *resolver.resolve(query);

					udp::socket socket(_io_Service);
					socket.open(udp::v4());

					boost::array<char, 1> send_buf  = { 0 };
					socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

					boost::array<char, 128> recv_buf;
					udp::endpoint sender_endpoint;
					size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

					std::cout.write(recv_buf.data(), len);
				}
				catch (std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
				*/
			}

			~NetworkManager()
			{
			}
		};
	}
}
