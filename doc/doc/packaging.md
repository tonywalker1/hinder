# Packaging

hinder can be packaged as installable system packages (RPM or DEB) using CPack.

## Packages

Two packages are produced, following Linux library naming conventions:

| Package       | Format | Contents                                         |
|---------------|--------|--------------------------------------------------|
| `hinder`      | RPM    | `libhinder.so.0`, `libhinder.so.0.1.0`          |
| `libhinder0`  | DEB    | `libhinder.so.0`, `libhinder.so.0.1.0`          |
| `hinder-devel`| RPM    | headers under `/usr/include/hinder/`, CMake config files |
| `libhinder-dev`| DEB   | headers under `/usr/include/hinder/`, CMake config files |

Install the runtime package to run applications linked against hinder. Install the devel package to
build applications that use hinder.

## Prerequisites

**RPM (Fedora/RHEL/openSUSE):**

```bash
sudo dnf install rpm-build
```

**DEB (Debian/Ubuntu):**

```bash
sudo apt install dpkg-dev
```

## Quick Build

The helper script configures, builds, and generates both package types:

```bash
bash scripts/build-packages.sh
```

Packages are written to `build/packages/` by default. Pass an alternate directory as the first
argument:

```bash
bash scripts/build-packages.sh /tmp/hinder-pkgs
```

## Manual Build

```bash
# Configure (tests disabled — not needed for packaging)
cmake -B build/packages -DCMAKE_BUILD_TYPE=Release -DHINDER_WITH_TESTS=OFF

# Build
cmake --build build/packages -j$(nproc)

# Generate RPM packages
cd build/packages && cpack -G RPM

# Generate DEB packages
cd build/packages && cpack -G DEB
```

## Package Contents

Inspect packages without installing:

```bash
# RPM
rpm -qpl build/packages/hinder-0.1.0-1.*.rpm
rpm -qpl build/packages/hinder-devel-0.1.0-1.*.rpm

# DEB
dpkg -c build/packages/libhinder0_0.1.0_*.deb
dpkg -c build/packages/libhinder-dev_0.1.0_*.deb
```

Expected runtime package contents:

```
/usr/lib64/libhinder.so.0
/usr/lib64/libhinder.so.0.1.0
```

Expected devel package contents:

```
/usr/include/hinder/...
/usr/lib64/cmake/hinder/hinderConfig.cmake
/usr/lib64/cmake/hinder/hinderConfigVersion.cmake
/usr/lib64/cmake/hinder/hinderTargets.cmake
```

## Install and Uninstall

**RPM:**

```bash
# Install
sudo rpm -ivh build/packages/hinder-0.1.0-1.*.rpm
sudo rpm -ivh build/packages/hinder-devel-0.1.0-1.*.rpm

# Uninstall
sudo rpm -e hinder-devel hinder
```

**DEB:**

```bash
# Install
sudo dpkg -i build/packages/libhinder0_0.1.0_*.deb
sudo dpkg -i build/packages/libhinder-dev_0.1.0_*.deb

# Uninstall
sudo dpkg -r libhinder-dev libhinder0
```
