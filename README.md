# VEML6075 Arduino Library

Arduino, et al, library for the Vishay VEML6075 UVA/UVB i2c sensor, a cheap and
easy alternative to the Si1145 and friends (which aren't true UV sensors) and
the raw photodiode GUVA-S12D (and it's rare cousin the GUVB-S11D).

The library handles the gnarly calculations necessary to spit out a UV index value.

# Usage

Pretty straightforward; see the example sketch for a detailed test.

    // Declar sensor controller instance
    VEML6075 my_veml6075 = VEML6075();

    // Initialize i2c bus and sensor
    Wire.begin();
    if (!my_veml6075.begin()) {
      // Do something intelligent if the sensor isn't found
    }

    // Poll sensor
    my_veml6075.poll();

    // Get "raw" UVA and UVB counts, with the dark current removed
    uint16_t uva = my_veml6075.getUVA();
    uint16_t uvb = my_veml6075.getUVB();

    // Get calculated UV index based on Vishay's application note
    float uv_index = my_veml6075.getUVIndex();

# Notes

Unlike the VEML6070, the VEML6075 only uses a single i2c address.
Unfortunately, almost none of the UV index calculations are done onboard, not
even subtracting out the dark current count (I suppose for error checking
purposes?). Also, the output is *not* compensated for visible or IR
interference (I think?), which needs to be compensated. Thus, there's a lot of
number crunching required to get a UV index value out. 

To clear up some of the terrible naming in the datasheet:

* The "dummy" channel is actually the dark current count
* UV compensation channel 1 is visible light
* UV compensation channel 2 is infrared light (IR)

According to the application note, the formula for getting a UV index out of
the sensor's raw values:

    uva_comp = (uva_raw - dark) - a * (vis_comp - dark) - b * (ir_comp - dark)
    uvb_comp = (uvb_raw - dark) - c * (vis_comp - dark) - d * (ir_comp - dark)

Empirical values provided by Vishay:

* a = 3.33 (UVA visible compensation coefficient)
* b = 2.5  (UVA IR compensation coefficient)
* c = 3.66 (UVB visible compensation coefficient)
* d = 2.75 (UVB IR compensation coefficient)

I assume part of the empircal/magic going on is that the sensor is only
measuring two narrow bands, while a "proper" UV index measurement would
integrate the whole UV spectrum, weighted by the erythema function.

# References

[VEML6075 product information]<http://www.vishay.com/optical-sensors/list/product-84304/>
[VEML6075 data sheet]<http://www.vishay.com/docs/84304/veml6075.pdf>
[VEML6075 application note]<http://www.vishay.com/docs/84339/designingveml6075.pdf>
[VEML6040/VEML6075 breakout board]<https://www.tindie.com/products/onehorse/veml6040-rgbw-color-and-ambient-light-sensor/> (not mine)

# See Also

[Adafruit VEML6070 breakout]<https://www.adafruit.com/products/2899> - Adafruit
does great work, and their VEML6070 is the inspiration and starting point for
my library here (but not strictly a derrivative work, or I'd have kept the BSD
license on the lib).
[Adafruit VEML6070 library]<https://github.com/adafruit/Adafruit_VEML6070>
