<img src="https://github.com/R-Rudolph/OmegaRPG/blob/master/icons/omegarpg.png" width="200" height="200" /> 

# OmegaRPG
 
OmegaRPG is a free, open-source Virtual Game Tabletop application inspired by OpenRPG. It is licensed under the GNU General Public License Version 3.

## Features

* Chat with main channel, direct messages and Game Master channel
* Quick dice rolls via dice toolbar
* Using aliases and differing text colors in chat, e.g. to better differentiate between characters
* *Alias sheets* for each character, allowing for notes and more complex, prepared dice rolls. Also allows for plugins with custom functionality (currently under development).
* Different client statuses (Lurker, Player, GM) for rights management
* Whiteboard with functions for measuring distance (available to all), drawing, writing, miniature placement (available to Players and GMs) and fog of war (available only to GMs)
* Initiative menu to keep track of the order players/characters act in
* Session logs to review previous sessions
* SSL/TLS-based client-server encryption

## Installation

### Windows

Just grab a windows release zip from the [most recent release](https://github.com/R-Rudolph/OmegaRPG/releases/download/v0.5.1/omegarpg-0.5.1-i686-w64.zip) and unpack it anywhere. Then just run OmegaRPG.exe.

### Arch Linux

Sadly, no binary releases yet. But git repository contains a PKGBUILD file to build a package! Just run

```
wget https://github.com/R-Rudolph/OmegaRPG/blob/master/distribution/abs/PKGBUILD?raw=true -O PKGBUILD
makepkg -sci
```

to install.

### Other Linux distributions

Also no binary releases yet, you'll have to build from source manually:

```
git clone https://github.com/R-Rudolph/OmegaRPG
cd OmegaRPG
git submodule update --init --recursive
cd ..
mkdir build
cd build
qmake ../OmegaRPG/
make
```

### Dependencies

* [Qt 5.8](https://www.qt.io/) with modules
  * widgets
  * network
  * multimedia
  * xml
  * opengl
  * quickwidgets
  * svg
* [OpenSSL 1.1](https://www.openssl.org/)
* [zlib](https://zlib.net/)

Currenty, [QuaZIP](https://stachenov.github.io/quazip/) is compiled statically into OmegaRPG. In future versions it may be added as a dependency and linked dynamically.

## Screenshots

![OmegaRPG Screenshot](https://github.com/R-Rudolph/OmegaRPG/blob/master/screenshots/screenshot1.png?raw=true "A regular, naturally occuring OmegaRPG room")
