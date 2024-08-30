# Library that controls sensor adding and reading
class Sensor:
    def __init__(self, board, name, pin):
        # Initialize a Sensor object with board, name, and pin
        self.board = board
        self.name = name
        self.pin = pin

    def add(self):
        # Add this sensor to the 'example.ino' file
        add_sensor(self)

    def remove(self):
        # Remove this sensor from the 'example.ino' file
        remove_sensor(self)

    def __str__(self):
        # Return a string representation of the Sensor object
        return f"Sensor: {self.name} | Pin: {self.pin} | Board: {self.board}"

def number_of_sensors():
    # Count the number of sensors defined in 'example.ino' by looking for '#define' directives
    num = 0
    with open('example.ino', 'r') as file:
        data = file.readlines()
        for line in data:
            if "#define" in line:
                num += 1
    return num

def add_sensor(sensor):
    # Add a sensor configuration to the 'example.ino' file
    lines = []

    # Read the entire 'example.ino' file and store it in an array
    with open('example.ino', 'r') as file:
        while True:
            line = file.readline()

            # Insert or update pin number for the sensor
            if str(sensor.name + "_PINS") in line:
                if "{ 0 }" in line:
                    line = line.replace("{ 0 }", "{ " + str(sensor.pin) + " }")
                else:
                    line = line.replace("}", f", {sensor.pin} }}")

            # Handle DS18B20 sensor setup
            if str(sensor.name) == "DS18B20" and "DS18B20 SETUP" in line:
                lines.append(line)

                # Append lines for DS18B20 sensor setup until an empty line is found
                while True:
                    line = file.readline()
                    if line == "\n":
                        break
                    lines.append(line)

                # Add DS18B20 sensor initialization
                lines.append(f"OneWire DS18B20_{sensor.pin}({sensor.pin});\n")
                lines.append(f"DallasTemperature DS18B20_{sensor.pin}(&DS18B20_{sensor.pin});\n")

            lines.append(line)

            if line == "":
                break

    # Write the modified content back to 'example.ino'
    with open('example.ino', 'w') as file:
        for line in lines:
            file.write(line)

def remove_sensor(sensor):
    # Remove a sensor configuration from the 'example.ino' file
    lines = []

    # Read the entire 'example.ino' file and store it in an array
    with open('example.ino', 'r') as file:
        while True:
            line = file.readline()

            # Remove the pin number from the sensor configuration
            if str(sensor.name + "_PINS") in line:
                if str("{ " + sensor.pin + " }") in line:
                    line = line.replace(f"{{ {sensor.pin} }}", "{ 0 }")
                elif f"{{ {sensor.pin}" in line:
                    line = line.replace(f"{{ {sensor.pin} ,", "{")
                elif f"{sensor.pin} }}" in line:
                    line = line.replace(f", {sensor.pin} }}", "}")
                elif f" , {sensor.pin}" in line:
                    line = line.replace(f" , {sensor.pin}", " , ")

            # Skip lines that initialize DS18B20 sensor setup
            if line == f"OneWire DS18B20_{sensor.pin}({sensor.pin});\n":
                continue
            if line == f"DallasTemperature DS18B20_{sensor.pin}(&DS18B20_{sensor.pin});\n":
                continue

            lines.append(line)

            if line == "":
                break

    # Write the modified content back to 'example.ino'
    with open('example.ino', 'w') as file:
        for line in lines:
            file.write(line)

def get_sensors(board):
    # Retrieve the list of sensors defined in 'example.ino' for a given board
    sensors = []

    with open('example.ino', 'r') as file:
        data = file.readlines()
        for line in data:
            if "_PINS" in line:
                sensor_name = line.split(" ")[2].split("_")[0]
                for pin in line.split(" "):
                    if pin.isdigit() and pin != "0":
                        sensors.append(Sensor(board, sensor_name, pin))

    return sensors

