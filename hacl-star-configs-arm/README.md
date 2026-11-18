# Building HACL* on NVIDIA Jetson ARM on a Ubuntu x86 Machine

1. Copy `config.h` and `Makefile.config` to `dist/gcc-compatible` in the `hacl-start` Git submodule.

2. Run `make -j CC=aarch64-linux-gnu-gcc AR=aarch64-linux-gnu-ar` to build HACL*. **Do not run `../configure`, as doing so will overwrite `config.h` and `Makefile.config` to set the x86 build configurations**.