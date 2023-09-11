import pygame, math, serial, numpy
pygame.init()

#constant based on lidar resolution
LIDAR_RESOLUTION = 240
#lidar resolution divided by 4 to simplify the visualization
VISUALIZATION_RESOLUTION = 240

distances_list = []
first_run = True

def GetDataFromArduino():
    global distances_list, first_run
    #[:-3] get rid of end of line sign and additional comma separator that is sent from arduino
    data = arduino.readline()[:-3].decode("utf-8") 
    d_list= data.split(",")
    if len(d_list) == LIDAR_RESOLUTION*2:
        if first_run:
            distances_list = d_list
            first_run = False
        else:
            distances_list = filterDistancesList(d_list)
    return distances_list

def filterDistancesList(d_list):
    filtered_list =[]
    for i in range(int(len(d_list)/2)):
        if int(d_list[i*2+1]) < 50:
            filtered_list.append(distances_list[i*2])
        elif int(d_list[i*2]) > 7500:
            filtered_list.append(distances_list[i*2])
        else:
            filtered_list.append(d_list[i*2])
    return filtered_list


def GenerateLinePositions(numberOfLines):
    angle = 360/numberOfLines
    lines = []
    for x in range(numberOfLines):
        lines.append([300 * math.cos((x+1)*angle/180 * math.pi), 300 * math.sin((x+1)*angle/180 * math.pi)])
    return lines




arduino = serial.Serial('COM4', 230400, timeout=.1)

line_positions = GenerateLinePositions(VISUALIZATION_RESOLUTION)

# Set up the drawing window
screen = pygame.display.set_mode([800, 800])
sysfont = pygame.font.get_default_font()
font1 = pygame.font.SysFont(sysfont, 72)

while True:
    distances = GetDataFromArduino()
    print(len(distances))
    if(len(distances) == LIDAR_RESOLUTION):
        # Did the user click the window close button?
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            
    
        # Fill the background with white
        screen.fill((250, 250, 250))

    
        for x in range(VISUALIZATION_RESOLUTION):
            a = int(distances[x])/4000
            pygame.draw.circle(screen, (50, 50, 150), (line_positions[x][0]*a+400, line_positions[x][1]*a+400), 3)
            
            
        #pygame.draw.circle(screen, (120, 40, 40), (400, 400), 60)
        # Flip the display
        pygame.display.flip()

arduino.close()
pygame.quit()