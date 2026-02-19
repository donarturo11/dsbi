# DSBI Transmitter firmware for attiny13/x5
This code is for AVR(R) attiny13/x5. Firmware was tested with
simulide and simulavr.
It samples EEG data with oversampling, and results are sent 512 times in second.
## Data format
```
data[]={ 0b10HHHHH, 0b01LLLLL }
H - High (MSB)
L - Low (LSB)
```.
## Usage
To aqcuire data use [QBrainwaveOSC](https://github.com/donarturo11/QBrainwaveOSC).
A simple circuit is in dsbi-transmitter-test.sim1 opened with
[SimulIDE](https://simulide.com/p/).