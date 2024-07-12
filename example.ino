// Libraries for Ethernet connection.
#include <SPI.h>
#include <Ethernet.h>

// Libraries for temperature sensors (need to implement).
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// SENSOR PIN
#define DS18B200 1
#define DS18B201 2
#define DS18B202 3
#define DHT0 7

// SENSOR ARRAY
float DHT[1] = {0};
float DS18B20[1] = {0};


// Mac address of Arduino REMS board.
byte mac[] = {
    0x2C, 0xF7, 0xF1, 0x08, 0x33, 0x4E
};

// IP address for the webpage.
IPAddress ip(192, 168, 3, 160);

EthernetServer server(80);



void setup() {

    // Begin serial monitor.
    Serial.begin(9600);

    // Start webserver.
    Ethernet.begin(mac, ip);
    server.begin();
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
    client.println();

    // Begin HTML
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    // Code for the head portion of the webpage
    client.println("<head>");
    client.println("<title>REMS SENSOR ADDING TEST</title>");

    client.println("</head>");

    // Start body portion of site. Header that has site label.
    client.println("<body>");
    client.println("<h1>REMS SENSOR ADDING TEST</h1>");
    client.println("<h4>192.168.3.160</h4>");

	// SENSOR WEBPAGE
	
    client.println("</body></html>");
}
client.println("DHT");

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
