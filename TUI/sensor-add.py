# Import TUI library
import pytermgui as ptg


# Dictionary to store the output of the input fields from the TUI.
OUTPUT = {}

# List that stores all the lines in the file with all the information added.
finalFile = []

# Function that is called when the submit button is pressed.
def submit(manager: ptg.WindowManager, window: ptg.Window) -> None:

    # Get the values from the input fields and store them in the OUTPUT dictionary.
    for widget in window:
        if isinstance(widget, ptg.InputField):
            OUTPUT[widget.prompt] = widget.value
            continue

    # Close the window.
    manager.stop()

    # Open the file and read all the lines.
    file = open("example.ino", "r")

    # Num to store the number of preexisting sensors
    num = 0

    # Loop through all the lines in the file.
    while True:

        # Read the line.
        line = file.readline()

        # Add the line to the finalFile list.
        finalFile.append(line)


        # If the line contains "SENSOR PIN" then add the new sensor's pin to the file.
        if "SENSOR PIN" in line:

            # Loop through the file until we get an empty line.
            while True:

                # Read the line.
                line = file.readline()

                # If the line contains the sensor name then increment the num variable.
                if OUTPUT["Sensor:"] in line:
                    num += 1

                # If the line is empty then add the new sensor to the file.
                if line == "\n":
                    line = "#define " + OUTPUT["Sensor:"] + str(num) + " " + OUTPUT["Pin:"] + "\n"
                    finalFile.append(line)

                    # Add the empty line to the file.
                    finalFile.append("\n")
                    break

                # Add the existing sensor lines to the file.
                finalFile.append(line)

        # If SENSOR ARRAY is on the line then we need to update the sensor's array with the number of sensors.
        if "SENSOR ARRAY" in line:

            # Loop through the file until we get the sensor's array.
            while True:

                # Read the line.
                line = file.readline()

                # If the line contains the sensor name then update the array with the new number of sensors.
                if OUTPUT["Sensor:"] in line:
                    line = "float " + OUTPUT["Sensor:"] + "[" + str(num + 1) + "]" + " = {0};\n"
                    finalFile.append(line)
                    break

                # Add the existing sensor lines to the file.
                finalFile.append(line)

        if "SENSOR WEBPAGE" in line:
            while True:
                line = file.readline()

                if line == "\n":
                    line = "client.println(\"" + OUTPUT["Sensor:"] + "\");\n"
                    finalFile.append(line)
                    finalFile.append("\n")
                    break;
                
                finalFile.append(line)

        if "DHT SETUP" in line and OUTPUT["Sensor:"] == "DHT11":
            while True:
                line = file.readline()

                if line == "\n":
                    line = "dht.setup(" + OUTPUT["Pin:"] + ");\n"
                    finalFile.append(line)
                    finalFile.append("\n")
                    break;

                finalFile.append(line)


        # If we reach the end of the file then break the loop.
        if line == "":
            break

    file.close()
    file = open("example.ino", "w")
    file.writelines(finalFile)


        

# STUFF TO GET INPUT. TEMPORARY
CONFIG = """
config:
    InputField:
        styles:
            prompt: dim italic
            cursor: '@72'
    Label:
        styles:
            value: dim bold

    Window:
        styles:
            border: '60'
            corner: '60'

    Container:
        styles:
            border: '96'
            corner: '96'
"""

with ptg.YamlLoader() as loader:
    loader.load(CONFIG)

with ptg.WindowManager() as manager:
    window = (
        ptg.Window(
            "",
            ptg.InputField(" \"REMS1\" or \"REMS2\"", prompt="REMS Version:"),
            ptg.InputField(" \"DHT11\" or \"DS18B20\"", prompt="Sensor:"),
            ptg.InputField(" Pin number for the sensor", prompt="Pin:"),
            "",
            "",
            ["Submit", lambda *_: submit(manager, window)],
            width=60,
            box="DOUBLE",
        )
        .set_title("[210 bold]New Sensor")
        .center()
    )

    manager.add(window)
