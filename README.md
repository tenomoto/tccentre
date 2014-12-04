# tccentre

A simple program to track the minimum location in longitude and latitude.
The sub-grid position is found by quadratic interpolation.
Note that the circular boundary condition is not implemented.
Use longitudes from -180 to 180 for hurricane applications.

## Requirements

* [libeno](https://github.com/tenomoto/libeno)

## How to use

### Preparation

1. Prepare two text files of longitudes and latitues.
2. Prepare a data file (e.g. slp) in 4-byte real flat binary.

### Execution

Follow the interactive messages when tccentre is executed. 

1. Enter horizontal resolution: nx ny
2. Enter number of records: nr
3. Enter the half width for search in degrees.
4. Enter the path to the file of longitudes.
5. Enter the path to the file of latitudes.
6. Enter the binary data file.
7. Enter the estimated initial position: lon lat
