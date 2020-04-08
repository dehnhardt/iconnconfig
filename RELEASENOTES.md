
## v.01.
### Linux
*Tested only with MIO10 and iConnect Audio 4+ (where only MIDI-Configuration works...)* 
* what works
  * Searching for devices on all alsa ports
  * Setting the device name
  * Setting the network parameters
  * Setting the routes
  * Storing all settings to FLASH
  * Reading all setting from FLASH (1)
  * Restore current configuration to factory default (1)

* known problems
  * in general error handling is currently nearly not implemented. I still don't have a concept
  * (1) after reading the settings from flash or restoring to factory default, the application might crash - I'm still not sure what the problem is.

### 0.4
#### General
* dependencies
 * I moved some controls to a separate project. See BUILD.md for instructions

#### Linux
* new functionality
 * configuration of audio enabled devices integrated
  * global audio configuration
  * audio mixing
  * audio routing
* fixes
 * better error handling
 * memory leaks fixed

### Windows
someone might try;-)

### Mac
someone might try;-)
