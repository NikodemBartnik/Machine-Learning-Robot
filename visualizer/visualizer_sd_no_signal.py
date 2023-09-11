import pygame, math, serial, numpy
pygame.init()

#constant based on lidar resolution
LIDAR_RESOLUTION = 240
#lidar resolution divided by 4 to simplify the visualization
VISUALIZATION_RESOLUTION = 240

distances_list = []
first_run = True

def GetDataFromArduino(line):
    global distances_list, first_run
    #[:-3] get rid of end of line sign and additional comma separator that is sent from arduino
    data = line[:-3]
    print(data)
    d_list= data.split(",")
    return d_list




def GenerateLinePositions(numberOfLines):
    angle = 360/numberOfLines
    lines = []
    for x in range(numberOfLines):
        lines.append([300 * math.cos((x+1)*angle/180 * math.pi), 300 * math.sin((x+1)*angle/180 * math.pi)])
    return lines



line_positions = GenerateLinePositions(VISUALIZATION_RESOLUTION)

# Set up the drawing window
screen = pygame.display.set_mode([800, 800])
sysfont = pygame.font.get_default_font()
font1 = pygame.font.SysFont(sysfont, 72)

file1 = open('I:/arduino lidar/visualizer/test6.txt', 'r')
Lines = file1.readlines()

for line in Lines:
    distances = GetDataFromArduino(line)
    print(len(distances))
    if(len(distances) == LIDAR_RESOLUTION):
        # Did the user click the window close button?
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            
    
        # Fill the background with white
        screen.fill((250, 250, 250))

    
        for x in range(VISUALIZATION_RESOLUTION):
            a = int(distances[x])/2000
            if x in [141, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 203, 204, 205, 206, 207]:
                pygame.draw.circle(screen, (255,0,0), (line_positions[x][0]*a+400, line_positions[x][1]*a+400), 3)
            else:
                pygame.draw.circle(screen, (0,0,0), (line_positions[x][0]*a+400, line_positions[x][1]*a+400), 2)
            
            
        pygame.draw.circle(screen, (252, 132, 3), (400, 400), 12)
        # Flip the display
        pygame.display.flip()
        pygame.time.wait(50)

pygame.quit()