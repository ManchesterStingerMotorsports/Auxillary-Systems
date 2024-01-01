# Auxillary-Systems
Firmware for the EV team's kart computer 


# APPS
- The code for APPS is designed assuming two potentiometers with 10 kOhm total resistance.
- Each potentiometer will be offset by a resistor, the first one by a 1 kOhm resistor and the second one by a 2 kOhm resistor.
- The offset should be present on the low side of the potentiometer.
- The potentiometer with the 1 kOhm offset needs to be connected to PA0 and the other one needs to be connected to PA1. (*This can be easily changed in the code and the IOC files*) 

It needs to be assembled in the following way:
```
    				PWR
				 |
				 \	 
				 /   Potentiometer
	To controller ADC  ----> \
		pins		 /
				 |
				 \
				 /   Offest resistor
				 \
				 /
				 |
				GND
```    
