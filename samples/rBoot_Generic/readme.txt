rBoot Generic example

This is an beta implementation of a Generic rBoot layer for easing the use of rBoot and provide a ftp upload to support the esp fully remote.

This PR includes 

"RBootClass" which implements :
    A layer around rBoot/rbootHttpUpdate
    A way to mount/unmount spiffs filesystems, based on rBoot configuration
    An iCommandHandler interface to the functionality.
 
Make targets for : 
	ftp_0x002000  -> upload/flash application to ROM0
	ftp_0x202000  -> upload/flash application to ROM1
	spiffs_100000 -> upload/flash spiffs to ROM2
	spiffs_300000 -> upload/flash spiffs to ROM3 

An example application using the above : rboot_generic

Documentation for now in RBootClass.h and rboot command output
    "rboot info" 
    "rboot load 1 http://10.0.0.201/rom0.bin" 
    "rboot ls" etc type "rboot" for all options.
    "rboot start <rom#>
    "rboot unmount"
    "rboot mount <rom#>

All command examples have equivalent API's -> available within application

FTP usage for upload application & spiffs.
	Upload to flash is deined by setting a "directory" within FTP.
	-> ftpServer.setFlashDirectory("flash");
	Target filenames should be !<hex flashlocation> fe. !0x002000
	In this example FTPServer will flash flash/!0x02000 
	
	The ftp client command client needs to be specified in makefile-user.mk
	
	f.e : FLASH_ROM = c:/Utilities/curl-7.45.0-win32-mingw/bin/curl.exe -P - -T $(FLASH_FILE) ftp://me:123@10.0.0.200/flash/$(FLASH_TARGET)
	The $(FLASH_FILE) and $(FLASH_TARGET) need to be in this specification and will be filled within makefile-rboot.mk
	
Current application version is usable/has hardcoded values for use on 4Mb devices.

The used romconfig is the following :

ROM0 -> 0x002000	-> number from initial rboot figures, application area
ROM1 -> 0x202000	-> number from initial rboot figures, application area
ROM2 -> 0x100000;	-> number set in application code, spiffs area
ROM3 -> 0x300000;	-> number set in application code, spiffs area

Running the example the functionality can be shown by the following commands :

rboot info -> shows ROM info
rboot load 0 http://10.0.0.200/rom0.bin -> error, don't allow current rom update
rboot load 1 http://10.0.0.200/rom0.bin -> http update rom0.bin to ROM1
rboot load 2 http://10.0.0.200/spiffs_rom.bin -> http update spiffs to ROM2
rboot load 3 http://10.0.0.200/spiffs_rom2.bin -> http update spiffs2 to ROM3

rboot start -> start current ROM (= restart)
rboot start 1 -> start ROM1

rboot mount 2 -> mount ROM2 spiffs
rboot ls -> show files on spiffs
rboot unmount -> unmount spiffs
rboot ls -> error not mounted
rboot mount 3 -> mount ROM3 spiffs
rboot ls -> show files on spiffs


