# psCast
A podcast client app for the PS Vita. Original logo design was made by [NotMatei](https://github.com/NotMatei). I'm using [feedpp](https://github.com/kamarya/feedpp) from [kamarya](https://github.com/kamarya) to parse the feeds for the podcasts.

# Building

## Requirements
- The PS Vita SDK, which can be installed from [here](https://vitasdk.org/). Alternatively, you can use [this](https://hub.docker.com/r/thelastbilly/vitacontained) docker image I built that has the SDK in it
- cmake
- gcc

## Building

#### Note: For those using the vitacontained docker image
*Go to the root of this repository on your terminal and use the following command to launch the vitacontained shell in it:
 `bash -c "docker run -ti -v '$PWD:/mnt/share' thelastbilly/vitacontained:latest"`. You can then move on to the steps bellow*

### Compiling
- Make a new directory (if it doesn't already exist) in the root of this repository called **build/** and `cd` into it. This is not mandatory, but it makes the build process easier and cleanner.
- Run `cmake ..` from this directory
- Run `make`

### Getting a working .vpk file
Assuming the compilation process went forward without any issues, you should have a file called **pscast.vpk** in your **build/** directory. You can now use FTP or other means to move it to your PS Vita and install it using [VitaShell](https://github.com/TheOfficialFloW/VitaShell/releases) or other tools. 

#### Note: For those using the vitacontained docker image
You may need to use the following comand on the **build/** directory in order to modify the files inside it: `sudo chown $USER:$USER -R build/` (from the root of this repository). This is because the docker image is running as root by default, so the files are owned by root when they're first created. You shouldn't have to repeat this step again if you don't delete the **build/** folder.