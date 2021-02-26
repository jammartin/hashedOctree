#!/usr/bin/env python3

import csv
import numpy as np

# Format of particle file: key, i_x, i_y, i_z
def read_particle_csv(file):
    particles=[]
    with open(file) as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for i, row in enumerate(reader):
            if i > 0: # skipping header
                particles.append(np.array([row[1], row[2], row[3]], dtype=np.int64))
    return np.array(particles)

if __name__ == "__main__":
    # reading in default csv-file
    particles = read_particle_csv("hot_particles.csv") # (N, 3) dimensional numpy array: [[i_x, i_y, i_z], ... ]
    #TODO: do something with the data