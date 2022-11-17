# SpyPacker

Not intended to bypass anticheats and/or antiviruses. Made for research and educational purposes only!

exe -> exe / exe -> dll / dll -> dll / dll -> exe

Payload is encrypted in AES. Supports x64 and x86 files.

Methods: manual map injection (if input is dll) or process hollowing svchost.exe (if input is exe). 

Erases itself from Prefetch. Included timedatestamp cleaner and junk code generator for stub.

Usage: SpyPacker.exe -in "file" -out exe|dll [-target "process.exe"] [-name "output.exe"|random] [-console] [-genjunk] [-requireadmin]

Installed Visual Studio is required. Disable Defender real-time protection for .manifest support

# Examples:
SpyPacker.exe -i calc.exe -o dll -c -g

SpyPacker.exe -i osiris.dll -o exe -t "csgo.exe" -m

![Screenshot](https://i.imgur.com/lobWIGn.png)

Credits: @kokke, @waleedassar, @UserExistsError, @Zer0Mem0ry, hitech_crypter, etc..
