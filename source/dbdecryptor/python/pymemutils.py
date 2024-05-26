import pymem
import ctypes

def patternScanAllAddress(processId, pattern, maxAddress, maxCount):
    nextRegion = 0
    pageFound = None
    addresses = []
    hProcess = ctypes.windll.kernel32.OpenProcess(0x1F0FFF, False, processId)
    while nextRegion < maxAddress:
        try:
            nextRegion, pageFound = pymem.pattern.scan_pattern_page(
                hProcess,
                nextRegion,
                pattern.encode("utf-8"),
                return_multiple=True
            )
        except Exception as e:
            print(e)
            break
        if pageFound:
            addresses += pageFound
        if len(addresses) > maxCount:
            break
    return addresses

if __name__ == "__main__":
    ret = patternScanAllAddress(18680, '\\\\Msg\\\\FTSContact', 140737488289792, 100)
    print(ret)
