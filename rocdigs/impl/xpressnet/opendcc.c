/*
 Rocrail - Model Railroad Software

 Copyright (C) Rob Versluis <r.j.versluis@rocrail.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "rocdigs/impl/xpressnet_impl.h"
#include "rocdigs/impl/xpressnet/opendcc.h"
#include "rocdigs/impl/xpressnet/li101.h"

#include "rocrail/wrapper/public/Clock.h"

#include <time.h>


Boolean opendccConnect(obj xpressnet) {
  return li101Connect(xpressnet);
}

void opendccDisConnect(obj xpressnet) {
  li101DisConnect(xpressnet);
}

Boolean opendccAvail(obj xpressnet) {
  return li101Avail(xpressnet);
}

void opendccInit(obj xpressnet) {
  /*
  Messages from the Host to Client
  0x75 0xFF SID1_H SID1_L SID2_H SID2_L [XOR]
  This is a broadcast: BiDi detectors with a SID between SID1 and SID2 should report their actual state.
  This broadcast is typically issued after power up to collect all states of the BiDi detectors.
  */
  li101Init(xpressnet);
}

/*
 * BiDi Processing
 *
  Commands from the Client to the Host
   
  0x73 0xF0 SID_H SID_L [XOR]
  BiDi detector with the logical addr SID (=sender ID) is idle. This message shall be issued once if
  the detector changes from occupied to idle. The occupancy detector should not issue occupancy messages more often than 3s.

  0x73 0xF1 SID_H SID_L [XOR]
  BiDi detector with the logical addr SID (=sender ID) is occupied. This message shall be issued once if
  the detector changes from idle to occupied. The occupancy detector should not issue occupancy messages more often than 2s.

  0x75 0xF2 SID_H SID_L D+AddrH AddrL [XOR]
  BiDi detector with the logical addr SID (=sender ID) has detected a locomotive with the address
  Addr. D is the MSB of the AddrH byte and denotes the direction. D=0 indicates that the locomotive
  has it's right side is connected to the detector, D=1 denotes the left side is connected to the detector.

  0x75 0xFF SID1_H SID1_L SID2_H SID2_L [XOR]
  Request a scan message of the command station in the given range. The command station will issue a
  broadcast (see below) with this range.

  0x74 0xE0 AddrH AddrL SPEED [XOR]
  Locomotive with the address Addr anounces its real speed. Speedbyte is coded according to the BiDi-Standard.
  A detector shall issue this package only if a speed change is detected.

  0x78 0xE1 SID_H SID_L AddrH AddrL CV_H CV_L DAT [XOR]
  BiDi detector with the logical addr SID (=sender ID) has detected a BiDi PoM command.
  The locomotive with the address Addr responds to a configuration variable CV access with the content DAT

  0x78 0xD0+BiDi-ID AddrH AddrL Data[1], ... Data[n] [XOR]
  This is a raw message mirroring the BiDi message on the track 1:1 (reserved for future use)
 */
static void __evaluateBiDi(obj xpressnet, byte* buffer) {

}

int opendccRead(obj xpressnet, byte* buffer, Boolean* rspreceived) {
  int liRead = li101Read(xpressnet, buffer, rspreceived);
  if(buffer[0] & 0x70 == 0x70 ) {
    /* BiDi packet */
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "TODO: Processing BiDi packet." );
  }
  return liRead;
}
Boolean opendccWrite(obj xpressnet, byte* buffer, Boolean* rspexpected) {
  return li101Write(xpressnet, buffer, rspexpected);
}


void opendccTranslate( obj xpressnet, void* node ) {
  iOXpressNetData data = Data(xpressnet);

  /* Clock command. */
  if( StrOp.equals( NodeOp.getName( node ), wClock.name() ) ) {
    /*
    from Slave to Command Station / and from Command Station to Slave:

    0x00 0x01 TCODE0 {TCODE1 TCODE2 TCODE3} Timecode transfer, accelerated layout time.
    A TCODE consists of one byte, coded binary as CCDDDDDD, where CC denotes the type of
    code and DDDDDDD the corresponding data.
    TCODE Content
    CC=00 DDDDDD = mmmmmm, this denotes the minute, range 0..59.
    CC=10 DDDDDD = 0HHHHHH, this denotes the hour, range 0..23
    CC=01 DDDDDD = 000WWW, this denotes the day of week,
    0=Monday, 1=Tuesday, 2=Wednesday, 3=Thursday, 4=Friday, 5=Saturday, 6=Sunday.
    CC=11 DDDDDD = 00FFFFF, this denotes the acceleration factor, range 0..31;
    an acceleration factor of 0 means clock is stopped, a factor of 1 means clock is running
    real time, a factor of 2 means clock is running twice as fast a real time.
    This message is issued as broadcast once every (layout-) minute. The command is not repeated.

    When no Parameters are given, it is a query and the answer will be sent only to the requesting slave.
     */
    if( data->fastclock && StrOp.equals( wClock.set, wClock.getcmd( node ) ) ) {
      long l_time = wClock.gettime(node);
      struct tm* lTime = localtime( &l_time );

      int mins    = lTime->tm_min;
      int hours   = lTime->tm_hour;
      int wday    = lTime->tm_wday;
      int divider = wClock.getdivider(node);
      byte* outa  = NULL;

      TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "set clock to %02d:%02d divider=%d", hours, mins, divider );

      outa = allocMem(32);
      outa[0] = 0x05;
      outa[1] = 0xF1;
      outa[2] = 0x00 + mins;
      outa[3] = 0x80 + hours;
      outa[4] = 0x40 + wday;
      outa[5] = 0xC0 + divider;
      ThreadOp.post( data->transactor, (obj)outa );
    }
  }
}
