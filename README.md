
This is a sample SPI code to control AD8402 Digital Potentiometer (e.g. from a Raspberry Pi).

On a Raspberry Pi, I use the following in the `/boot/config.txt` to test the code.

```
dtparam=spi=on
dtoverlay=spi0-1cs
```

AD8402 is connected to GND, 3V3, SPI0 CLK, MOSI and CS pins. RS and SHDN pins are tied to 3V3.
