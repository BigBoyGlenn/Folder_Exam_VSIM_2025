import laspy
import numpy as np

# Input/output files
input_file = '33-1-533-358-11.laz'  
output_file = 'lasdata.txt'             

# Read the LAZ file
las = laspy.read(input_file, laz_backend=laspy.LazBackend.Laszip)

# Convert to numpy arrays
x = np.array(las.x)
y = np.array(las.y)
z = np.array(las.z)

# Compute the centroid
centroid_x = x.mean()
centroid_y = y.mean()
centroid_z = z.mean()

# Centralize points to origin
centralized_x = x - centroid_x
centralized_y = y - centroid_y
centralized_z = z - centroid_z

# Flip so Y is up instead of Z
flipped_x = centralized_x
flipped_y = centralized_z   
flipped_z = centralized_y   

num_points = len(las.points)

# Write to ASCII file
with open(output_file, 'w') as f:
    f.write(f"{num_points}\n")
    for xi, yi, zi in zip(flipped_x, flipped_y, flipped_z):
        f.write(f"{xi} {yi} {zi}\n")

print(f"LAZ file '{input_file}' has been centralized, Y-up flipped, and exported to '{output_file}'.")
