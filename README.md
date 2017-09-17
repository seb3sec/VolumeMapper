# VolumeMapper
This "quick and dirty" tool maps volume up/down input to the foreground application.

**_"What's it good for?"_**
The primary use for this is when you're playing a game while chatting with your friends via voip software. If the game becomes too loud to understand your friends you can simply turn its volume down without diving into any option menu.

## Usage
You can [download the binary](https://github.com/seb3sec/VolumeMapper/releases/download/v1.0/VolumeMapper.exe) and put it on your desktop. After startup the tool appears as an icon in the system tray (bottom right of your screen). 
You can then use hardware buttons/knobs/wheels/... which normally adjust the master volume to control application specific volume levels.

To stop the tool, just right-click on the icon and choose "exit".

This tool has been tested on Windows 10 (64 bit) with fullscreen windowed applications. 

## Features

### Volume Control
You can use your hardware volume controls to adjust the volume of the currently focused application. This works with the Alt- and Shift-Modifiers.
If you hold your Ctrl key while changing volume, this tool doesn't change the applications volume and instead Windows changes the master volume level as usual. *(This was initially a bug but I decided it's quite useful.)*

### Toggle Volume
You can press mute to toggle between two volume levels. The tool basically memorizes the current level and toggles to the previous one. By default this key toggles between 5% and whatever your current volume is.

To use two custom values do the following:
1. Adjust the applications volume to its *loud level*
2. Press **MUTE**
3. Adjust the applications volume to its *quiet level*
2. Press **MUTE** again
Now pressing the mute key toggles between the two levels.

## Bugs
If you find any bugs you can report them using the issues tab. Due to different hardware/software it might be hard to track them down. Including as much information as possible may be helpful.

## Credits
Thanks to Mark Zitnik for his [SysTrayDemo on CodeProject](https://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win).
