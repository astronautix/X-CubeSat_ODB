#!/usr/bin/python3

import sys
import re
import datetime

p1 = re.compile( '^[^[]*\[([0-9. ]*)\] (.*)$' )
p2 = re.compile( 'RTC.*time is.*\(([0-9]+)\)$' )

dt = 0

for line in sys.stdin:
   #v1 = p1.search( line ).groups()
   v1 = [ arg.strip() for arg in p1.search( line ).groups() ]
   ( ts1, log ) = ( float( v1[0] ), v1[1] )
   v2 = p2.search( log )
   if v2:
      g2 = v2.groups()
      ts2 = int( g2[0] )
      dt = ts2 - ts1
      #print( dt )

   ts = round( ts1 + dt )
   print( "%s %s" % ( '{0:%Y-%m-%d@%H:%M:%S}'.format( datetime.datetime.utcfromtimestamp( ts )), log ))

