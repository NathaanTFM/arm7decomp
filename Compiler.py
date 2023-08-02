"""
this script was never meant to be clean
"""

import os
import sys
import subprocess
from elftools.elf.elffile import ELFFile

from colorama import just_fix_windows_console, Fore, Back, Style
just_fix_windows_console()

COMPILER = "compiler/dsi/1.2p2/mwccarm.exe"
OBJDUMP = r"C:\msys64\opt\devkitpro\devkitARM\bin\arm-none-eabi-objdump.exe"

if not os.path.isfile(OBJDUMP):
    OBJDUMP = "arm-none-eabi-objdump"
    
disasmQueue = []
    
def disassemble(name, rawAsm, rawSrc):
    with open("rawAsm.bin", "wb") as f:
        f.write(rawAsm)
    with open("rawSrc.bin", "wb") as f:
        f.write(rawSrc)
        
    outputAsm = subprocess.check_output([OBJDUMP, "-D", "-b", "binary", "-marm", "rawAsm.bin"])
    outputSrc = subprocess.check_output([OBJDUMP, "-D", "-b", "binary", "-marm", "rawSrc.bin"])
    
    outputAsm = outputAsm.decode().replace("\r", "").replace("\t", " ").split("\n")
    outputSrc = outputSrc.decode().replace("\r", "").replace("\t", " ").split("\n")
    
    LENGTH = 55
    print("  " + "-" * (LENGTH*2 + 3))
    print("  " + name.center(LENGTH*2 + 3))
    print("  " + "-" * (LENGTH*2 + 3))
    print("  " + "ORIGINAL".center(LENGTH) + " | " + "SOURCE".center(LENGTH))
    
    mismatch = False
    for n in range(7, max(len(outputAsm)-1, len(outputSrc)-1)):
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
                except Exception:
                    pass
                    
            print(icon + " " + lineAsm.ljust(LENGTH) + " | " + lineSrc.ljust(LENGTH) + Style.RESET_ALL)
            
    if not mismatch:
        print("(no mismatch found)")
        
    print()
    
def compile(path, target):
    global disasmQueue
    
    isRecompiled = False
    
    if not os.path.isfile(target) or os.path.getmtime(target) < os.path.getmtime(path) or "-f" in sys.argv:
        isRecompiled = True
        
        if os.path.isfile(target):
            os.remove(target)
        
        subprocess.call([
            COMPILER,
            "-o", target,
            "-i", "include",
            "-ipa", "file",
            
            "-O4",
            "-inline", "on,noauto",
            "-opt", "speed",
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
                subprog = "subprograms/" + os.path.basename(path) + "/" + name + ".bin"
                if os.path.isfile(subprog):
                    rawSrc = elf.get_section(shndx).data()[addr:addr+size]
                    rawAsm = open(subprog, "rb").read()
                    
                    if name in sys.argv:
                        disasmQueue.append((name, rawAsm, rawSrc))
                    
                    if len(rawSrc) != len(rawAsm):
                        results[name] = (1, len(rawAsm), len(rawSrc))#("    %-30s length mismatch (%d, %d)" % (name, len(rawSrc), len(rawAsm)))
                        continue
                        
                    for n in range(0, len(raw), 4):
                        valSrc, valAsm = rawSrc[n:n+4], rawAsm[n:n+4]
                        if valSrc != valAsm:
                            # there's a difference somewhere
                            if valSrc[3] == 0xEB and valAsm[3] == 0xEB and valSrc[:3] == b"\x00\x00\x00":
                                continue
                                
                            if valSrc == b"\x00\x00\x00\x00":
                                # relocation, probably
                                continue
                                
                            results[name] = (2, len(rawAsm), n, rawSrc[n:n+4], rawAsm[n:n+4])#("    %-30s %02d -> %r %r" % (name, n, rawSrc[n:n+4].hex(" "), rawAsm[n:n+4].hex(" ")))
                            break
                            
                    else:
                        results[name] = (0, len(rawAsm))#("    %-30s OK" % (name))
                        
                else:
                    results[name] = (3, 0) # missing .bin file
                        
                #else:
                    #print("    %-30s <unk>" % (name))
                    
    for filename in os.listdir("subprograms/" + os.path.basename(path)):
        if filename.endswith(".bin"):
            name = filename[:-4]
            if name not in results:
                rawAsm = open("subprograms/" + os.path.basename(path) + "/" + name + ".bin", "rb").read()
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
            
            
objects = []

os.makedirs("built", exist_ok=True)

if "-e" in sys.argv:
    emptyFuncs = []
    
progressPerFile = {}

for directory in ("marionea",):#, "libraries"):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if not file.endswith(".c"):
                continue
                
            path = os.path.join(root, file)
            if not isTargeted(path):
                continue
            
            target = "built/" + file[:-2] + ".o"
            results = compile(path, target)
            objects.append(target)
            
            progressPerFile[path] = [0, 0, 0, 0] # bytesOk, bytesTot, funcsOk, funcsTot
            
            for name, (status, *args) in results.items():
                if status == 0:
                    progressPerFile[path][0] += args[0]
                    progressPerFile[path][2] += 1
                    
                if status != 3:
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
    print("+-" + "-" * 30 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
    print("| %-30s | %-40s | %-8s |" % ("path", "name", "size"))
    print("+-" + "-" * 30 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
    
    for path, name, size in emptyFuncs:
        print("| %-30s | %-40s | %8d |" % (path, name, size))
        
    print("+-" + "-" * 30 + "-+-" + "-" * 40 + "-+-" + "-" * 8 + "-+")
    

for name, rawAsm, rawSrc in disasmQueue:
    disassemble(name, rawAsm, rawSrc)
    
sumBytesOk, sumBytesTot, sumFuncsOk, sumFuncsTot = 0, 0, 0, 0

progressVerbose = "File progress:\n"
for file, (bytesOk, bytesTot, funcsOk, funcsTot) in sorted(progressPerFile.items()):
    progressVerbose += "    " + file + "\n"
    progressVerbose += "        Functions: %d/%d (%.1f%%)" % (funcsOk, funcsTot, funcsOk/funcsTot * 100) + "\n"
    progressVerbose += "        Bytecode: %d/%d (%.1f%%)" % (bytesOk, bytesTot, bytesOk/bytesTot * 100) + "\n"
    progressVerbose += "\n"
    
    sumBytesOk += bytesOk
    sumBytesTot += bytesTot
    sumFuncsOk += funcsOk
    sumFuncsTot += funcsTot
    
progress = "Progress:" + "\n"
progress += "    Functions: %d/%d (%.1f%%)" % (sumFuncsOk, sumFuncsTot, sumFuncsOk/sumFuncsTot * 100) + "\n"
progress += "    Bytecode: %d/%d (%.1f%%)" % (sumBytesOk, sumBytesTot, sumBytesOk/sumBytesTot * 100) + "\n"

if "-p" not in sys.argv:
    print("\n" + progress)
    with open("PROGRESS", "w") as f:
        f.write(progress + "\n" + progressVerbose)
        