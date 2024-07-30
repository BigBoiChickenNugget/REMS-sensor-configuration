from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)


@app.route('/', methods=['GET'])
def index():
    return render_template('index.html')


@app.route('/add-sensor', methods=['GET', 'POST'])
def add_sensor():
    if request.method == 'POST':
        board = request.form['microcontroller']
        sensor = request.form['sensor-name']
        pin = request.form['pin']

        with open('sensors.txt', 'a') as file:
            file.write(f'{board} {sensor} {pin}\n')

        return redirect(url_for('index'))

    return render_template('add-sensor.html')


if __name__ == '__main__':
    app.run(host='192.168.68.118', port=5000, debug=True, threaded=False)
