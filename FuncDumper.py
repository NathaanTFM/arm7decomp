from elftools.elf.elffile import ELFFile
import os

MARIONEA = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\add-ins\NitroWireless\Marionea-2.88.00\WL\src"
LIBRARY = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries"
LIBRARY_WM = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries\wm"
LIBRARY_WVR = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries\wvr"
LIBRARY_WPA = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\libraries\wpa"
MONGOOSE = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\build\components\mongoose\src"
TIANSHAN = r"D:\pseg1\dev\autobuild\TwlSDK\branch-5_5\working\TwlSDK\add-ins\TwlWireless\TianShan_080602"

paths = [MARIONEA, TIANSHAN, LIBRARY_WM, LIBRARY_WVR, LIBRARY_WPA]

class ElfInfo:
    def __init__(self, filename, internal):
        self.__filename = filename
        self.__internal = internal
        self.__relocations = []
        self.__binfiles = {}
        
        
    def add_relocation(self, binfile, offset, addr, length):
        if binfile not in self.__binfiles:
            with open(binfile, "rb") as f:
                data = f.read()
                
            self.__binfiles[binfile] = data
            
        self.__relocations.append((binfile, offset, addr, length))
        
        
    def get_data(self, address, length):
        for rel in self.__relocations:
            if (rel[2] <= address < rel[2] + rel[3]) and (rel[2] < address + length <= rel[2] + rel[3]):
                return self.__binfiles[rel[0]][rel[1] + address - rel[2] : rel[1] + address - rel[2] + length]
                
        return None
        
        
    def get_filename(self):
        return self.__filename
        
        
    def get_internal(self):
        return self.__internal


nitroelf = ElfInfo("mongoose_sub.nef", "NITRO")
nitroelf.add_relocation("mongoose_sub.sbin", 0x394, 0x37F8000, 0x11684)
nitroelf.add_relocation("mongoose_sub.sbin", 0x394 + 0x11684, 0x27E0000, 0x16F4C)

twlelf = ElfInfo("mongoose.tef", "TWL")
twlelf.add_relocation("mongoose.TWL.FLX.sbin", 0x670, 0x37F8000, 0x122B4)
twlelf.add_relocation("mongoose.TWL.FLX.sbin", 0x670 + 0x122B4, 0x2FE0000, 0x188F0)
twlelf.add_relocation("mongoose.TWL.LTD.sbin", 0x4, 0x3000000, 0x1FCC4)
twlelf.add_relocation("mongoose.TWL.LTD.sbin", 0x4 + 0x1FCC4, 0x2F88000, 0x298B0)

for elfinfo in (nitroelf, twlelf):
    with open(elfinfo.get_filename(), "rb") as file:
        elf = ELFFile(file)
        dwarfinfo = elf.get_dwarf_info()
        
        for CU in dwarfinfo.iter_CUs():
            line_program = dwarfinfo.line_program_for_CU(CU)
        
            top_DIE = CU.get_top_DIE()
            path = top_DIE.get_full_path()
            
            for elem in paths:
                if path.startswith(elem):
                    break
                    
            else:
                # did not find
                print("ignored", path)
                continue
            
            for DIE in CU.iter_DIEs():
                if DIE.tag == "DW_TAG_subprogram":
                    addr = (DIE.attributes['DW_AT_low_pc'].value, DIE.attributes['DW_AT_high_pc'].value)
                    name = DIE.attributes["DW_AT_name"].value.decode()
                    data = elfinfo.get_data(addr[0], addr[1] - addr[0])
                    if data is None:
                        print("cannot find data", name)
                        continue
                        
                    target = "subprograms/" + os.path.basename(path) + "/" + name + "/" + elfinfo.get_internal() + ".bin"
                    os.makedirs(os.path.dirname(target), exist_ok=True)
                    with open(target, "wb") as f:
                        f.write(data)