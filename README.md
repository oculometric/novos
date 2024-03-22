# novos - a toy 'kernel'/videogame project

hiyya! this is my kernel development project. i use 'kernel' loosely since it mostly amounts to me building all the *bits* of a kernel without really following a specific kernel design or model.

## state of the project

as of writing this, i've written a reasonably okayish (if not very portable) *bootloader* for loading an elf binary, i've implemented a simple memory allocator, designed a *UI layout* mechanism and a drawing system, written some *3D graphics* library stuff, and implemented *serial logging* for debugging. some other cool things i've done are re-implementing *streams* to make serial-debugging things easier (and all the other useful things you can do with streams), and implementing *dynamic arrays and strings*. in fact, a fair bit of stuff is re-creating C++ standard library stuff.

## big bad mean todo list

- [x] dynamic memory management
- [x] fix elf loading to load non-program sections in the bootloader
- [x] basic vector & matrix maths
- [x] UI layouting system
- [x] dynamic arrays
- [x] strings
    - [ ] stream into a string
- [x] memory view UI panel
- [ ] text rendering
    - [ ] text view UI panel
- [x] 3D wireframe rendering panel able to read 3D model files
- [ ] virtual/abstract file systems
    - [ ] reading from disk, reading into virtual filesystem
    - [ ] update build system to be able to build proper filesystem images
    - [ ] split the kernel into loadable modules?????
- [ ] write interrupt handler wrapper
    - [ ] write interrupt handlers
    - [ ] write PIC reprogramming code
    - [ ] code for talking to the keyboard
    - [ ] code for talking to timers
- [ ] exception handling (needs interrupts)
    - [ ] convert unecessarry panics into throws
    - [ ] definitely separate the kernel from 'programs'
- [ ] proper memory management
    - [ ] better allocation algorithm
    - [ ] paging/virtual memory
- [ ] implement necessarry functions to re-enable v-tables and virtual functions
- [ ] keyboard input support
- [ ] timers
- [ ] multithreading
- [ ] design the game engine side, then implement it, then make it work, oh have mercy upon me please

i did say it was big bad and mean

## goal of the project

i don't have much of a concrete goal with this. to be honest, given how challenging this kind of development is (for me at least) i don't want to set myself huge goals, and you shouldn't expect this to turn into a full graphical operating system capable of loading Steam games for at least 70 years or so.

mostly the point of this is just learning doing things from the ground up, having fun implementing fundamental data structures and algorithms for myself, and having fun while suffering through that.

me being a graphics programming victim/interested-person, *the only long-term goal i have for this is creating some kind of simple 3D space-related videogame, in the sense that **the kernel is the game**. i.e. i'm writing a game engine, and a game to use it, but completely freestanding and bootable on its own*. oh yeah, and i want to keep it under about 16MB.

the other thing i do want to do is try and make *some* note about anything particular i learn during this. this is unlikely to happen substantially. something i am **absolutely** going to do is write *well-documented code* because *the number one reason building something like this is so difficult is that i can find very very few well-documented, understandable, followable code examples for this kind of project*. the [osdev wiki](https://wiki.osdev.org) is an amazing resource but, understandably, its pretty opaque if you don't already know everything on it intimately. hopefully someone else will find this useful (assuming another sufficiently stupid noob comes along like me).

## building the project

following the principle above, i'm trying to keep the build environment simple. everything that builds the project is in the Makefile, and its all just there so you can read it and actually understand the build process.

the `build_toolchain.sh` script should download and install all the necessary tools for building the project on Ubuntu, but if it doesn't work on your platform you might be able to figure out what you need manually (again, i need to get on this).

alternatively you can download prebuilt binaries from the releases tab and run them under qemu-system-x86_64/qemu-system-i386. again, the binary should stand entirely on its own.

### build with Docker

alternatively-alternatively you can use Docker to build the image.

prerequisites:
- Docker
- make*
- qemu (system-x86_64/system-i386) to run the resulting image

just run `make docker` and it should output files into `./bin/`. the first run will take a few minutes as it build the required dependencies, but subsequent runs only need to build the actual image thanks to Docker layer caching.
as a \*very* rough guide, `docker system df` reports using ~6.5GB after a build (having pruned beforehand), but that will *not* grow significantly with subsequent reruns.

additionally, the Dockerfile shows the exact steps and dependencies required on Ubuntu, so can serve as a rough guide on what tools/steps are required in other environments.

\*technically make is optional - have a look at the Makefile's `docker` recipe to see what it does (it's just being used as a command runner).
