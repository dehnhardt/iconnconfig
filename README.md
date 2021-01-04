# iconnconfig
**This repository is archived.
**Further development will be done at https://codeberg.org/dehnhardt/IConnConfig**

Linux based configuration utility for IConnectivity interfaces. MIDI and Audio configuration is implemented.
Since the SysEx interface is very well documented by iConnectivity, most of the functionality is implemented.
The extensions (Command Version Number 2) - presumably for the newer devices - are not implemented, because such a device is not available to me.

## Implemented
* Device detection
* Global Configuration
  * Device Name
  * IP Configuration
* MIDI Configuration
  * MIDI Routing
  * MIDI Filtering
  * MIDI Controller and Channel Remapping
* Audio Configuration
  * Global Audio Parameter
  * Audio Port Parameter
  * Audio Input and Output Configuration
  * Audio Mixing

## Known Problems
* Audio-Mixer
  * The levelmeters in USB output channels do not show anything (which is the same in the vendor tool)

## Tested Devices
* MIO 4
* MIO 10
* iConnect Audio 4+

## Opereating systems
* Linux Ubuntu
  * Tested: 18.04 (Bionic)
* Linux Debian
  * Tested: 10 (Buster)
* Windows 8
  * drivers from iconnectivity must be installed
  * compilation needs to be done with the mingw compiler 
* Windows 10
  * drivers from iconnectivity must be installed
  * compilation needs to be done with the mingw compiler 

## Status
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
* (2017-06-04) Applied a patch from Ralf Beck to make it compile with qt 4.8
* (2017-06-06) Added methods to update port routings. [Port routing](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/PortRoutingSettings.png) complete.
* (2017-06-07) Save settings to device and restore settings from device works
* (2017-06-12) Saving of static IP-address, netmask and gateway works
* (2017-06-14) Release of version v0.1-beta
* (2017-06-19) Added some screenshots to the WIKI-Page https://github.com/dehnhardt/iconnconfig/wiki
* (2017-07-14) Added error handling with exceptions
* (2017-07-18) "About Box" added, credits to everyone...
* (2017-08-05) Translations, translations....
* (2017-08-12) Tested with iConnect Audio 4+: MIDI functionality works, audio is to come
* (2017-09-05) Enhanced error messages
* (2019-02-13) French translations added by Olivier Humbert
* (2019-02-13) Fixes in translation code from JP Cimalando
* (2019-02-14) Fix bug in error handling, Fix wrong calculation of command id, All initializations of pointers changed from 0 or NULL to nullptr
* (2019-02-15) Application icon added (Thank again to Olivier Humbert). Device recognition made significantly faster by only querying inputs and outputs of the same physical ports.
* (2019-02-22) Added [portfilter configuration](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/PortFilterSettings.png) for system messages, channel messages and controllers for in- and output ports
* (2019-02-22) Changed the portrouting panel slightly
* (2019-02-23) Add [portinfo widget](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/PortSettings.png). Port name and status can now be changed
* (2019-02-26) Add [portmapping widget](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/PortRemapSettings.png) for in- and output ports.
* __(2019-02-26) MIDI configuration is complete__ _Branch v0.3-beta_
* (2019-03-05) [Global audio settings](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/GlobalAudioParm.png) can be configured
* (2019-03-07) [Audioport parameter](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/AudioPortParm.png) can now be configured
* (2019-03-22) [Audiocontrols / Input- and Output Ports](https://github.com/dehnhardt/wikis/blob/master/mioconfig/pictures/AudioMixing.png) implemented.
* (2019-03-30) Did a lot of refactoring. I lost track of memory leaks and decided to use samrt pointer at several places to make memory handling somewhat easier. There are still some problems, especially when retrieving the meter levels. I had to investigate further and rethink about this.
* (2019-04-14) All memoy leaks fixed.
* (2019-04-20) Start with the audiomixer. Create all in- and output channels
* (2019-04-23) Create branch for the audiomixer. It seems to be a big addition which might take some weeks.
* (2019-12-28) Create a branch with audio port settings only _Branch v0.4-beta_
* (2020-04-07) Merged audiomixer branch into master. [Routing](https://github.com/dehnhardt/iconnconfig/wiki#audio-routing) and [mixing](https://github.com/dehnhardt/iconnconfig/wiki#audio-mixer) works. Some visual updates to controls and grids.
* (2020-04-07) _Branch v0.4-beta_ is the release branch with all audio functions
* (2020-04-17) Replaced the QT-Dial (which looked ugly on soe QT-Versions) with a more flexible dial. It shows a logarithmic scale and displays the value.
* (2020-04-18) Fixed some bugs when mixer channels are linked: Input: Only valuechanges on the left channel should be sent to the device. Output: Channels are always linked, so only use the 'left' channel for all requests.
* (2020-04-20) Enable compiling for Windows: Move enumerations to namespace "pk", because at least one enum conflicted with a windows definition. Exclude Linux-Only code with preprocessor statements. Add the Windows multimedia library when compiled for windows.
* (2020-04-21) _Branch v0.5-beta_ Stabilisation of audio functions, improvements of some control elements, Windows compatibility
