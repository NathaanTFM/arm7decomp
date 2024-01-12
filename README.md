# arm7decomp
decomp of the arm7 binary (mongoose)

So far I'm only reversing the wifi driver (NitroWireless/Marionea). Current names for I/O ports are from nocash (gbatek).

- twlsdk 5.5 mongoose_sub.sbin using types and names from mongoose_sub.nef

- marionea 2.88.00 which is used by many games and seems to be the latest version of the wifi driver

- not meant to be compiled back, only meant to figure out the driver works. still trying to make a 1:1 replica of every function, tho

## Contributing

You need mongoose_sub.sbin and mongoose_sub.nef from TwlSDK 5.5 (TwlSDK/components/mongoose/ARM7-TS/Debug)

Put those two files in the working directory and run FuncDumper.py: this will extract the compiled functions in the subprograms directory.

Run `python3 Compiler.py -s` to see every inaccurate function ; run `python3 Compiler.py -e` to see every missing function. Passing no parameter will output all functions. You can also filter a specific C file by using `-p filename`.

Assuming you have `arm-none-eabi-objdump` in your path, you can disassemble the functions and compare them by typing the name of the function you wanna disassemble as a parameter. For example, `python3 Compiler.py -p WlNic WCheckTxBuf` will disassemble WCheckTxBuf from the file WlNic.c.

The following files are deprecated but kept in the repository for now:
- `include/Mongoose.h` used to include common preprocessor definitions and included every other header file. It has been splitted into multiple files and replaced by `marionea/Marionea.h`.
- `include/Prototypes.h` contains prototypes for every function in the binary, which includes functions we're not interested in.
- `include/Structures.h` contains every structure in the binary, which includes structures we don't use.
- `include/Registers.h` has been replaced by `marionea/Marionea.h`.

## What's left?

These functions have an inaccurate implementation
- [RequestCmdTask](https://decomp.me/scratch/Xdhok) certainly behaves correctly, but does not compile the same.
- [WlIntrRxEndTask](https://decomp.me/scratch/UHoXL) has multiple register swapped, but should behave the same.
- [CheckFrameTimeout](https://decomp.me/scratch/9KvUP) wants my death. I don't know how long they tortured the compiler to get this ; a normal compiler shouldn't output a branch instruction to the next instruction. 

These functions don't have any implementation in the repository
- [WlIntrRxEnd](https://decomp.me/scratch/3CheC) is a mess. If you manage to make it accurate, you're my hero. (Stack allocation is 100% wrong)
- [DiagBaseBand](https://decomp.me/scratch/8cHjU) is not really important, but we need it if we ever want to get to the 100%.
- [DEV_TestSignalReqCmd](https://decomp.me/scratch/w1lZs) is also not important?

**All the remaining functions fully compile to the same bytecode.** Some of those have ugly branching and gotos ; if you manage to make these look better without breaking them, feel free to open a merge request!

The WM API part is fully missing and contains the wireless play protocol. The Compiler.py script currently ignores it.