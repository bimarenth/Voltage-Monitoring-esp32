# Voltage-Monitoring-esp32

This project will reading voltage value from 48v battery using esp32 and voltage divider.

- First, to able read 48v from battery, we need making voltage divider from 48v to max 3.3v
- Resistor value is R1 = 100k, R2 = 100k, R3 = 10k.
- Voltage divider value = R2 / (R1 + R2 + R3) = 100k / 210k = 1/21
- Voltage output from voltage divider = 1/21 * 48v = 2,2857v
- Read ADC value from voltage divider using esp32.

## Voltage divider diagram
![image](https://github.com/bimarenth/Voltage-Monitoring-esp32/assets/94059195/0383d301-6fa8-4b11-b851-ff2b050c84b6)

![WhatsApp Image 2023-07-28 at 05 42 25](https://github.com/bimarenth/Voltage-Monitoring-esp32/assets/94059195/16ebc4d5-f7f5-4e28-9a98-cc59b9ae20ca)

## Thinkercad link
https://www.tinkercad.com/things/9ed7D9Qrtfk?sharecode=undefined
