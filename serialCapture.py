import os
import sys
import glob
import serial
import time
from datetime import datetime

BAUDRATE=9600
DATA_FILE="/tmp/serialData.txt"

def getCOM():
   # Assume the first /dev/tty.usb* is our COM
   ports = glob.glob("/dev/tty.usb*")
   if len(ports) >= 1:
      return ports[0]
   return None

def monitor():
   wait_for_it = False
   comport = getCOM()
   if not comport:
      wait_for_it = True
      print "Connect the Philometer..."
      while not comport:
         time.sleep(1)
         comport = getCOM()

   datafile = "data/%s.txt" % datetime.now().isoformat()[:16] 
   print "Capturing %s to %s" % (comport, datafile)
   
   ser = serial.Serial(comport, BAUDRATE, timeout=0)

   if wait_for_it:
      sys.stdout.write("Waiting for device to register")
      for i in range(10):
         sys.stdout.write('.')
         sys.stdout.flush()
         time.sleep(1)
      print "Done! \n"
   

   print "Collecting data..."
   while (1):
      line = ser.readline()
      if (line != ""):
         text_file = open(datafile, "a")
         text_file.write(line)
         text_file.close()

monitor()
