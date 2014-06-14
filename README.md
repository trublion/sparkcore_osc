sparkcore_osc
=============

OSC library (Oscuino) for the Spark Core

Let's keep in touch : 
- http://www.trublion.org
- https://www.facebook.com/trubliondotorg
- https://twitter.com/trubliondotorg
- contact@trublion.org

We've been working on porting the [Oscuino library] (https://github.com/CNMAT/OSC) to the Spark Core, and we've faced the same problems as the ones mentioned in [that thread](https://community.spark.io/t/osc-open-sound-control-with-spark-core/371) by [jfenwick](https://github.com/jfenwick).

Instead of rewriting the `UDPSend()` function of the `OSCMessage` and `OSCBundle` classes, we've identified that the issues came from the Spark's core-firmware code (`UDP` class in `spark_wiring_udp.cpp`).

We've overloaded the misfunctioning functions (i.e. `beginPacket()`, `endPacket()` and `write()`), and added proper includes in the Oscuino files, as explained in the following. 

Our library is provided with Puredata and Max/MSP example patches to test the functionalities of our code.

This code will only work with the most recent versions of the core-firmware (or the current online compiler of the cloud), since we use the `CFLAGS += -DSPARK` recently defined in the `makefile`.



### 1. What we have changed in the Spark Core's code : 

- We have overloaded the mysfunctioning UDP functions of the Spark Core (`myUDP` class in `application.cpp`)



### 2. What we have changed in the Oscuino code : 

- Proper includes have been added to `OSCData.h` and `OSCMessage.h`

- We do not use the `OSCTiming` class and `OSCBoards.h` (both must be rewritten to work with the Spark)

- All modifications are labelled with the following comment : `// Simon+Emilien-OSCUINO`



### 3. Setup (we're compiling our code with `make` as explained in the [documentation](https://github.com/spark/core-firmware/) ) : 

- Put `OSCBundle.cpp`, `OSCData.cpp`, `OSCMatch.c`, `OSCMessage.cpp` and `build.mk` into `core-firmware/src/OSC`

- Put `OSCBundle.h`, `OSCData.h`, `OSCMatch.h` and `OSCMessage.h` into `core-firmware/inc/OSC`

- Put the IP address of your computer into `application.cpp` (search for the comment `// put the IP address of your computer here`)



### 4. Use (test application) : 

- Wire LEDs to the `D0` and `D1` pins

- Send `OSCMessages` and `OSCBundles` to the Spark Core on port `8888`

- Receive `OSCMessages` and `OSCBundles` from the Spark Core on port `9999`

- The IP of the Core is automatically sent at startup as an `OSCMessage` (OSC address = `/coreip`)

- The Spark Core will manage either `OSCMessages` or `OSCBundles` (not both at the same time)

- 5 OSC addresses are taken into account by our test application:
	* `/manageMessages` (no arguments) will switch the Core to managing `OSCMessages` (`OSCBundles` will be ignored)
	* `/manageBundles` (no arguments) will switch the Core to managing `OSCBundles` (`OSCMessages` will be ignored)
	* `/sendTestMsg` (no arguments) will ask the Core to send an `OSCMessage` for test (its address is `/testmessage`)
	* `/sendTestBndl` (no arguments) will ask the Core to send an `OSCBundle` for test (its address is `/testbundle/*`)
	* `/led` followed by two ints will switch on/off the two LEDs
	  [The 1st int is the pin number (0 for `D0`, 1 for `D1`). The 2nd int is the LED state (0 for `LOW`, 1 for `HIGH`)]



### 5. PD and Max/MSP patches :

- To test our Max/MSP patch, download and install the [Max OSC library made by the CNMAT](http://cnmat.berkeley.edu/downloads)

- Start the patch and reboot the Spark Core (short press the "RST" button)

- The IP address of the Core is automatically setup (it is sent as an `OSCMessage` when the Core boots and registered to the `udpsend` object)

- By default, the Core will manage `OSCMessages` (`OSCBundles` are ignored at startup)

- Test the functionalities

[[Simon-L](https://github.com/Simon-L) & [Emilien-G](https://github.com/emilien-g) for [trublion](https://github.com/trublion)]
