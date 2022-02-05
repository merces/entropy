# entropy

`entropy` is a simple command-line tool to calculate the entropy of files.

## Why

The [entropy of a file](https://kennethghartman.com/calculate-file-entropy/) tells us how random this file is.
The closer to `8.00`, the more random is an input file.

As compression and encryption algorithms output usually consists of high entropy data, one can say
any file with entropy `greater than 7.0` is likely compressed, encrypted, or packed (in case of executables).

I needed a fast way to calculate the entropy for a couple of files at once in Windows, so I ended up coding
this tool, which also supports Linux and macOS.

## Download

Windows releases are available [here](https://github.com/merces/entropy/releases). In order to run them, 
you need the latest [Microsoft Visual C++ Redistributable](docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)
installed.

For Linux and macOS see the [Building](#Building) section.

## Usage

Calculating the entropy of a single file:

    $ ./entropy /bin/ls
    5.85 /bin/ls

Shell expansion is supported too:

    PS C:\> .\entropy.exe C:\Users\User\Downloads\*
    7.92 C:\Users\User\Downloads\1.jpeg
    8.00 C:\Users\User\Downloads\setup.exe
    7.58 C:\Users\User\Downloads\nov.pptx
    4.66 C:\Users\User\Downloads\data.bin
    7.99 C:\Users\User\Downloads\pic.png
    4.07 C:\Users\User\Downloads\budget.xls
    
From the above output one could say `/bin/ls` is not packed, `1.jpeg` uses compression,
`setup.exe` is compressed, `nov.pptx` is compressed (yup, these modern MS Office files are all
ZIP files indeed), `data.bin` is not compressed, etc. Is that garuanteed? No, it's just math. :nerd_face:

## Building

### Linux/macOS

Clone the repo:

    $ git clone https://github.com/merces/entropy.git
    $ cd entropy
    
If you have CMake installed, build with:
    
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    
Or if you don't, just use `g++`:

    $ g++ -std=c++20 -o entropy entropy.cpp
    
### Windows

If you use a recent Visual Studio version, you can clone this repository and open the `CMakeLists.txt` here
with `File -> Open -> CMake...`. Then, select a configuration that suits your needs and build.

## Similar Work

I am aware of the following tools that achieve similar results:

| Name                                                                        | Comments                                                                |
| --------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| [DensityScout](https://cert.at/en/downloads/software/software-densityscout) | Uses a different algorithm the author called density.                   |
| [Detect It Easy](https://github.com/horsicq/Detect-It-Easy)                 | Shows a nice graph. Do many other great things. CLI version included.   |
| [entropy](https://github.com/dirtbags/fluffy/blob/master/entropy.c)         | From Fluffy Suite. Fast. Supports one file at time.                     |
| [Ent](https://gynvael.coldwind.pl/?id=158)                                  | Different algorithm, one file at a time. Windows only. Creates a graph. |
| [ent](https://www.fourmilab.ch/random/)                                     | Gives more information. Process one file at a time.                     |
| [rahash2](https://www.radare.org/n/radare2.html)                            | From radare2 framework. `rahash2 -a entropy` does the job.              |
| [rz-hash](https://rizin.re)                                                 | From Rizin framework. `rz-hash -a entropy` does the job.                |


