//SFML UDP CLIENT CODE 
//connect - tcp - used by sf::tcpsocket to connect to listening server
//listen - tcp - used by sf::tcplistener on server to listen for connections
//accept - tcp - used by sf::tcplistener on server to accept new connections
//bind - udp - used by sf::udpsocket to bind to socket on client and server
//send - used by udp and tcp on server and client to send data
//receive - used by udp and tcp on server and client to reveive data
// with udp only reveiving sockets need to bind. if send only just call send 

#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <iostream>
#include <array>
#include <string>
#include <thread>



void send_data(sf::UdpSocket& socket, const std::string& message, const sf::IpAddress& ip, const unsigned short port) {
      if (socket.send(message.c_str(), message.size(), ip, port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send message to server\n";
        }
        std::cout << "Sent: " << message << "\n";
}


void receive_data(sf::UdpSocket& socket, std::array<char, 200>& receiving_buffer) {
  std::size_t received;
  unsigned short senderPort;
  std::optional<sf::IpAddress> optionalServerIp;

  sf::Socket::Status result = socket.receive(receiving_buffer.data(), receiving_buffer.size(), received, optionalServerIp, senderPort);
  if (result == sf::Socket::Status::Done) {
    std::string reply(receiving_buffer.data(), received);
    std::cout << "Received from server: " << reply << "\n";
    std::cout << "Size in bytes:" << received << "\n";
    //zero out reveiving buffer
    receiving_buffer.fill('\0');
    
  }
}


int main() {
  const unsigned short serverPort = 54000;
  const unsigned short clientPort = 54001; // could be sf::Socket::AnyPort
  const sf::IpAddress serverIp(127,0,0,1); // replace with your server's actual LAN IP
  sf::UdpSocket socket;
 

  //init
  if (socket.bind(clientPort) != sf::Socket::Status::Done) {
     std::cerr << "Failed to bind client UDP socket\n";
     return 1;
  }
  socket.setBlocking(false);
  std::cout << "connection complete \n";
  
  
  //main loop
  std::string message_to_send;
  std::array<char, 200> buffer{};
  
  while (true) {
    std::getline(std::cin, message_to_send);
    send_data(socket, message_to_send, serverIp, serverPort);
    receive_data(socket, buffer);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}

