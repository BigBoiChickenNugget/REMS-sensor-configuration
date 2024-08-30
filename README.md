# REMS Sensor Management System

## Overview

The REMS Sensor Management System is a web-based application for managing sensors in a remote environment monitoring system. It provides a web interface for adding and removing sensors, which updates a configuration file used by the hardware system.

## Components

### Flask Web Application (`application.py`)

This Flask application provides a web interface to manage sensors. It includes:
- A main page accessible at `/` (renders `index.html`).
- A route to add new sensors at `/add-sensor` (supports both GET and POST methods).
- A route to remove sensors at `/remove-sensor` (supports both GET and POST methods).

### Sensor Management Library (`sensor.py`)

The `sensor.py` module handles sensor management by updating the `example.ino` configuration file. It includes:
- The `Sensor` class for representing and managing sensor attributes.
- Functions for adding (`add_sensor`) and removing (`remove_sensor`) sensors in the configuration file.

## Installation

1. Clone the repository:
   `git clone https://github.com/your-repo-url.git`

2. Install the required Python packages. Ensure you have Python and Flask installed:
   `pip install Flask`

3. Ensure the `example.ino` file exists in the same directory as `sensor.py`.

## Usage

1. Start the Flask application by running:
   `python application.py`

2. Open a web browser and navigate to:
   `http://192.168.68.117:5000`

3. Use the web interface to manage sensors:
   - **Add Sensors**: Navigate to `/add-sensor` to add new sensors. Provide the board, sensor name, and pin details.
   - **Remove Sensors**: Navigate to `/remove-sensor` to remove existing sensors. Select the board, sensor name, and pin to be removed.

## How It Works

- **Adding Sensors**: When a sensor is added via the web interface, the `add_sensor` function in `sensor.py` updates the `example.ino` file with the new sensor configuration.
- **Removing Sensors**: When a sensor is removed, the `remove_sensor` function modifies the `example.ino` file to remove the specified sensor configuration.
- **Web Interface**: The Flask application provides routes for interacting with these functions and reflects changes in the `example.ino` file.

## Further Development

- **Actual REMS Files**: The current implementation uses `example.ino` as a placeholder. Future development should point to actual REMS files. These REMS files should adhere to a format resembling `example.ino`, but the exact structure may vary depending on specific REMS implementations.
- **Dynamic Updates**: Enhance the web interface to dynamically update sensor readings using AJAX or a more suitable web server library.
- **Sensor Configuration**: Integrate sensor configuration management directly into the web application for improved usability.
