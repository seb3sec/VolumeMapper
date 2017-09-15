# VolumeMapper
This "quick and dirty" tool maps volume up/down input to the foreground application.

**_"What's it good for?"_**
The primary use for this is when you're playing a game while chatting with your friends via voip software. If the game becomes too loud to understand your friends you can simply turn its volume down without diving into any option menu.

## Usage
You can download the binary and put it on your desktop. After startup the tool appears as an icon in the system tray (bottom right of your screen). 
You can then use hardware buttons/knobs/wheels/... which normally adjust the master volume to control application specific volume levels.

To stop the tool, just right-click on the icon and choose "exit".

This tool has been tested on Windows 10 (64 bit) with fullscreen windowed applications. 

## Features
If you hold your Ctrl key while changing volume, this tool doesn't change the applications volume and instead Windows changes the master volume level as usual. *(This was initially a bug but I decided it's quite useful.)*

## Bugs
If you find any bugs you can report them using the issues tab. Due to different hardware/software it might be hard to track them down. Including as much information as possible may be helpful.

## Credits
Thanks to Mark Zitnik for his [SysTrayDemo on CodeProject](https://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win).
