ESP WIFI Terminal

Application for imitation serial interface with microcontroller esp32\8266  by a local network method. For interaction with microcontroller library https://github.com/VLS-Esp32/VLS_Esp32_Wifi_Terminal must be integrated into sketch. For additional information observe documentation in  library  https://github.com/VLS-Esp32/VLS_Esp32_Wifi_Terminal/blob/Esp32/README.md

Application allows automatically detect microcontroller`s of local network and exchange data in text format

Assembly
For assembling you need Qt 5.15 and higher;
Supported platforms: Windows x86, Linux x86, х86_64, Linux (aarch64).
 
Windows – you need open Qt Creator. As compiler desirable use minGW 32bit,  possibly using minGW 64bit but performance is not guaranteed , any other compiler won't work. Any other special libriaries  not required. 

Linux x86, х86_64, Linux (aarch64) - you need open Qt Creator and compile with any available compiler.
 
Attention
Most firewall`s on Linux blocks automatic search of microcontroller. Please make sure that the port 3283 (udp) is available for incoming connections.
Class ModuleSearch() and ModuleConnection() are completely autonomous and can be used for any projects on byte-by-byte transfer of data between PC and esp32.
Possible incorrect work on Windows, in case of problems use another application that is in the folder Soft repository in library esp32 you can find ready-made binaries here https://github.com/VLS-Esp32/Esp32_Wifi_Terminal/releases/tag/v1.0.1
