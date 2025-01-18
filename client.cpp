/*
Author: Rahil Vasa
Last Date Modified: 11/14/2024
Description:
Client side of the UDP that sends commands to control the robot on the server side. 
*/


#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return -1;
    }
    // Retrieving server IP and port from the command-line arguments
    sf::IpAddress serverIp = argv[1];
    unsigned short serverPort = static_cast<unsigned short>(std::atoi(argv[2]));

    sf::UdpSocket socket;
    socket.setBlocking(false); // Enabling non-blocking mode

    sf::RenderWindow window(sf::VideoMode(200, 200), "Robot Controller");
    window.setFramerateLimit(60);

    bool isConnected = true; 

    while (window.isOpen() && isConnected) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::string disconnectMsg = "q";
                socket.send(disconnectMsg.c_str(), disconnectMsg.size() + 1, serverIp, serverPort);
                window.close();
                isConnected = false;
            }
            else if (event.type == sf::Event::KeyPressed) {
                sf::Keyboard::Key key = event.key.code;
                std::string message;
                // Updating the message based on the key pressed
                if (key == sf::Keyboard::W) {
                    message = "w"; // Moving up
                } else if (key == sf::Keyboard::S) {
                    message = "s"; // Moving down
                } else if (key == sf::Keyboard::A) {
                    message = "a"; // Moving left
                } else if (key == sf::Keyboard::D) {
                    message = "d"; // Moving right
                } else if (key == sf::Keyboard::G) {
                    message = "g"; // Increasing speed
                } else if (key == sf::Keyboard::H) {
                    message = "h"; // Decreasing speed
                } else if (key == sf::Keyboard::Q) {
                    message = "q"; // Disconnecting
                    isConnected = false;
                }

                if (!message.empty()) {
                    if (socket.send(message.c_str(), message.size() + 1, serverIp, serverPort) != sf::Socket::Done) {
                        std::cerr << "Error sending message to server." << std::endl;
                    } else {
                        std::cout << "Message sent to server: " << message << std::endl;
                    }
                    // Closing the window if 'q' was pressed
                    if (message == "q") {
                        window.close();
                    }
                }
            }
        }
        char data[100];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;
        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            data[received] = '\0';
            std::string serverMessage(data);
            if (serverMessage == "server_closing") {
                std::cout << "Server is closing. Exiting client." << std::endl;
                isConnected = false;
                window.close();
            }
        }
        window.clear();
        window.display();
    }
    return 0;
}
