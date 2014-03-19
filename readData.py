import os
import sys
import glob
import serial
import time
from datetime import datetime

def getDataFile():
   # Assume the last file is our guy
   files = glob.glob("data/*")
   if len(files) >= 1:
      return files[len(files)-1]
   return None

def monitor():
   datafile = getDataFile()
   if not datafile:
      print "No data files!"
      exit

   print "Reding from %s" % datafile
   text_file = open(datafile, "r")
   while (1):
      line = text_file.readline()
      if (line != ""):
         fields = line.split(',');
         if len(fields) > 17:
            values = (fields[0], fields[2], fields[3], fields[4], fields[13], fields[14], fields[15], fields[16], fields[17])
            print "Time: %s\tBrain: %s/%s/%s\t\tHeart: %s/%s\tGSR: %s\tTemp: %s/%s" % values
      else:
         time.sleep(1)

monitor()
