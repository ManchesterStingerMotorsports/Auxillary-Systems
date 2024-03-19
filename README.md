
* I've tested my code with two 470k pots, two separate power lines 5V and 3.3V
* Any voltage above 3.3V presents issues with scaling due to the 12-bit ADC conversion.
* While a voltage regulator could reduce from 5V to ~ 3.3V, I've used a 333k resistor setup for now. (1M // 1M // 1M)
* There is a 10% offset between Vcc and GND, ensuring readings fall within the 10%-90% range, so we can detect any short or open circuit




       Vcc (5V)                 Vcc (3.3V)
        |                          |
        R1 (333k)                  |
        |                          |
        |                          |
        R2 (60k)                   R1 (60k)
        |                          |
        |                          |
        |                          |
       POT                        POT
        |                          |
        |                          |
        |                          |
        |                          |
        R3 (60k)                   R2 (60k)
        |                          |
        |                          |
        |                          |
       GND                        GND


