
import keyboard
import time
if __name__ == "__main__":

    trackName = input("Enter Track Name: ")
    print(3)
    time.sleep(1)
    print(2)
    time.sleep(1)
    print(1)
    time.sleep(1)
    header = open("tracks\\" + trackName + ".h", "w+")
    header.write("#ifndef " + trackName.upper() + "\n")
    header.write("#define " + trackName.upper() + "\n")
    header.write("#include \"../logic.h\"\n")
    header.write("extern Track " + trackName + ";\n")
    header.write("#endif\n")
    header.close()
    trackStr = "{\n"
    len = 0;
    limit = 4096
    aPress = False
    bPress = False
    downPress = False
    rightPress = False
    skip = False
    print("Go!")
    for i in range(limit):
        if skip:
            trackStr += "0, "
            if i % 7 == 0:
                trackStr += "\n"
            continue
        timeStr = 0
        if keyboard.is_pressed('z'):
            if not aPress:
                timeStr += 8
                aPress = True
        else:
            aPress = False
        if keyboard.is_pressed('x'):
            if not bPress:
                timeStr += 4
                bPress = True
        else:
            bPress = False
        if keyboard.is_pressed('down'):
            if not downPress:
                timeStr += 2
                downPress = True
        else:
            downPress = False
        if keyboard.is_pressed('right'):
            if not rightPress:
                timeStr += 1
                rightPress = True
        else:
            rightPress = False
        trackStr += str(timeStr) + ", "
        if i % 7 == 0:
            trackStr += "\n"
        if keyboard.is_pressed('return'):
            len = i
            skip = True

        time.sleep(1/60)
    trackStr += "}"
    mainFile = open("tracks\\" + trackName + ".c", "w+")
    mainFile.write('#include "' + trackName + '.h"\n')
    mainFile.write("Track " + trackName + " = {" + trackStr + ", " + str(min(len + 90, limit)) + "};\n")
    mainFile.close()

        