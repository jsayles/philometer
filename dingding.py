#!/usr/bin/python

import sys
import time
import urllib2

TRIGGER_URL="http://pi1.local/trigger"

def main(sec):
   print("Counting down %s seconds..." % sec)

   start_ts = time.time()
   end_ts = start_ts + sec

   while True:
      if time.time() > end_ts:
         print("DING DING DING!")
         urllib2.urlopen(TRIGGER_URL, data="")
         exit(0)

if __name__ == "__main__":
   if len(sys.argv) > 2:
      s = int(sys.argv[1])
   else:
      s = 0
   main(s)
