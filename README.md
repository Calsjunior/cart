# cart

Cart is a TUI (Terminal User Interface) file manager for linux-based systems that is written in C using the ncurses library.

Although this program was made for [CS50x](https://cs50.harvard.edu/x/)'s final project and as a proof of concept,
I will (hopefully) keep on learning to bring more features to cart!

<img src="/assets/cart.png">


## Table of contents
-  [Features](#features)
-  [Requirements](#requirements)
-  [Installation](#installation)
-  [Usage](#usage)
-  [Configuring Your Text Editor](#configuring-your-text-editor)
-  [Keymaps](#keymaps)

## Features
-  Simple, minimal, and clean UI layout
-  Basic navigation with arrow keys or hjkl for my fellow vim users
(more keymaps can be found [here](#keymaps))
-  Files and folders deletion
-  Files and folders creation
-  Open your favorite text editor straight from cart
-  Auto-resize to fit terminal size

## Requirements
Before installing cart, make sure you also have the following:
-  A linux-based system (tested on archlinux and macOS, but WSL on Windows work too)
-  `gcc` or `clang`
-  ncurses library
-  Any [nerd fonts](https://www.nerdfonts.com) (required for icons)

Popular choices include: `FiraCode Nerd Font, JetBrainsMono Nerd Font, Hack Nerd Font, Cascadia Nerd Font`.
Once installed, make sure your terminal uses the nerd font.

To install the dependencies:
-  Archlinux: `sudo pacman -S ncurses`
-  Debian/Ubuntu: `sudo apt install libncurses5-dev libncursesw5-dev`
-  macOS (if you use homebrew): `brew install ncurses`

## Installation
To install cart, simply clone the repo with 
```bash
git clone https://github.com/Calsjunior/cart
cd cart
sudo make install
```

To uninstall cart, run 
```bash
cd cart
sudo make uninstall
```
or
```bash
rm -rf /usr/local/bin/cart
```

## Usage 
Run `cart` in your terminal, and voila!

## Configuring Your Text Editor
**cart** can open files and folders using your preferred text editor.

You can set your editor globally in your shell config so cart and other CLI tools will use it automatically.

For **bash/zsh**
```bash
export EDITOR=nvim
```

For **fish**
```fish
set -gx EDITOR nvim
```

## Keymaps
A keymaps menu can also be found inside the program by pressing '?'

| **Keymaps**                | **Action**             |
|:--------------------------:|:----------------------:|
|  / , k/j                 | Move up/down           |
|  / , h/l                 | Move left/right        |
| PgUp/PgDn, ^U/^D           | Move half page         |
| Home/End or gg/G           | Go top/bottom          |
| g                          | Go to                  |
| a                          | Create file or folder  |
| Delete/d                   | Delete file            |
| v                          | Open text editor       |

## Inspiration
Thank you to the makers of [yazi](https://github.com/sxyazi/yazi) and [ncursesFM](https://github.com/FedeDP/ncursesFM) for the inspiration!

## License
[MIT (c) Calsjunior](LICENSE)
