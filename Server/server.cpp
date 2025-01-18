/*
Author: Rahil Vasa
Class: ECE4122 
Last Date Modified: 11/14/2024
Description:
Server side of the UDP protocol that opens a connection, listens for UDP streams of data, and displays movement of robot on the screen. 
*/


#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <algorithm> 

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }

    // Retrieving the port number from the command-line arguments
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[1]));

    sf::UdpSocket socket;
    if (socket.bind(port) != sf::Socket::Done) {
        std::cerr << "Error binding server to port " << port << std::endl;
        return -1;
    }
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    socket.setBlocking(false);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Robot Simulation Server");
    window.setFramerateLimit(60);

    // Loading the robot texture
    sf::Texture robotTexture;
    if (!robotTexture.loadFromFile("/home/hice1/rvasa7/Desktop/ECE 4122/Beginning-Cpp-Game-Programming-Second-Edition/Lab5/Lab5Server/robot.png")) {
        std::cerr << "Error loading robot image" << std::endl;
        return -1;
    }

    // Setting up the robot sprite
    sf::Sprite robotSprite;
    robotSprite.setTexture(robotTexture);
    robotSprite.setOrigin(robotTexture.getSize().x / 2.f, robotTexture.getSize().y / 2.f);  // Setting origin for rotation
    robotSprite.setScale(0.5f, 0.5f);  // Adjusting the scale as needed
    robotSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);  // Placing robot at center

    float speed = 3.0f;  // Setting initial speed to 3 pixels/second
    sf::Vector2f direction(0.f, 0.f);  // Initializing direction
    bool robotVisible = false;  // Setting robot visibility to false until a packet is received


    char data[100];
    std::size_t received;
    sf::IpAddress clientAddress;
    unsigned short clientPort;
    bool clientConnected = false;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Informing the client that the server is closing
                if (clientConnected) {
                    std::string closingMessage = "server_closing";
                    socket.send(closingMessage.c_str(), closingMessage.size() + 1, clientAddress, clientPort);
                }
                window.close();
            }
        }

        if (socket.receive(data, sizeof(data), received, clientAddress, clientPort) == sf::Socket::Done) {
            data[received] = '\0';
            std::string message(data);

            // Marking the client as connected upon first packet
            if (!clientConnected) {
                clientConnected = true;
                robotVisible = true;
                std::cout << "Client connected from " << clientAddress << ":" << clientPort << std::endl;
                robotSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);  // Resetting robot position to center
            }

            // Updating direction or speed based on the message
            if (message == "w") {
                direction = {0.f, -1.f};
                robotSprite.setRotation(0.f);  // Moving up
            } else if (message == "s") {
                direction = {0.f, 1.f};
                robotSprite.setRotation(180.f);  // Moving down
            } else if (message == "a") {
                direction = {-1.f, 0.f};
                robotSprite.setRotation(270.f);  // Moving left
            } else if (message == "d") {
                direction = {1.f, 0.f};
                robotSprite.setRotation(90.f);  // Moving right
            } else if (message == "g") {
                speed += 1.0f;  // Increasing speed
                std::cout << "Speed increased to " << speed << " pixels/second" << std::endl;
            } else if (message == "h") {
                speed = std::max(1.0f, speed - 1.0f);  // Decreasing speed but ensuring a minimum of 1
                std::cout << "Speed decreased to " << speed << " pixels/second" << std::endl;
            } else if (message == "q") {
                // Disconnecting the client
                std::cout << "Client disconnected." << std::endl;
                clientConnected = false;
                robotVisible = false;
                direction = {0.f, 0.f}; 
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (robotVisible) {
            robotSprite.move(direction * speed * deltaTime);
        }

        window.clear(sf::Color::Black);
        if (robotVisible) {
            window.draw(robotSprite);
        }
        window.display();
    }

    if (clientConnected) {
        std::string closingMessage = "server_closing";
        socket.send(closingMessage.c_str(), closingMessage.size() + 1, clientAddress, clientPort);
    }

    // Unbinding the socket before exiting
    socket.unbind();
    return 0;
}
