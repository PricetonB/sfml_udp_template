//SFML UDP SERVER CODE 
//connect - tcp - used by sf::tcpsocket to connect to listening server
//listen - tcp - used by sf::tcplistener on server to listen for connections
//accept - tcp - used by sf::tcplistener on server to accept new connections
//bind - udp - used by sf::udpsocket to bind to socket on client and server
//send - used by udp and tcp on server and client to send data
//receive - used by udp and tcp on server and client to reveive data
// with udp only reveiving sockets need to bind. if send only just call send 

#include <SFML/Network.hpp>
#include <iostream>
#include <array>
#include <thread>


int main() {
    const unsigned short serverPort = 54000;
    const sf::IpAddress serverIp(127,0,0,1); // replace with your server's actual LAN IP
    

    sf::UdpSocket socket;
        // --- Server Code ---
        if (socket.bind(serverPort) != sf::Socket::Status::Done) {
            std::cerr << "Failed to bind server UDP socket\n";
            return 1;
        }
        socket.setBlocking(false);
        std::cout << "Server listening on port " << serverPort << "...\n";

        std::array<char, 100> buffer;
        std::size_t received;
        unsigned short clientPort;
        std::optional<sf::IpAddress> clientIp;

        // Wait to receive data from the client
        while (true) {
            sf::Socket::Status result = socket.receive(buffer.data(), buffer.size(), received, clientIp, clientPort);
            if (result == sf::Socket::Status::Done) {
                std::string msg(buffer.data(), received);
                std::cout << "Received from client: " << msg << "\n";
            
                if (clientIp) {  // check if optional has a value
                    sf::IpAddress convertedClientIp = *clientIp;  // extract the actual IP address
                    std::string response = "Hello from server!";
            
                    if (socket.send(response.c_str(), response.size(), convertedClientIp, clientPort) != sf::Socket::Status::Done) {
                        std::cerr << "Failed to send response\n";
                    } else {
                        std::cout << "Sent: " << response << "\n";
                    }
                } else {
                    std::cerr << "Received data but no client IP address\n";
                }
            }


            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Waiting for client...\n";

    }

    return 0;
}





