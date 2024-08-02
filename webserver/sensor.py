# Library that controls sensor adding and reading stuff
class Sensor:
    def __init__(self, board, name, pin):
        self.board = board
        self.name = name
        self.ID = number_of_sensors()+1
        self.pin = pin

    def add(self):
        add_sensor(self)


def number_of_sensors():
    num = 0
    with open('example.ino', 'r') as file:
        data = file.readlines()

        for line in data:
            if "#define" in line:
                num += 1

    return num


def add_sensor(sensor):

    lines = []

    # First read entire file and store in an array
    with open('example.ino', 'r') as file:
        while True:
            line = file.readline()
            if str(sensor.name + "_PINS") in line:
                if "{ }" in line:
                    line = line[:-3] + str(sensor.pin) + " }" + ";" + "\n"
                else:
                    line = line[:-3] + ", " + str(sensor.pin) + " }" + ";" + "\n"

            lines.append(line)

            if line == "":
                break

    # Then write the file with the new sensor
    with open('example.ino', 'w') as file:
        for line in lines:
            file.write(line)
