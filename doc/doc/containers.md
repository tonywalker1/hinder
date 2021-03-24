# Reference Containers

To help you build and/or help contribute to the hinder library, I have provided several reference
containers that have everything you need to build this library. You can also use these as a basis
for your own containers and development projects.

The [container directory](../../container) currently has support for:

* Debian 10 (Buster)
* Debian 11 (Bullseye)
* Ubuntu 20.04 (Focal; LTS)

## Managing Containers

There are two scripts to manage containers: *build_container.sh* and *run_container.sh*.

```bash
    # <container_name> is where the Dockerfile lives. See the debian-* directories for examples.
    # Because container name is actually a subdirectory in the container directory, you can see what
    # to type. 
    # Example:
    #     cd container 
    #     ./build_container.sh debian-bullseye
    build_container.sh <container_name>
    
    # <container_name> is the same as above and <path_to_hinder_source> will almost certainly be "..".
    # Example: 
    #     cd container 
    #     ./run_container.sh debian-bullseye ..
    run_container.sh <container_name> <path_to_hinder_source>
```

To build an image of any reference container, you will need Docker or Podman (I use Podman).

## Using a Container

When you start a container using one of the reference images via _run_container.sh_, the
hinder source directory is automatically mounted under _/hinder_ and the build directory is mounted
under _/build_.

Note:

1. /hinder is a bind mount, so you are mounting the underlying source (not working with a copy in
   the container image).

2. /build is a volume mount, so it is stored separately and changes survive after the container is
   stopped. When you start a new instance, the /build directory is in whatever state it was left.

```bash
cd container
./run_container.sh debian-bullseye .. 

# you will be left at a prompt in the source directory (/hinder)
ll  # you should see the hinder source
# the build directory is /build
ll /build  # will be empty on your first run

# you can build and test all configurations for this container
# note: this script will create a lot of subdirectories and build every configuration for gcc 
#       and clang
# rebuild-all.sh is copied to the container image when it is built (/usr/local/bin/rebuild-all.sh)
rebuild-all.sh

# or, you can build with your own config
cd /build
cmake <options> -S /hinder -B .
make -j
ctest
```

**Have fun!**

