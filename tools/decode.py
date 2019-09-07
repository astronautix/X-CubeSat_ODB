#!/usr/bin/python3

import sys
import re
import base64
import binascii
import struct
import datetime

re1 = re.compile( '^.*] ([=+])([^ ]+)$' )
whk = [ 'type', 'len', 'ticks', 'ts' ]
acc = ''

rid = {
   0x02: 'SU_R_ACK',
   0x03: 'SU_R_NACK',
   0x04: 'SU_R_ID',
   0x20: 'SU_R_HK',
   0x30: 'SU_R_SDP',
   0x33: 'SU_R_CAL'
}

opath = './out'


def handleADCS( w, x ):
   print( "ADCS" )


def handleFIPEX( w, x ):
   #time = '{0:%Y-%m-%d@%H:%M:%S}'.format( datetime.datetime.utcfromtimestamp( w['ts'] ))
   time = '{0:%Y%m%d_%H%M%S}'.format( datetime.datetime.utcfromtimestamp( w['ts'] ))
   rsp_id = rid.get( x[1], '(unknown)' )
   print( "%s FIPEX %s (len: %s)" % ( time, rsp_id, w['len'] ))

   binpath = opath + '/FPX_' + time + '.bin'

   with open( binpath, 'wb' ) as w:
      w.write( x )

   hexpath = opath + '/FPX_' + time + '.hex'

   with open( hexpath, 'wb' ) as w:
      w.write( binascii.hexlify( x ))


def handleWOD( w, x ):
   print( "WOD" )


def handleXXX( w, x ):
   print( "(unknown type)" )


def handle( s ):
   # a simple branch table
   x = base64.b64decode( s )
   w = dict( zip( whk, struct.unpack( '<BxHLL', x[:12] )))
   brt = {
      0x00: handleADCS,
      0x14: handleFIPEX,
      0x58: handleWOD
   }
   lamb = brt.get( w['type'], handleXXX )
   return lamb( w, x[12:] )


for line in sys.stdin:
   m1 = re1.search( line )
   if m1:
      g1 = [ arg.strip() for arg in m1.groups() ]
      if g1:
         acc += g1[1]
         if g1[0] == '=':
            handle( acc )
            acc = ''

