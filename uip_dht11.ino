// UIPEthernet EchoServer example : https://github.com/ntruchsess/arduino_uip
#include <UIPEthernet.h>
EthernetServer server = EthernetServer(80);

// ladyAda DHT11 tutorial http://learn.adafruit.com/dht
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11
// what pin we're connected to ,  pin 1 (on the left) of the sensor to +5V, pin 2 of the sensor to whatever your DHTPIN is, pin 4 (on the right) of the sensor to GROUND, a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;

// Using Plot.ly's Arduino API to visualize Temperature and Humidity Readings from A DHT22 Sensor
#include "plotly_ethernet.h"
plotly plotly; // initialize a plotly object, named plotly


//char layout[]="{}";
//char filename[] = "Temp_humid"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data


void temperature ()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }  
}

void webserver()
{
  size_t size;

  if (EthernetClient client = server.available())
    {
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          Serial.write(msg,size);
          free(msg);
        }
      client.println("DATA from Server!");
      client.println("humitat = ");
      client.println(h);      
      client.println("temperatura = ");  
      client.println(t);          
      client.stop();
    }  
}
/*
void plotlySetup()
{
  // Initialize plotly settings
  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers 
  plotly.username = "albertsp"; // your plotly username -- sign up at https://plot.ly/ssu or feel free to use this public account. password of the account is "password"
  plotly.api_key = "not in github"; // "public_arduino"'s api_key -- char api_key[10]
  plotly.timestamp = true; // tell plotly that you're stamping your data with a millisecond counter and that you want plotly to convert it into a date-formatted graph
  plotly.timezone = "Europe/Madrid"; // full list of timezones is here:
  
}  

void plotlyPost()
{
      // Open the Stream
      plotly.open_stream(1, 2, filename, layout); // plotlystream(number_of_points, number_of_traces, filename, layout)

      plotly.post(millis(),t); // post temperature to plotly (trace 1)
      delay(150);
      plotly.post(millis(),h); // post humidity to plotly (trace 2)  
}
*/
void setup()
{
  Serial.begin(9600);

  // webserver 
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,0,25);
  Ethernet.begin(mac,myIP);
  server.begin();

  // temperatura   
  dht.begin();
  
  // plotlySetup
  //plotlySetup();
}

void loop()
{
  // plotly timer 
  //plotlyPost();
  
  // webserver 
  webserver();  
  
  // temperature
  temperature();
  Serial.println("Completed Loop");
}

