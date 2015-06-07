// Based on the "trublion/sparkcore_osc" release, this version let you connect the Spark Core to your router, without the cloud option (you don't need an internet connexion)
// The receiver (computer) must be connected to the network firt, before connecting the Spark Core.

#include "application.h"
#include "OSC/OSCMessage.h"
#include "OSC/OSCBundle.h"

//----- REGISTERED OSC COMMANDS
char oscReceiveAddress[6] = "/test";

//----- IP ADRESSES
IPAddress computerIPAddress = IPAddress(192,168,0,3);	// put the IP address of your computer here
IPAddress coreIPAddress;
OSCMessage coreIPMessage("/coreip");

IPAddress gateway = IPAddress(192,168,0,1); 			// put the IP address of your router, important to ping it !


//------- PIN

float ap0 = A0;
float ap1 = A1;

//----- PORTS
#define LOCALPORT  5000		// to send data to the Spark Core (from the computer)
#define REMOTEPORT 8888		// to send data to the computer (from here)


// ---- SETUP SYSTEM
SYSTEM_MODE(MANUAL); 		// MANUAL mode automatically disable the cloud connexion




//----- UDP + overloading the inappropriate UDP functions of the Spark Core (REQUIRED !)
class myUDP : public UDP {
private :
	uint8_t myBuffer[1024];
	int offset = 0;
public :
	virtual int beginPacket(IPAddress ip, uint16_t port)
  {
    offset = 0;
    return UDP::beginPacket(ip, port);
	};
	virtual int endPacket()
    {
		return UDP::write(myBuffer, offset);
	};
	virtual size_t write(uint8_t buffer)
    {
		write(&buffer, 1);
		return 1;
	}
	virtual size_t write(const uint8_t *buffer, size_t size)
    {
		memcpy(&myBuffer[offset], buffer, size);
		offset += size;
		return size;
	}
};
myUDP Udp;

//=========================================================================================
//=========================================================================================
void setup()
{

	WiFi.connect(); 		// to force the connexion to the wifi router
	WiFi.ping(gateway);		// just ping the router; if we do not, the Spark Core cannot send UDP over the network; that's the key !

	delay(1000);

  Udp.begin(LOCALPORT);

	// Get the IP address of the Spark Core and send it as an OSC Message
	coreIPAddress = WiFi.localIP();
	coreIPMessage.add(coreIPAddress[0]).add(coreIPAddress[1]).add(coreIPAddress[2]).add(coreIPAddress[3]);

	Udp.beginPacket(computerIPAddress, REMOTEPORT);
	coreIPMessage.send(Udp);
	Udp.endPacket();

	pinMode(A3, INPUT);

  delay(1000);
}

void sendMessage(OSCMessage &message)
{
  Udp.beginPacket(computerIPAddress, REMOTEPORT);
  message.send(Udp);
  Udp.endPacket();
}

//=========================================================================================

void loop()
{
  ap0 = analogRead(A0);
  
// --- Send Message

	OSCMessage message("/A0/");
	message.add(ap0);
	sendMessage(message);
}
