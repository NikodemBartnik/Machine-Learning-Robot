import socket

# Server UDP address and port (choose a port to listen on)
server_ip = "0.0.0.0"  # Listen on all available network interfaces
server_port = 2390  # Replace with the port you want to listen on

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server address and port
udp_socket.bind((server_ip, server_port))

print("Listening for UDP packets from Arduino...")

while True:
    data, addr = udp_socket.recvfrom(256)  # Receive data from Arduino
    print("Received data from {}: {}".format(addr, data.decode('utf-8')))
