# Kirikiri for Nintendo Switch / ニンテンドースイッチ用の吉里吉里

This repository contains a Kirikiri-compatible (often mistranslated as Yoshiri Yoshizato) engine ported to the Nintendo Switch. Code from [Kirikiri2 / 吉里吉里2](https://sv.kikyou.info/trac/kirikiri), [KirikiriZ / 吉里吉里Z](https://github.com/krkrz/krkrz), [Kirikiroid2](https://github.com/zeas2/Kirikiroid2), and [mkrkr](https://github.com/zhangguof/mkrkr) was used for this project.

## Building

After installing the preresiqute libraries using `dkp-pacman`, a simple `make` will generate `krkrs.nro`.

## RomFS Integration

To integrate the game into one single `nro` file, uncomment line 39 of `Makefile`, place game files in a directory named `romfs`, and build as described in the "Building" section.

## Project Directory Selection

If `startup.tjs` exists in RomFS, the project directory will be set to `romfs:/`. If the aforementioned condition is not met, the project directory will be set to the current directory.

## License

This project is licensed under a modified BSD license. Please read the `LICENSE` file for more information.
