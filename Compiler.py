"""
this script was never meant to be clean
"""

import os
import sys
import subprocess
import re
from elftools.elf.elffile import ELFFile

from colorama import just_fix_windows_console, Fore, Back, Style
just_fix_windows_console()

COMPILER = "compiler/dsi/1.2p2/mwccarm.exe"
OBJDUMP = r"C:\msys64\opt\devkitpro\devkitARM\bin\arm-none-eabi-objdump.exe"

if not os.path.isfile(OBJDUMP):
    OBJDUMP = "arm-none-eabi-objdump"
    
disasmQueue = []

def parseOutput(output):
    result = []
    
    lineRegex = re.compile(r"^\s*([0-9a-fA-F]+)\s*\:\s*([0-9a-fA-F]{8})\s*(.*)$")
    
    for n in range(len(output)):
        match = lineRegex.match(output[n])
        if match:
            addr, value, instr = match.groups()
            addr = int(addr, 16)
            if addr & 3 != 0:
                raise Exception("expected ARM and 4 bytes aligned address")
            
            addr //= 4
            if addr < len(result):
                print("oof")
            elif addr > len(result):
                for m in range(len(result), addr):
                    result.append("---")
                
            result.append(output[n])
            
    return result
    
    
def regswapChecker(asm, src):
    REGISTERS = ("r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "sl", "fp", "ip", "sp", "lr", "pc")
    
    def trimRegisters(line):
        for reg in REGISTERS:
            line = re.sub(r"\b" + reg + r"\b", "(reg)", line)
            
        return line
        
    def findRegisters(line):
        return re.findall(r"\b(" + "|".join(REGISTERS) + r")\b", line)
        
    # check if it's a regswap (remove registers, if it matches, it's most certainly a regswap)
    if trimRegisters(asm) != trimRegisters(src):
        return False 
        
    # TODO: figure out what registers have been swapped
    registers_asm = findRegisters(asm)
    registers_src = findRegisters(src)
    
    if len(registers_asm) != len(registers_src):
        return False
        
    # (code goes here)    
    return True
    
    
def disassemble(name, rawAsm, rawSrc):
    with open("rawAsm.bin", "wb") as f:
        f.write(rawAsm)
    with open("rawSrc.bin", "wb") as f:
        f.write(rawSrc)
        
    outputAsm = subprocess.check_output([OBJDUMP, "-D", "-b", "binary", "-marm", "rawAsm.bin"])
    outputSrc = subprocess.check_output([OBJDUMP, "-D", "-b", "binary", "-marm", "rawSrc.bin"])
    
    outputAsm = outputAsm.decode().replace("\r", "").replace("\t", " ").split("\n")
    outputSrc = outputSrc.decode().replace("\r", "").replace("\t", " ").split("\n")
    
    outputAsm = parseOutput(outputAsm)
    outputSrc = parseOutput(outputSrc)
    
    LENGTH = 55
    print("  " + "-" * (LENGTH*2 + 3))
    print("  " + name.center(LENGTH*2 + 3))
    print("  " + "-" * (LENGTH*2 + 3))
    print("  " + "ORIGINAL".center(LENGTH) + " | " + "SOURCE".center(LENGTH))
    
    mismatch = False
    for n in range(max(len(outputAsm), len(outputSrc))):
        lineAsm = "<none>"
        lineSrc = "<none>"
        if n < len(outputAsm):
            lineAsm = outputAsm[n].strip()
        if n < len(outputSrc):
            lineSrc = outputSrc[n].strip()
            
        if lineAsm != lineSrc:
            mismatch = True
            
        if mismatch:
            icon = Fore.RED + " "
            if lineAsm == lineSrc:
                icon = Fore.LIGHTBLACK_EX + "="
                
            else:
                # lazy!!!
                try:
                    if lineAsm.split()[2] == lineSrc.split()[2]:
                        icon = Fore.WHITE + "*"
                        
                        if lineSrc.split()[1] == "eb000000":
                            icon = Fore.LIGHTBLACK_EX + "~"
                            
                        else:
                            # Check for regswap
                            if regswapChecker(lineAsm.split(None, 2)[2], lineSrc.split(None, 2)[2]):
                                icon = Fore.CYAN + "r"
                            
                except Exception as e:
                    pass
                    
            print(icon + " " + lineAsm.ljust(LENGTH) + " | " + lineSrc.ljust(LENGTH) + Style.RESET_ALL)
            
    if not mismatch:
        print("(no mismatch found)")
        
    print()
    
def compile(path, target, internal):
    global disasmQueue
    
    isRecompiled = False
    
    if not os.path.isfile(target) or os.path.getmtime(target) < os.path.getmtime(path) or "-f" in sys.argv:
        isRecompiled = True
        
        if os.path.isfile(target):
            os.remove(target)
        
        extra_flags = []
        if internal == "TWL":
            extra_flags.append("-DTWL_MODE")
            
        subprocess.call([
            COMPILER,
            "-o", target,
            "-Iinclude",
            "-Imarionea",
            "-ipa", "file",
            *extra_flags,
            
            "-O4",
            "-inline", "on,noauto",
            "-opt", "space",
            "-g",
            "-proc", "arm7tdmi",
            "-msgstyle", "gcc", "-gccinc",
            "-fp", "soft",
            "-lang", "c99",
            "-Cpp_exceptions", "off",
            "-interworking",
            "-enum", "int",
            "-r",
            "-W", "noimpl_signedunsigned",
            "-w", "notinlined",
            "-c",
            path
            
        ], env={"LM_LICENSE_FILE": "compiler/license.dat"})
    
    #if "-s" not in sys.argv or isRecompiled:
    #    print(path)
        
    with open(target, "rb") as f:
        raw = f.read()
        
    results = {}
    
    with open(target, "rb") as f:
        elf = ELFFile(f)
        #dwarfinfo = elf.get_dwarf_info()
        
        symtab = elf.get_section_by_name('.symtab')
        for symbol in symtab.iter_symbols():
            addr = symbol.entry.st_value
            shndx = symbol.entry.st_shndx
            name = symbol.name
            size = symbol.entry.st_size
            info = symbol.entry.st_info
            if info["type"] == "STT_FUNC":
                subprog = "subprograms/" + os.path.basename(path) + "/" + name + "/" + internal + ".bin"
                if os.path.isfile(subprog):
                    rawSrc = elf.get_section(shndx).data()[addr:addr+size]
                    rawAsm = open(subprog, "rb").read()
                    
                    if name in sys.argv:
                        disasmQueue.append((name, rawAsm, rawSrc))
                    
                    if len(rawSrc) != len(rawAsm):
                        results[name] = (1, len(rawAsm), len(rawSrc))#("    %-30s length mismatch (%d, %d)" % (name, len(rawSrc), len(rawAsm)))
                        continue
                        
                    nonMatching = 0
                    firstMismatch = None
                    
                    for n in range(0, len(raw), 4):
                        valSrc, valAsm = rawSrc[n:n+4], rawAsm[n:n+4]
                        if valSrc != valAsm:
                            # there's a difference somewhere
                            if valSrc[3] == 0xEB and valAsm[3] == 0xEB and valSrc[:3] == b"\x00\x00\x00":
                                continue
                                
                            if valSrc == b"\x00\x00\x00\x00":
                                # relocation, probably
                                continue
                                
                            if firstMismatch is None:
                                firstMismatch = n
                                
                            nonMatching += 4
                        
                    if firstMismatch is not None:
                        results[name] = (2, len(rawAsm), firstMismatch, rawSrc[firstMismatch:firstMismatch+4], rawAsm[firstMismatch:firstMismatch+4], len(rawAsm) - nonMatching)#("    %-30s %02d -> %r %r" % (name, n, rawSrc[n:n+4].hex(" "), rawAsm[n:n+4].hex(" ")))
                        
                    else:
                        results[name] = (0, len(rawAsm))#("    %-30s OK" % (name))
                        
                else:
                    results[name] = (3, 0) # missing .bin file
                        
                #else:
                    #print("    %-30s <unk>" % (name))
                    
    parent = "subprograms/" + os.path.basename(path)
    for filename in os.listdir(parent):
        if os.path.isfile(parent + "/" + filename + "/" + internal + ".bin"):
            name = filename
            if name not in results:
                rawAsm = open(parent + "/" + filename + "/" + internal + ".bin", "rb").read()
                results[name] = (4, len(rawAsm)) # missing function
                
                
    return results


def isTargeted(path):
    if "-p" in sys.argv:
        for arg in sys.argv:
            if arg in path:
                break
        else:
            return False
            
    return True


for internal in ("NITRO", "TWL"):
    objects = []

    os.makedirs("built/" + internal, exist_ok=True)

    if "-e" in sys.argv:
        emptyFuncs = []
        
    progressPerFile = {}

    for directory in ("marionea", "libraries"):
        for root, dirs, files in os.walk(directory):
            for file in files:
                if not file.endswith(".c"):
                    continue
                    
                path = os.path.join(root, file)
                if not isTargeted(path):
                    continue
                
                target = "built/" + internal + "/" + file[:-2] + ".o"
                results = None
                
                tmp = re.split(r"[/\\]", path)
                if internal == "NITRO":
                    if "wpa" in tmp or "ARM7.TWL" in tmp or "common.TWL" in tmp or "tianshan" in tmp:
                        continue
                        
                elif internal == "TWL":
                    if "wvr_sp.c" in tmp and "ARM7" in tmp:
                        continue
                        
                results = compile(path, target, internal)
                    
                objects.append(target)
                
                progressPerFile[path] = [0, 0, 0, 0] # bytesOk, bytesTot, funcsOk, funcsTot
                
                for name, (status, *args) in results.items():
                    if status == 0: # OK
                        progressPerFile[path][0] += args[0]
                        progressPerFile[path][2] += 1
                        
                    elif status == 2: # mismatch
                        progressPerFile[path][0] += args[4] # these are the identical bytes
                        
                    if status != 3: # (missing .bin file)
                        progressPerFile[path][1] += args[0]
                        progressPerFile[path][3] += 1
                
                if "-e" in sys.argv:
                    # sort the interesting results per size
                    for name, (status, *args) in sorted(results.items()):
                        if (status == 1 and args[1] == 4):
                            emptyFuncs.append((path, name, args[0]))
                            
                        elif status == 4:
                            emptyFuncs.append((path, name, args[0]))
                        
                        
                else:
                    # just print the results
                    flag = 0
                    
                    for name, (status, *args) in sorted(results.items()):
                        if name[0] == "$":
                            continue
                            
                        if "-s" in sys.argv and (status == 0 or (status == 1 and args[1] == 4) or status == 4):
                            continue
                            
                        # only print path if there's any function to print
                        if not flag:
                            flag = 1
                            print(path)
                            
                        print("    " + name.ljust(39), end=" ")
                        if status == 0:
                            print("OK")
                            
                        elif status == 1: # length mismatch
                            print("length mismatch (%d, %d)" % (args[1], args[0]))
                            
                        elif status == 2: # incorrect instr
                            print("incorrect (%02Xh -> %r - %r)" % (args[1], args[2].hex(" "), args[3].hex(" ")))
                            
                        elif status == 3: # missing .bin
                            print("no original")
                            
                        elif status == 4: # missing func
                            print("missing (%d)" % (args[0],))
                            
                        else:
                            print("(Status %d)" % status)
            
    if "-e" in sys.argv:
        emptyFuncs.sort(key = lambda x: (x[2], x[1]))
        print("+-" + "-" * 50 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
        print("| %-50s | %-40s | %-8s |" % ("path", "name", "size"))
        print("+-" + "-" * 50 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
        
        for path, name, size in emptyFuncs:
            print("| %-50s | %-40s | %8d |" % (path, name, size))
            
        print("+-" + "-" * 50 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
        
    sumBytesOk, sumBytesTot, sumFuncsOk, sumFuncsTot = 0, 0, 0, 0

    progressVerbose = "File progress:\n"
    for file, (bytesOk, bytesTot, funcsOk, funcsTot) in sorted(progressPerFile.items()):
        progressVerbose += "    " + file + "\n"
        progressVerbose += "        Functions: %d/%d (%.1f%%)" % (funcsOk, funcsTot, funcsOk/(funcsTot or 1) * 100) + "\n"
        progressVerbose += "        Bytecode: %d/%d (%.1f%%)" % (bytesOk, bytesTot, bytesOk/(bytesTot or 1) * 100) + "\n"
        progressVerbose += "\n"
        
        sumBytesOk += bytesOk
        sumBytesTot += bytesTot
        sumFuncsOk += funcsOk
        sumFuncsTot += funcsTot
        
    progress = "Progress:" + "\n"
    progress += "    Functions: %d/%d (%.1f%%)" % (sumFuncsOk, sumFuncsTot, sumFuncsOk/(sumFuncsTot or 1) * 100) + "\n"
    progress += "    Bytecode: %d/%d (%.1f%%)" % (sumBytesOk, sumBytesTot, sumBytesOk/(sumBytesTot or 1) * 100) + "\n"

    if "-p" not in sys.argv:
        print("\n" + progress)
        with open("PROGRESS.TWL" if internal == "TWL" else "PROGRESS", "w") as f:
            f.write(progress + "\n" + progressVerbose)
            
for name, rawAsm, rawSrc in disasmQueue:
    disassemble(name, rawAsm, rawSrc)