# Import necessary modules
import hyperionlibrary_python_wrapper as hypl
import numpy as np
import matplotlib.pyplot as plt

def annual_efficiency_matrix(latitude, io, beta, transmittance_model, tower_height, receiver_radius, xmin, xmax, ymin, ymax, nrows, ncolumns):

    # Initialize location and atmosphere based on the given latitude
    location = hypl.Location(latitude)
    atmosphere = hypl.Atmosphere(io, beta, transmittance_model)
       
    # Set up the environment
    environment = hypl.Environment(location, atmosphere)
 
    # Define receiver characteristics
    receiver_position = hypl.vec3d(0, 0, tower_height)
    receiver = hypl.Receiver(receiver_position, receiver_radius)

    # Create an efficiency map
    boundaries = hypl.Boundaries(xmin, xmax, ymin, ymax)
    
    receivers = [receiver]  # List of receivers

    efficiency_map = hypl.IdealEfficiencyMap(environment, boundaries, receivers, nrows, ncolumns)
    
    # Evaluate annual efficiencies
    ideal_efficiency_type = hypl.IdealEfficiencyType.AllFactors
    delta_t = 250.0
    efficiency_map.EvaluateAnnualEfficiencies(ideal_efficiency_type, delta_t)

    # Get heliostats and compute efficiencies
    heliostats = efficiency_map.heliostats()
    efficiencies = np.zeros((nrows, ncolumns))
    
    for i, heliostat in enumerate(heliostats):
        row, col = divmod(i, ncolumns)
        efficiencies[row, col] = heliostat.m_annual_ideal_efficiency  # Assuming this attribute holds the computed efficiency

    # Save the efficiency matrix to a text file
    with open("annual_efficiency_matrix.txt", "w") as f:
        for row in efficiencies:
            f.write(" ".join(f"{value:.6f}" for value in row) + "\n")
    
    # Create a grid of x and y coordinates
    x = np.linspace(xmin, xmax, ncolumns)
    y = np.linspace(ymin, ymax, nrows)
    X, Y = np.meshgrid(x, y)

    # Plot the efficiency matrix using a contour plot
    plt.figure(figsize=(10, 8))
    contour = plt.contourf(X, Y, efficiencies, cmap="viridis")
    plt.colorbar(contour, label="Annual Efficiency")
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.title(f"Annual Efficiency Contour Plot (Latitude: {np.degrees(latitude)}°, Tower Height: {tower_height} m, Receiver Radius: {receiver_radius} m)")
    plt.show()


# Example usage
annual_efficiency_matrix(latitude=np.radians(37.2), io=1027.0, beta=0.3, transmittance_model=hypl.TransmittanceModel.SW, tower_height=100, receiver_radius=5, xmin=-1000, xmax=1000, ymin=-600.0, ymax=1400.0, nrows=400, ncolumns=400)
