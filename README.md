# SpyCryptor

Encrypt your files (AES) for research and educational purpose

exe -> exe / exe -> dll / dll -> dll / dll -> exe

Supports x64 and x86 files

Methods: manual map injection (if source is dll) or process hollowing svchost.exe (if source is exe). 

Erases itself from Prefetch. Included timedatestamp cleaner.

Usage: SpyCryptor.exe -in \"file.exe|dll\" -out exe|dll [-target \"process.exe\"] [-console] [-genjunk] [-requireadmin]

Installed Visual Studio is required. 

Disable Defender real-time protection for .manifest support

![Screenshot](https://i.imgur.com/AxuyOaS.png)
