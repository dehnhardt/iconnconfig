# mioconfig
Linux based configuration utility for MIO midi interfaces
After trying to get the vendor provided config-utility to work under Wine without success I started analyzing the usb packets to write an Linux enabled version.
Later I found the complete documentation for sysex-data from IConnectivity.

Status
* (2017-03-12) nothing relevant done so far
* (2017-03-15) base structure done
* (2017-03-17) device recognition works
* (2017-03-24) initiate device communication
* (2017-04-03) start with parser for returned messages
* (2017-04-09) decision for docked layout as main GUI 
* (2017-04-12) implemented method to check if commands are available for specific device before trying to execute them
* (2017-05-13) only scan all ports if no devices are in config
* (2017-05-14) First working update of device data: Device name can be changed
* (2017-05-15) Read all information from first ethernet device
* (2017-05-20) Some GUI-infrastructure to check for valid ip addresses
* (2017-05-22) Update of method (static, dhcp) for IP-connection implemented (still struggling with verifying static IP-Adresses)
* (2017-06-01) Reading and Displaying the Midi port routings works
