import pygame
import math
import serial
import numpy

pygame.init()

# Constant based on lidar resolution
LIDAR_RESOLUTION = 240
# Lidar resolution divided by 4 to simplify the visualization
VISUALIZATION_RESOLUTION = 240

distances_list = []
first_run = True

# Initialize serial communication with your Arduino
ser = serial.Serial('COM7', 230400)  # Replace 'COMx' with your Arduino's COM port and baud rate

def GetDataFromArduino():
    global distances_list, first_run
    line = ser.readline().decode().strip()  # Read a line of data from the serial port
    #data = line[:-1]  # Get rid of the trailing comma
    print(line[-1])
    data = line[:-2]
    d_list = data.split(",")
    return d_list

def GenerateLinePositions(numberOfLines):
    angle = 360 / numberOfLines
    lines = []
    for x in range(numberOfLines):
        lines.append([300 * math.cos((x + 1) * angle / 180 * math.pi), 300 * math.sin((x + 1) * angle / 180 * math.pi)])
    return lines

line_positions = GenerateLinePositions(VISUALIZATION_RESOLUTION)

# Set up the drawing window
screen = pygame.display.set_mode([800, 800])
sysfont = pygame.font.get_default_font()
font1 = pygame.font.SysFont(sysfont, 72)

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Get lidar data from Arduino
    distances = GetDataFromArduino()

    if len(distances) == LIDAR_RESOLUTION:
        # Fill the background with white
        screen.fill((250, 250, 250))

        for x in range(VISUALIZATION_RESOLUTION):
            a = int(distances[x]) / 2000
            if x in [147, 148, 149, 150, 151, 152, 206, 207, 208, 209, 210, 211, 212, 213, 214]:
                pygame.draw.circle(screen, (255, 0, 0), (line_positions[x][0] * a + 400, line_positions[x][1] * a + 400), 3)
            else:
                pygame.draw.circle(screen, (0, 0, 0), (line_positions[x][0] * a + 400, line_positions[x][1] * a + 400), 2)

        #pygame.draw.circle(screen, (252, 132, 3), (400, 400), 12)
        # Flip the display
        pygame.display.flip()
        pygame.time.wait(50)

# Close the serial connection and quit Pygame
ser.close()
pygame.quit()
