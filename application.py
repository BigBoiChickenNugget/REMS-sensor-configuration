from flask import Flask, render_template, request, redirect, url_for
from sensor import Sensor, get_sensors

app = Flask(__name__)

# Load existing sensors and write their details to 'sensors.txt'
sensors = get_sensors("REMS1")
with open('sensors.txt', 'w') as f:
    for sensor in sensors:
        f.write(f"{sensor.board},{sensor.name},{sensor.pin}\n")

@app.route('/', methods=['GET'])
def index():
    return render_template('index.html')

@app.route('/add-sensor', methods=['GET', 'POST'])
def add_sensor():
    if request.method == 'POST':
        board = request.form['board']
        sensor_name = request.form['sensor-name']
        pin = request.form['pin']

        # Create a new Sensor object and add it
        sensor = Sensor(board, sensor_name, pin)
        print(sensor)
        sensor.add()

        # Redirect to the index page after adding the sensor
        return redirect(url_for('index'))

    return render_template('add-sensor.html')


@app.route('/remove-sensor', methods=['GET', 'POST'])
def remove_sensor():
    if request.method == 'POST':
        board = request.form['board']
        name = request.form['sensor-name']
        pin = request.form['pin']

        # Create a new Sensor object and remove it
        sensor = Sensor(board, name, pin)
        sensor.remove()

        # Redirect to the remove-sensor page after removing the sensor
        return redirect(url_for('remove_sensor'))

    # Get the current list of sensors to display in the form
    sensors = get_sensors("REMS1")
    return render_template('remove-sensor.html', sensors=sensors)


if __name__ == '__main__':
    # Run the Flask application on the specified host and port
    app.run(host='192.168.68.117', port=5000, debug=True, threaded=False)

