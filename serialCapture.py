import sys, os, serial, threading

COMPORT="/dev/tty.usbmodemfa131"
BAUDRATE=9600
DATA_FILE="/tmp/serialData.txt"

def monitor():
   ser = serial.Serial(COMPORT, BAUDRATE, timeout=0)
   print "Capturing %s to %s" % (COMPORT, DATA_FILE)
   while (1):
      line = ser.readline()
      if (line != ""):
         #print line
         #fields = line[:-1].split('; ');
         #ID = fields[0]
         #TIME = int(fields[1])
         #print fields
         #print "device ID: ", ID

         # write to file
         text_file = open(DATA_FILE, "a")
         text_file.write(line)
         text_file.close()

monitor()
