from flask import Flask, render_template, request, redirect, url_for
from sensor import Sensor, get_sensors

app = Flask(__name__)

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
        sensor = request.form['sensor-name']
        pin = request.form['pin']

        sensor = Sensor(board, sensor, pin)
        sensor.add()

        return redirect(url_for('index'))

    return render_template('add-sensor.html')


@app.route('/remove-sensor', methods=['GET', 'POST'])
def remove_sensor():
    if request.method == 'POST':
        board = request.form['board']
        name = request.form['sensor-name']
        pin = request.form['pin']

        sensor = Sensor(board, name, pin)
        sensor.remove()

        return redirect(url_for('remove_sensor'))

    sensors = get_sensors("REMS1")
    return render_template('remove-sensor.html', sensors=sensors)


if __name__ == '__main__':
    app.run(host='192.168.68.117', port=5000, debug=True, threaded=False)
