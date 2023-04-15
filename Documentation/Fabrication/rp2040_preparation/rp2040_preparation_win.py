#!/usr/bin/env python3

# rp2040_preparation.py

# Python tool for initial flashing of
#  * esp32_addon_bootloader
#  * esp32_mouse_keyboard
#  * FLipMouse / FLipWare / FabiWare firmwares
# Please refer to README.md for documentation

# Copyright 2022 Benjamin Aigner <beni@asterics-foundation.org>

#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  

import argparse
import serial
import serial.tools.list_ports
import io
import sys
import os
import time
import subprocess

portName="COM1"

def getPort():
    global portName
    global connected
    while True:
        connectedWA = []
        comlist = serial.tools.list_ports.comports()
        for element in comlist:
            connectedWA.append(element.device)
        if len(connectedWA) != len(connected):
            # print("Connected COM ports: " + str(connectedWA))
            break

    for port in connectedWA:
        if port not in connected:
            print("The Com Port of the Arduino Nano is: " +port)
            portName=port
            break

comlist = serial.tools.list_ports.comports()
connected = []
for element in comlist:
    connected.append(element.device)
print("Connected COM ports: " + str(connected))
print("Now connect the Arduino Nano 2040 !")

getPort()

devices = {
  'FM': "FLipMouse",
  'FP': "FLipPad",
  'FB': "FABI"
}

#subprocess.run('python D:/your_script.py')

def main():
    global current_serial
    def error(msg):
        print(msg)
        sys.exit(1)
    parser = argparse.ArgumentParser(description='Prepare Arduino Nano RP2040 connect for FLipMouse, FLipPad or FABI')
    parser.add_argument('-d' , '--device', dest="device",
                        help='select a device type to be flashed (FM, FP or FB)')
    parser.add_argument('-o', '--start_with', dest='startwith', help='Start with this step')
    args = parser.parse_args()
    
    if not args.device:
      error("Error: Need a device type (e.g. -d FM)")
    elif not args.device.upper() in devices:
      error("Error: Need a correct device type (e.g. -d FM)")
    else:
      global current_device
      current_device = args.device.upper()
      
    if not args.serial:
      current_serial = portName
    else:
      current_serial = args.serial
      
    global startwith
    if args.startwith:
      startwith = int(args.startwith)
      print("Starting with step " + args.startwith)
    else:
      startwith = 0
    
    while 1:
      print("Flashing " + devices[args.device] + " firmware to " + current_serial)
      
      # 1.) serialflasher1.uf2 -> esptool.py compatible serial interface to the ESP32, ESP32 is in download mode
      if startwith <= 1:
        print("Part 1 ###############################################")
        proc = subprocess.run(["python","uf2conv.py","serialflasher1.uf2","-s", current_serial,"-D"], capture_output=True, text=True)
        print(proc.stdout)
        if proc.returncode != 0:
          print("Error flashing UF2 'serialflasher1.uf2'")
          exit
        time.sleep(5)
      
      # 2.) esptool.py -> flash the bootloader
      if startwith <= 2:
        print("Part 2 ###############################################")
        retry = 0
        while 1:
          # this was the output from idf.py build:
          # esptool.py -p (PORT) -b 460800 --before no_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 
          # 0x1000 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0xd000 build/ota_data_initial.bin 0x10000 build/esp32_addon_bootloader.bin
          proc = subprocess.run(["esptool","--port", current_serial,"--baud", "115200", "--before", "no_reset", "--after", "hard_reset", "--chip", "esp32", "write_flash", \
            "--flash_mode", "dio", "--flash_size", "detect", "--flash_freq", "40m", \
            "0x1000", "bootloader.bin", "0x8000", "partition-table.bin", "0xd000", "ota_data_initial.bin", "0x10000", "esp32_addon_bootloader.bin"], capture_output=True, text=True)
          print(proc.stdout)
          print(proc.stderr)
          if proc.returncode != 0:
            print("Error flashing esp32_addon_bootloader.bin")
            getPort()
            current_serial = portName
            retry = retry + 1
            if retry > 10:
              exit
          else:
            break
          time.sleep(1)
      
      # 3.) serialflasher2.uf2 -> usb to serial passthrough sketch with 500k, compatible to FM/FP/FB ESP32 update mode
      if startwith <= 3:
        print("Part 3 ###############################################")
        time.sleep(2)
        getPort()
        current_serial = portName
        proc = subprocess.run(["python","uf2conv.py","serialflasher2.uf2","-s", current_serial,"-D"], capture_output=True, text=True)
        print(proc.stdout)
        if proc.returncode != 0:
          print("Error flashing UF2 'serialflasher1.uf2'")
          exit
      
      
      # 4.) wait for "OTA:ready" signal
      if startwith <= 4:
        print("Part 4 ###############################################")
        retry = 0
        while 1:
          try:
            ser = serial.Serial(current_serial, timeout=1)
            break
          except Exception:
            print("Could not open Serial Port "+current_serial)
            retry = retry + 1
            getPort()
            current_serial = portName
            if retry > 20:
              exit
            else:
              time.sleep(0.5)
            
        # avoid a Windows quirk: the Arduino does not respond to any data, if the DTR line is not set
        if os.name == 'nt':
          ser.dtr = True
        ser.flushInput()
        tries = 0
        while 1:
          try:
            line = ser.readline()
            print(str(line))
            line = line.decode()
            if line == "OTA:ready\r\n":
              print("Got OTA:ready")
              break
            else:
              tries = tries + 1
              if tries > 20:
                error("Not receiving OTA:ready")
                break
          except Exception:
            continue
        
      # 5.) flash esp32_mouse_keyboard.bin with update.py code
      if startwith <= 5:
        print("Part 5 ###############################################")
        if not ser:
          retry = 0
          while 1:
            try:
              ser = serial.Serial(current_serial, timeout=1)
              break
            except Exception:
              print("Could not open Serial Port "+current_serial)
              retry = retry + 1
              getPort()
              current_serial = portName
              if retry > 10:
                exit
              else:
                time.sleep(0.5)
        # as long as there is something available in the input file
        counter=0
        with open('esp32_mouse_keyboard.bin', "rb") as input:
          while True:
            # read in 128B chunks
            data = input.read(128)
            #datalen += len(data)
            #print(datalen)
            counter += 1
            if (counter % 20) == 0:
                print ("*", end="", flush=True)
            ser.write(data)
            ser.flush()
            # if this is the last block (less than 128B)
            if len(data) < 128:
                  # finished...
              print("Finished")
              break
            #sleep for 10ms 
            time.sleep(10 / 1000)
            # any feedback?
            if ser.in_waiting:
              line = ser.read(100)
              print(line.decode())
        ser.flush()
        
        tries = 0
        while 1:
          try:
            line = ser.readline()
            print(str(line))
            line = line.decode()
            if line == "OTA:$FINISHED\r\n":
              print("Got OTA:$FINISHED\r\n")
              break
            else:
              tries = tries + 1
              if tries > 20:
                error("Not receiving OTA:$FINISHED")
                break
          except Exception:
            continue
        ser.close()
      
      # 6.) FM/FB/FP.uf2 -> selected device firmware
      if startwith <= 6:
        print("Part 6 ###############################################")
        proc = subprocess.run(["python","uf2conv.py",current_device + ".uf2","-s", current_serial,"-D"], capture_output=True, text=True)
        print(proc.stdout)
        if proc.returncode != 0:
          print(proc.stderr)
          print("Error flashing UF2 '"+current_device+".uf2'")
          exit
        
      # 7.) test output of "AT BC $ID" to confirm working firmware for RP2040 & ESP32
      if startwith <= 7:
        print("Part 7 ###############################################")
        retry = 0
        while 1:
          try:
            ser = serial.Serial(current_serial, timeout=1)
            break
          except Exception:
            print("Could not open Serial Port "+current_serial)
            retry = retry + 1
            getPort()
            current_serial = portName
            if retry > 10:
              exit
            else:
              time.sleep(0.5)
        #set device name depending on selected module
        ser.write(str("AT BC $NAME "+devices[current_device] + "\r\n").encode())
        print(ser.readline().decode())
        retry = 0
        while 1:
          ser.write(str("AT BC $ID\r\n").encode())
          line = ser.readline().decode()
          if line.startswith("ESP32"):
            break
          else: 
            retry = retry + 1
            if retry > 15:
              print("Error getting ESP32 revision number")
              break
          time.sleep(1)
        ser.close()
      
      print("Finished - waiting 5s before starting new cycle")
      # TODO: endless cycles?
      break
      time.sleep(5)

if __name__ == "__main__":
    main()
