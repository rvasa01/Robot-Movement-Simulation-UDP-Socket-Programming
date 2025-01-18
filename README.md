# UDP Sockets - Robot Movement Simulation

This project implements a robot movement simulation using **UDP sockets** for communication between a client and a server. The server uses **SFML** to render the robot’s movement on a 2D grid, while the client sends motion commands to the server.

## Features
### Server
- **Render Robot**:
  - Uses SFML to display a 2D grid in an 800x600 black window.
  - Represents the robot as an image in the center of the grid.
- **UDP Communication**:
  - Listens for incoming UDP packets from the client.
  - Updates the robot's position based on received direction commands.
- **Robot Behavior**:
  - Initial speed of the robot: 3 pixels/second.
  - Stops rendering the robot if the client disconnects.
  - Notifies the client when the server is about to shut down.

### Client
- **Send Commands**:
  - Sends direction commands (`w`, `a`, `s`, `d`) to move the robot up, left, down, and right.
  - Allows speed adjustment:
    - `g` to increase speed.
    - `h` to decrease speed.
  - Sends a disconnect message to the server when `q` is pressed.
- **User Interaction**:
  - Accepts key presses to control the robot's motion.

## How to Run
### Server
1. Compile and run the server code with the appropriate port number as a command-line argument.
2. Wait for client connections to start the robot rendering.
3. Close the server by closing the SFML window or receiving a disconnect signal from the client.

### Client
1. Compile and run the client code with the server's IP and port as arguments.
2. Use the following keys to control the robot:
   - `w`: Move up.
   - `s`: Move down.
   - `a`: Move left.
   - `d`: Move right.
   - `g`: Increase speed.
   - `h`: Decrease speed.
   - `q`: Disconnect and exit.
