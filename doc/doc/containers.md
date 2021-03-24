# Reference Containers

I use Debian for most of my development and consider it the reference platform for the hinder
library. That doesn't mean the library will not compile on other Linux distributions, it means I
develop and test on Debian. If you have problems with your Linux distribution, please let me know by
posting a question or opening an issue. I may add containers for other distributions later. This
also does not mean that I don't care about other operating systems (e.g., FreeBSD or MacOS). I do
care, but I do not have a MacOS or Windows computer on which to develop and test. Please feel free
to help port the code to your favorite OS.

## Building a Container

The [container directory](../../container) currently has support for two Debian releases: 10 
(Buster) and 11 (Bullseye). To build an image of either, you will need Docker or Podman (I use 
Podman). 
```bash
cd hinder/container
./build_container.sh debian-11  # builds a Debian 11 container image
./run_container.sh debian-11 ..  # creates an instance of the container

# you will be in the source directory (/hinder)
ls -lAF  # you should see your source
# the build directory is /build
ls -lAF  # will be empty on your first run

# you can build and test all configurations for this container
./rebuild-debian-11.sh
```



