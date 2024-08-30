// Libraries for Ethernet connection.
#include <SPI.h>
#include <Ethernet.h>

// Libraries for temperature sensors (need to implement).
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// SENSOR PIN
const int DS18B20_PINS[] = { 0 };
const int DHT11_PINS[] = { 0 };

// DS18B20 SETUP

// Mac address of Arduino REMS board.
byte mac[] = {
    0x2C, 0xF7, 0xF1, 0x08, 0x33, 0x4E
};

// IP address for the webpage.
IPAddress ip(192, 168, 3, 160);

EthernetServer server(80);

// String that'll store the response from the webpage.
String httpResponse;

// Array to store status of buttons. Order is heat, cool, power, water.
boolean status[] = {false, false, false, false};

void setup() {

    // Begin serial monitor.
    Serial.begin(9600);

    // Start webserver.
    Ethernet.begin(mac, ip);
    server.begin();

    // Setup all pins.
    pinMode(HEATREQUEST, OUTPUT);
    pinMode(COOLREQUEST, OUTPUT);
    digitalWrite(HEATREQUEST, LOW);
    digitalWrite(COOLREQUEST, LOW);

    pinMode(SMOKEALARM, INPUT);

    pinMode(POWERSHUTOFF, OUTPUT);
    digitalWrite(POWERSHUTOFF, LOW);

    pinMode(WATERSHUTOFF, OUTPUT);
    digitalWrite(WATERSHUTOFF, LOW);

    // Setup sensors
    pinMode(VIBRATION1, INPUT);
    pinMode(VIBRATION2, INPUT);
    pinMode(VIBRATION3, INPUT);

    pinMode(MOTION1, INPUT);
    pinMode(MOTION2, INPUT);
    pinMode(MOTION3, INPUT);
    pinMode(MOTION4, INPUT);

    // DHT Temp sensors
    pinMode(DHT22_1, INPUT);
    pinMode(DHT22_2, INPUT);
    pinMode(DHT11_1, INPUT);
    pinMode(DHT11_2, INPUT);
    dht1.begin();
    dht2.begin();
    dht3.begin();
    dht4.begin();

    // LM35DZ Temp sensors
    pinMode(LM35DZ_1, INPUT);
    pinMode(LM35DZ_2, INPUT);
    pinMode(LM35DZ_3, INPUT);

    // DS18B20 temp sensors
    sensor1.begin();
    sensor2.begin();
    sensor3.begin();

}

void loop() {

    // Listen for incoming clients.
    EthernetClient client = server.available();

    // Read sensors
    readVibration();
    readMotion();
    readDHT();
    readLM35DZ();
    readDS18B20();

    // If a client is found...
    if (client) {

	// If the client is availalble, read the incoming HTTP request.
	if (client.available()) {

	    // Calls a function to read the HTTP request and stores it in the httpResponse global String.
	    readRequest(client);

	    // Displays the updated webpage in line with the request, and sends whatever commands the HTTP request asked to do.
	    ClientResponse(client);

	    delay(1);
	    client.stop();
	}
    }
}

// Function to display entire webpage and process the HTTP request.
void ClientResponse(EthernetClient client) {

    // Send http request.
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Refresh: 5");
    client.println();

    // Begin HTML
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    // Code for the head portion of the webpage
    client.println("<head>");
    client.println("<title>REMS CONTROL CENTRE 1</title>");

    client.println("</head>");

    // Start body portion of site. Header that has site label.
    client.println("<body>");
    client.println("<h1>REMS006 1</h1>");
    client.println("<h4>192.168.3.160</h4>");

    String heat, cool, power, water;
    for (int i = 0; i < 4; i++) {
	if (!status[i]) {
	    if (i == 0) heat = "HEAT ON";
	    if (i == 1) cool = "COOL ON";
	    if (i == 2) power = "POWER OFF";
	    if (i == 3) water = "WATER OFF";
	}
	else if (status[i]) {
	    if (i == 0) heat = "HEAT OFF";
	    if (i == 1) cool = "COOL OFF";
	    if (i == 2) power = "POWER ON";
	    if (i == 3) water = "WATER ON";
	}
    }

    client.println("<a href='/?heatrequest'><button>" + heat + "</button></a>");
    client.println("<a href='/?coolrequest'><button>" + cool + "</button></a>");
    client.println("<a href='/?powerOff'><button>" + power + "</button></a>");
    client.println("<a href='/?waterOff'><button>" + water + "</button></a>");


    // End body and HTML.
    if (status[0]) {
	digitalWrite(HEATREQUEST, HIGH);
	Serial.println("HEAT ON");
    }

    else {
	digitalWrite(HEATREQUEST, LOW);
	Serial.println("HEAT OFF");
    }

    if (status[1]) {
	digitalWrite(COOLREQUEST, HIGH);
	Serial.println("COOL ON");
    }

    else {
	digitalWrite(COOLREQUEST, LOW);
	Serial.println("COOL OFF");
    }

    if (status[2]) {
	digitalWrite(POWERSHUTOFF, HIGH);
	client.println("POWER OFF");
    }

    else {
	digitalWrite(POWERSHUTOFF, LOW);
    }

    if (status[3]) {
	digitalWrite(WATERSHUTOFF, HIGH);
	client.println("WATER OFF");
    }

    else {
	digitalWrite(WATERSHUTOFF, LOW);
    }

    // Div tag for the vibration sensors.
    client.println("<div style='position: relative; top: 10%'>");
    client.println("<h4>Vibration Sensors</h4>");
    client.println("<p>Living Room: " + String(vibrations[0]) + "</p>");
    client.println("<p>Kitchen: " + String(vibrations[1]) + "</p>");
    client.println("<p>Laundry: " + String(vibrations[2]) + "</p>");
    client.println("</div>");

    // Div tag for the motion sensors.
    client.println("<div style='position: relative; top: 10%'>");
    client.println("<h4>Motion Sensors</h4>");
    client.println("<p>Living Room: " + String(motions[0]) + "</p>");
    client.println("<p>Kitchen: " + String(motions[1]) + "</p>");
    client.println("<p>Laundry: " + String(motions[2]) + "</p>");
    client.println("<p>EQ: " + String(motions[3]) + "</p>");
    client.println("</div>");

    // Div tag for the DHT sensors.
    client.println("<div style='position: relative; top: 10%'>");
    client.println("<h4>DHT Sensors</h4>");
    client.println("<p>Living Room: " + String(dht[0]) + "</p>");
    client.println("<p>Kitchen: " + String(dht[1]) + "</p>");
    client.println("<p>Laundry: " + String(dht[2]) + "</p>");
    client.println("<p>EQ: " + String(dht[3]) + "</p>");
    client.println("</div>");

    // Div tag for the LM35DZ sensors.
    client.println("<div style='position: relative; top: 10%'>");
    client.println("<h4>LM35DZ Sensors</h4>");
    client.println("<p>Kitchen: " + String(LM35DZ[0]) + "</p>");
    client.println("<p>Laundry: " + String(LM35DZ[1]) + "</p>");
    client.println("<p>Bathroom: " + String(LM35DZ[2]) + "</p>");
    client.println("</div>");

    // Div tag for the DS18B20 sensors.
    client.println("<div style='position: relative; top: 10%'>");
    client.println("<h4>DS18B20 Sensors</h4>");
    client.println("<p>Living Room: " + String(DS18B20[0]) + "</p>");
    client.println("<p>Kitchen: " + String(DS18B20[1]) + "</p>");
    client.println("<p>Laundry: " + String(DS18B20[2]) + "</p>");
    client.println("</div>");

    client.println("</body></html>");
}

//  Function that reads the incoming HTTP request.
void readRequest(EthernetClient client) {

    // Boolean variable to store if the request is POST (sending states of buttons).
    httpResponse = "";

    char c = client.read();

    // Iterate through all the strings until the newline appears (in the case of a GET request) or until the line with all the checkbox statuses appears (in the case of a POST request).
    while (c != '\n') {
	httpResponse += c;
	c = client.read();
    }

    if (httpResponse.indexOf("?heatrequest") >= 0) {
	status[0] = !status[0];
    }
    if (httpResponse.indexOf("?coolrequest") >= 0) {
	status[1] = !status[1];
    }
    if (httpResponse.indexOf("?powerOff") >= 0) {
	status[2] = !status[2];
    }
    if (httpResponse.indexOf("?waterOff") >= 0) {
	status[3] = !status[3];
    }
}

// Functions to read the sensors.
void readVibration() {
    vibrations[0] = digitalRead(VIBRATION1);
    vibrations[1] = digitalRead(VIBRATION2);
    vibrations[2] = digitalRead(VIBRATION3);
}

void readMotion() {
    motions[0] = digitalRead(MOTION1);
    motions[1] = digitalRead(MOTION2);
    motions[2] = digitalRead(MOTION3);
    motions[3] = digitalRead(MOTION4);

}

void readDHT() {
    dht[0] = dht1.readTemperature();
    dht[1] = dht2.readTemperature();
    dht[2] = dht3.readTemperature();
    dht[3] = dht4.readTemperature();
}

void readLM35DZ() {
    float temp1 = analogRead(LM35DZ_1);
    float realTemp1 = temp1 * 4.88;
    realTemp1 = realTemp1 / 10;

    float temp2 = analogRead(LM35DZ_2);
    float realTemp2 = temp2 * 4.88;
    realTemp2 = realTemp2 / 10;

    float temp3 = analogRead(LM35DZ_3);
    float realTemp3 = temp3 * 4.88;
    realTemp3 = realTemp3 / 10;

    LM35DZ[0] = realTemp1;
    LM35DZ[1] = realTemp2;
    LM35DZ[2] = realTemp3;
}

void readDS18B20() {
    sensor1.requestTemperatures();
    sensor2.requestTemperatures();
    sensor3.requestTemperatures();

    DS18B20[0] = sensor1.getTempCByIndex(0);
    DS18B20[1] = sensor2.getTempCByIndex(0);
    DS18B20[2] = sensor3.getTempCByIndex(0);
}
