# Library that controls sensor adding and reading stuff
class Sensor:
    def __init__(self, board, name, pin):
        self.board = board
        self.name = name
        self.pin = pin

    def add(self):
        add_sensor(self)

    def remove(self):
        remove_sensor(self)


# Might get rid of this since it seemes useless
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


def remove_sensor(sensor):

    lines = []

    # First read entire file and store in an array
    with open('example.ino', 'r') as file:
        while True:
            line = file.readline()
            if str(sensor.name + "_PINS") in line:

                # In the middle
                if f" , {sensor.pin}" in line:
                    line = line.replace(f" , {sensor.pin}", " , ")

                # Empty list
                elif f"{{ {sensor.pin} }}" in line:
                    line = line.replace(f"{{ {sensor.pin} }}", "{ }")

                # First element
                elif f"{{ {sensor.pin}" in line:
                    line = line.replace(f"{{ {sensor.pin} ,", "{")

                # Last element
                elif f"{sensor.pin} }}" in line:
                    line = line.replace(f", {sensor.pin} }}", "}")

            lines.append(line)

            if line == "":
                break

    # Then write the file with the new sensor
    with open('example.ino', 'w') as file:
        for line in lines:
            file.write(line)


def get_sensors(board):
    sensors = []

    with open('example.ino', 'r') as file:
        data = file.readlines()

        for line in data:

            if "_PINS" in line:
                sensor = line.split(" ")[2].split("_")[0]

                for pin in line.split(" "):
                    if pin.isdigit():
                        sensors.append(Sensor(board, sensor, pin))

    return sensors
