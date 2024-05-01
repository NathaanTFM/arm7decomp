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
- `include/Mongoose.h` used to include common preprocessor definitions and included every other header file. It has been split into multiple files and replaced by `marionea/Marionea.h`.
- `include/Prototypes.h` contains prototypes for every function in the binary, which includes functions we're not interested in.
- `include/Structures.h` contains every structure in the binary, which includes structures we don't use.
- `include/Registers.h` has been replaced by `marionea/Marionea.h`.

## What's left?

### Marionea (Wi-Fi Driver)

These functions have an inaccurate implementation:
- [CheckFrameTimeout](https://decomp.me/scratch/9KvUP) has some branching issues, but should behave the same?
- [WlIntrRxEnd](https://decomp.me/scratch/3CheC) has a stack allocation differences, but should behave the same.
- [WlIntrRxEndTask](https://decomp.me/scratch/UHoXL) has multiple register swapped, but should behave the same.

These functions don't have any implementation in the repository
- [DiagBaseBand](https://decomp.me/scratch/8cHjU)
- [DEV_TestSignalReqCmd](https://decomp.me/scratch/w1lZs)

**All the remaining functions fully compile to the same bytecode.** Some of those have ugly branching and gotos ; if you manage to make these look better without breaking them, feel free to open a merge request!

### ARM7 WMSP (Wi-Fi API)

Inaccurate functions:
- [WMSP_DisconnectCore](https://decomp.me/scratch/E4qqZ): register swap
- [WMSP_IndicateThread](https://decomp.me/scratch/6dCWR): stack allocation differences
- WMSP_SetDCFData: two swapped instructions (moveq/movne)
- WMSP_SetGameInfo: two swapped instructions (moveq/movne)
- WMSPi_CommonInit: severe register swap
- WVR_Begin: it's different
- WmspChildAdjustVSync1: register swap for 2 instructions

Non-implemented functions:
- WMSP_FlushSendQueue
- WMSP_MeasureChannel
- WMSP_PutSendQueue
- WMSP_Reset
- WMSP_SendMaKeyData
- WMSP_SendMaMP
- WMSP_StartMP
- WMSP_StartScan
- WMSP_StartScanEx
- WmspChildAdjustVSync2
- WmspChildVAlarmMP
- WmspGetTmptt