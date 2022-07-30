# FrameworkExample
A fully-featured training software for Counter Strike: Global Offensive, made for Linux with [Framework](https://github.com/Sumandora/Framework)
### WARNING: This program is being detected by Valve. Usage on official, custom or private servers will most likely result in punishment from Valve!
## Vulkan branch
This is a test for supporting opengl and vulkan at the same time, by hooking dx9 functions from the game  
Right now this does not compile, because ImGui expects `d3d9.h` to be present, which is not the case for Linux machines  
A wrapper is probably needed, but this has not been done yet
## Installation
### Dependencies
**Ubuntu / Debian**
```bash
apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev patchelf libglfw3-dev 
```
**Arch Linux**
```bash
pacman -S base-devel cmake gdb git sdl2 patchelf glfw-x11
```
**Fedora**
```bash
dnf install cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel libX11-devel patchelf
```
**Gentoo**
```sh
emerge cmake dev-vcs/git gdb libsdl2 mesa patchelf sys-devel/binutils
```
These might be outdated, in that please open an issue or a pull request, which updates the package names.

### Download
Make sure to download the submodules.
```sh
git clone --recurse-submodules https://github.com/Sumandora/FrameworkExample.git
```

The build script can be used to build the program.
```sh
./Build.sh
```
If you want to speed up the compilation you can use multiple threads to build the program.
```sh
MAKEOPTS="-j2" ./Build.sh
```
Make sure that your CPU/Memory is capable of doing this.  
Usually memory consumption will be around twice the number of threads you provide in gigabytes.  
If you dont know how many threads you have you can execute the `nproc` program.

**Gentoo Users do not have to do this, because the script will take the settings you specified in your `make.conf`.  
If this behaviour is unwanted then you can supply MAKEOPTS aswell.**

A ELF-Binary will be built in the "Build"-subdirectory, which will be created by the script.
## Usage
A simple inject script using a debugger like GDB or LLDB is provided.
```sh
./Load.sh
```
In case the default debugger (GDB) is not present, the script will error out.  
Use the `DEBUGGER` variable to set it to something you have.  
```sh
DEBUGGER=lldb ./Load.sh
```

The same is true for your 'run as root'-program.  
If you don't use `sudo` you can set the `SU` variable.  
```sh
SU=doas ./Load.sh
```
The script is not going to use the program if already root.

Make sure to use the script in a state, where the game is not changing frequently.  
- Don't inject before you can see the main menu
- Don't inject when you are loading a level

### Contribution
Contributions are welcome  
Issues and Pull Requests can help improve the program

#### Setting up a workspace (Example, your setup can differ)
- Fork the repository
```sh
git clone --recurse-submodules [URL] # Replace URL with your forked repository
./Build.sh # Check for initial build errors
** Make changes **
./Debug.sh # The debug script will build and load a debug build, which can also be analyzed using a debugger of your choice

git add file1 file2 # If you added new files, you have to mark them to be tracked, if you didn't add any files, you can skip this step.
git commit -a # Make a commit with all changed files. You may have to set the 'EDITOR' variable, because you have to write a commit message. Please write a small and compact message explaining what you have done.
git push
```
You should have pushed your changes to your fork  
If you want to propose the changes in the mainline release,  
you can open a pull request on GitHub
