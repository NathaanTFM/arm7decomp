from elftools.elf.elffile import ELFFile
import os

# Relocations:
    # begin     end         file offset
    # 27E0000   27F6F4C     11A18
    # 37F8000   3809684     394
    
    
def getFileOffset(addr):
    if 0x27E0000 <= addr < 0x27F6F4C:
        return addr - 0x27E0000 + 0x11A18
    elif 0x37F8000 <= addr < 0x3809684:
        return addr - 0x37F8000 + 0x394
    elif 0x2380000 <= addr < 0x23a897c:
        return addr - 0x2380000
        
    raise ValueError("Unknown section for address %08x" % (addr))    
    
MARIONEA = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\add-ins\NitroWireless\Marionea-2.88.00\WL\src"
LIBRARY = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries"
LIBRARY_WM = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries\wm"
LIBRARY_WVR = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries\wvr"
MONGOOSE = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\components\mongoose\src"

fps = {}

os.makedirs("subprograms", exist_ok=True)

with open("mongoose_sub.sbin", "rb") as file:
    raw = file.read()
    
with open("mongoose_sub.nef", "rb") as file:    
    elf = ELFFile(file)
    dwarfinfo = elf.get_dwarf_info()
    
    for CU in dwarfinfo.iter_CUs():
        line_program = dwarfinfo.line_program_for_CU(CU)
        
        top_DIE = CU.get_top_DIE()
        path = top_DIE.get_full_path()
        if path.startswith(MARIONEA):
            path = path[len(MARIONEA):].lstrip("\\/")
            path = "marionea/" + path
            
        elif path.startswith(LIBRARY_WM):
            path = path[len(LIBRARY_WM):].lstrip("\\/")
            path = "libraries/wm/" + path
            
        elif path.startswith(LIBRARY_WVR):
            path = path[len(LIBRARY_WVR):].lstrip("\\/")
            path = "libraries/wvr/" + path
        #
        #elif path.startswith(LIBRARY):
        #    path = path[len(LIBRARY):].lstrip("\\/")
        #    path = "libraries/" + path
        #    
        #elif path.startswith(MONGOOSE):
        #    path = path[len(MONGOOSE):].lstrip("\\/")
        #    path = "src/" + path
            
        else:
            print("unk", path)
            path = None
            continue
            
        for DIE in CU.iter_DIEs():
            if DIE.tag == "DW_TAG_subprogram":
                declFile = DIE.attributes["DW_AT_decl_file"].value
                declFile = line_program.header["file_entry"][declFile - 1].name.decode()
                declFile = os.path.basename(declFile) # probably useless
                
                addr = (DIE.attributes['DW_AT_low_pc'].value, DIE.attributes['DW_AT_high_pc'].value)
                name = DIE.attributes["DW_AT_name"].value.decode()
                
                static = ("DW_AT_external" not in DIE.attributes) or (not DIE.attributes["DW_AT_external"].value)
                #if static:
                filename = declFile + "/" + name + ".bin"
                #else:
                #    filename = name + ".bin"
                    
                off = (getFileOffset(addr[0]), getFileOffset(addr[1]))
                    
                if path is not None:
                    os.makedirs(os.path.dirname(path), exist_ok=True)
                    if path not in fps and not os.path.isfile(path):
                        fps[path] = open(path, "w")
                            
                    if path in fps:
                        f = fps[path]
                        f.write("// " + name + "\n")
                
                    os.makedirs("subprograms/" + os.path.dirname(filename), exist_ok=True)
                    with open("subprograms/" + filename, "wb") as f:
                        f.write(raw[off[0]:off[1]])
                        
            """elif DIE.tag == "DW_TAG_variable":
                location = DIE.attributes["DW_AT_location"].value
                if location[0] == 3:
                    addr = (location[4] << 24) | (location[3] << 16) | (location[2] << 8) | location[1]
                    try:
                        addr = getFileOffset(addr)
                    except ValueError:
                        continue
                        
                    print(hex(addr))"""
                    
for f in fps.values():
    f.close()