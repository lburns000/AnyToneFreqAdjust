# AnyTone Frequency Adjust Tool
A simple tool for bulk-applying frequency offsets to an AnyTone channel CSV file.

## Introduction
It is not uncommon for there to be a difference between the programmed and actual radio transmission frequency. Usually this does not pose a problem, but sometimes this can cause issues when using with a digital hotspot. Some symptoms of this are: garbled transmissions originating from local RF, high local RF BER and lack of response from the hotspot when pressing PTT. Note that hotspots also have a frequency adjustment that can be done with its calibration tool (such as MMDVMCal) - running this after the radio is calibrated is recommended to minimize difference of frequency between the radio and hotspot.

**Note: Configuration issues can cause lack of response from the hotspot, mimicking an off-frequency issue. Always check radio and hotspot programming first before applying offsets! Also, a simplex hotspot will not respond to local RF transmissions when it is transmitting - check that your hotspot is not connected to an active talkgroup causing it to transmit.**

If you have found that the radio tx frequency difference is more than ~+-100Hz from the target frequency (using a calibrated frequency counter/service monitor/etc. and transmitting in analog) and you are experiencing the above issues, it may be worth applying an offset. With these particular radios (AnyTone 868, 878, 578, etc.), this is done on a per-channel basis. This tool eliminates the step of opening the file in spreadsheet software and manually editing it.

## Steps
Export the channel CSV of your codeplug, run this tool on it, import the channel CSV again, write to radio.

## Usage:
`AnyToneFreqAdjust <path to file> vhf|uhf|all <offset amount>|reset`

`path to file` is self-explanatory

`vhf|uhf|all`  selects which band to apply the offset to

`<offset amount>|reset` is either for specifiying the offset amount (a signed integer) or resetting the offset to 0 Hz



## Examples:
```
AnyToneFreqAdjust.exe myChannels.csv uhf 300
AnyToneFreqAdjust.exe Channel.csv all reset
```

## Disclaimer:
This software is provided as-is with no guarantees whatsoever. Always back up your codeplugs before modifying them.
