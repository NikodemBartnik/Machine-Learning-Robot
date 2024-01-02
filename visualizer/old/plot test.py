import math
import matplotlib.pyplot as plt

# Generate the points in a circular pattern
angle = 360  # Angle in degrees
num_points = 100  # Number of points
lines = []
for x in range(num_points):
    lines.append([300 * math.cos((x + 1) * angle / num_points / 180 * math.pi), 300 * math.sin((x + 1) * angle / num_points / 180 * math.pi)])

# Extract x and y coordinates
x_coords, y_coords = zip(*lines)

# Create the plot
plt.figure(figsize=(6, 6))  # Set the figure size (optional)
plt.plot(x_coords[0:10], y_coords[0:10])
plt.title('Plot of Points in a Circular Pattern')
plt.xlabel('X-coordinate')
plt.ylabel('Y-coordinate')
plt.grid(True)

# Show the plot
plt.show()
