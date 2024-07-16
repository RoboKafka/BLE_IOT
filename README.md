# Requirements
Nicla Sense ME
Arduino OPTA PLC WIFI or ESP-32 or Arduino Uno r4 or any module with a BLE capability


## Procedure for Non Opta
1. Run the Nicla sense file on your Nicla sense , this will sensor will start publishing its services for Temperature and acceleration via BLE
2. If you are using opta, skip to the next section, for anyother controller, use the scan for peripheral program, which will subscribe to the nicla sense. connect to the serial monitor to see the data, also you can push this data into another program you have as inputs as well.

## Procedure for Opta
1. Follow step 1 as above.
2. Install Arduino PLC IDE, then open the ble folder and upload it into your opta.
3. I found it easier and quicker to ONLY OPEN THE ARDUINO IDE after clciking the upload button on the Arduino PLC IDE. and yes, they are different.

You will now have a virtual input for your plc ide. potentially extending your inputs from 8 to 9.

I have not tested it while using all the inputs yet so please be careful and use this program at your own risk. 
