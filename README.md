# arm7decomp
decomp of the arm7 binary (mongoose)

So far I'm only reversing the wifi driver (NitroWireless/Marionea). Current names for I/O ports are from nocash (gbatek).

- twlsdk 5.5 mongoose_sub.sbin using types and names from mongoose_sub.nef

- marionea 2.88.00 which is used by many games and seems to be the latest version of the wifi driver

- not meant to be compiled back, only meant to figure out the driver works. still trying to make a 1:1 replica of every function, tho

## What's left?

These functions have an inaccurate implementation
- [RequestCmdTask](https://decomp.me/scratch/Xdhok) certainly behaves correctly, but does not compile the same.
- [CheckFrameTimeout](https://decomp.me/scratch/9KvUP) wants my death. A normal compiler wouldn't output anything like this. A normal compiler wouldn't output a branch instruction to the next instruction. 

These functions don't have any implementation in the repository
- [WlIntrRxEnd](https://decomp.me/scratch/3CheC) is a mess. If you manage to make it accurate, you're my hero. (Stack allocation is 100% wrong)
- [DiagBaseBand](https://decomp.me/scratch/8cHjU) is not really important, but we need it if we ever want to get to the 100%.
- [DEV_TestSignalReqCmd](https://decomp.me/scratch/w1lZs) is also not important?

The WM API part is fully missing and contains the wireless play protocol. The Compiler.py script currently ignores it.