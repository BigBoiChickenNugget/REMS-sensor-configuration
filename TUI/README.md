### REMS Sensor Configuration Program
This program is meant to add sensors, edit existing sensors, and view overall sensor data. At the moment, the way this program works is by editing the actual
Arduino code. The supporting example.ino file is the one that's being edited by the program, but eventually, the goal is to use this in an actual production
environment with it actually editing physical files on working Arduinos. 

In regards to current development, the program can successuflly add sensor pins to the Arduino code, but there is a bug where it cannot add the sensor to
the array or to the actual webpage. This is a work in progress and will be fixed soon. The eventual goal is also to migrate from a TUI environment to a
flask web server that can be accessed from any device on the network. However, the TUI program provides the benefit of being lightweight.

###### How to run the program
Inside the python script, there should be a path pointing towards whatever file you wish
to manipulate the sensor program for. Moreover, in the SENSOR ARRAY section, there 
should be a commented line which has an array declaration for each sensor.
