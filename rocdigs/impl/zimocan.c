/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2014 Rob Versluis, Rocrail.net

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

#include "rocdigs/impl/zimocan_impl.h"

#include "rocs/public/trace.h"
#include "rocs/public/node.h"
#include "rocs/public/attr.h"
#include "rocs/public/mem.h"
#include "rocs/public/str.h"
#include "rocs/public/strtok.h"
#include "rocs/public/system.h"

#include "rocrail/wrapper/public/DigInt.h"
#include "rocrail/wrapper/public/SysCmd.h"
#include "rocrail/wrapper/public/Command.h"
#include "rocrail/wrapper/public/FunCmd.h"
#include "rocrail/wrapper/public/Loc.h"
#include "rocrail/wrapper/public/Switch.h"
#include "rocrail/wrapper/public/Signal.h"
#include "rocrail/wrapper/public/Output.h"
#include "rocrail/wrapper/public/Feedback.h"
#include "rocrail/wrapper/public/Response.h"
#include "rocrail/wrapper/public/FbInfo.h"
#include "rocrail/wrapper/public/FbMods.h"
#include "rocrail/wrapper/public/Program.h"
#include "rocrail/wrapper/public/State.h"
#include "rocrail/wrapper/public/Accessory.h"
#include "rocrail/wrapper/public/Clock.h"
#include "rocrail/wrapper/public/Text.h"

#include "rocdigs/impl/zimocan/zimocan_const.h"
#include "rocdigs/impl/zimocan/serial.h"
#include "rocdigs/impl/zimocan/udp.h"

#include "rocutils/public/crc.h"


static int instCnt = 0;


static int __makePacket( byte* msg, int group, int cmd, int mode, int dlc, int id, int addr, int d2, int d3, int d4, int d5, int d6, int d7 );
static int __getNID(byte* msg);

/** ----- OBase ----- */
static void __del( void* inst ) {
  if( inst != NULL ) {
    iOZimoCANData data = Data(inst);
    /* Cleanup data->xxx members...*/
    
    freeMem( data );
    freeMem( inst );
    instCnt--;
  }
  return;
}

static const char* __name( void ) {
  return name;
}

static unsigned char* __serialize( void* inst, long* size ) {
  return NULL;
}

static void __deserialize( void* inst,unsigned char* bytestream ) {
  return;
}

static char* __toString( void* inst ) {
  return NULL;
}

static int __count( void ) {
  return instCnt;
}

static struct OBase* __clone( void* inst ) {
  return NULL;
}

static Boolean __equals( void* inst1, void* inst2 ) {
  return False;
}

static void* __properties( void* inst ) {
  return NULL;
}

static const char* __id( void* inst ) {
  return NULL;
}

static void* __event( void* inst, const void* evt ) {
  return NULL;
}

/** ----- OZimoCAN ----- */

static iONode __translate( iOZimoCAN inst, iONode node ) {
  iOZimoCANData data = Data(inst);
  iONode rsp = NULL;

  TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "translate: %s", NodeOp.getName( node ) );

  if( StrOp.equals( NodeOp.getName( node ), wFbInfo.name() ) ) {
  }

  /* System command. */
  else if( StrOp.equals( NodeOp.getName( node ), wSysCmd.name() ) ) {
    const char* cmdstr = wSysCmd.getcmd( node );
    if( StrOp.equals( cmdstr, wSysCmd.stop ) ) {
      /* CS off */
      byte* msg = allocMem(32);
      msg[0] = __makePacket(msg+1, SYSTEM_CONTROL_GROUP, SYSTEM_POWER, MODE_CMD, 4, data->NID, data->masterNID, SYSTEM_POWER_TRACK1, SYSTEM_POWER_OFF, 0, 0, 0, 0);
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "request power OFF" );
      ThreadOp.post(data->writer, (obj)msg);
    }

    else if( StrOp.equals( cmdstr, wSysCmd.go ) ) {
      /* CS on */
      byte* msg = allocMem(32);
      msg[0] = __makePacket(msg+1, SYSTEM_CONTROL_GROUP, SYSTEM_POWER, MODE_CMD, 4, data->NID, data->masterNID, SYSTEM_POWER_TRACK1, SYSTEM_POWER_ON, 0, 0, 0, 0);
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "request power ON" );
      ThreadOp.post(data->writer, (obj)msg);
    }
  }

  return rsp;
}


/**  */
static iONode _cmd( obj inst ,const iONode cmd ) {
  iOZimoCANData data = Data(inst);
  iONode rsp = NULL;

  if( cmd != NULL ) {
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "zimocan cmd = %s", NodeOp.getName(cmd) );
    rsp = __translate( (iOZimoCAN)inst, cmd );
    cmd->base.del(cmd);
  }
  return rsp;
}


/**  */
static byte* _cmdRaw( obj inst ,const byte* cmd ) {
  return 0;
}


/**  */
static void _halt( obj inst ,Boolean poweroff ) {
  iOZimoCANData data = Data(inst);
  data->run = False;
  ThreadOp.sleep(100);
}


/**  */
static Boolean _setListener( obj inst ,obj listenerObj ,const digint_listener listenerFun ) {
  iOZimoCANData data = Data(inst);
  data->listenerObj = listenerObj;
  data->listenerFun = listenerFun;
  return True;
}


/**  */
static Boolean _setRawListener( obj inst ,obj listenerObj ,const digint_rawlistener listenerRawFun ) {
  return 0;
}


/** external shortcut event */
static void _shortcut( obj inst ) {
  return;
}


/** bit0=power, bit1=programming, bit2=connection */
static int _state( obj inst ) {
  return 0;
}


/**  */
static Boolean _supportPT( obj inst ) {
  return 0;
}


/** vmajor*1000 + vminor*100 + patch */
static int vmajor = 2;
static int vminor = 0;
static int patch  = 99;
static int _version( obj inst ) {
  iOZimoCANData data = Data(inst);
  return vmajor*10000 + vminor*100 + patch;
}


/*
Header Size  Group Cmd+Mode NId    Data0...7 CRC16 Tail
16Bit  8Bit  8Bit  8Bit     16Bit  8x8Bit    16Bit 16 Bit
 */
static int __makePacket( byte* msg, int group, int cmd, int mode, int dlc, int nid, int nidTarget, int d2, int d3, int d4, int d5, int d6, int d7 ) {
  int crc = 0;

  msg[0]  = 0x5A;
  msg[1]  = 0x32;
  msg[2]  = dlc;
  msg[3]  = group;
  msg[4]  = (cmd << 2);
  msg[4] |= (mode & 0x03);
  msg[5]  = (nid & 0xFF);
  msg[6]  = ((nid >> 8) & 0xFF);
  msg[7]  = (nidTarget & 0xFF);
  msg[8]  = ((nidTarget >> 8) & 0xFF);
  msg[9]  = d2;
  msg[10] = d3;
  msg[11] = d4;
  msg[12] = d5;
  msg[13] = d6;
  msg[14] = d7;

  crc = CRCOp.checkSum16(msg+2, 13);
  msg[15] = (crc & 0xFF);
  msg[16] = ((crc >> 8) & 0xFF);

  msg[17] = 0x32;
  msg[18] = 0x5A;

  return 19;
}

static int __getNID(byte* msg) {
  int nid = msg[5] + (msg[6] * 256);
  return nid;
}


static void __evauluateNetworkGroup( iOZimoCAN zimocan, byte* msg ) {
  iOZimoCANData data    = Data(zimocan);
  int cmd  = (msg[4] >> 2);
  int mode = (msg[4] &0x03);

  switch( cmd ) {
  case NETWORK_PING:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "NETWORK PING" );
    if( !data->connAck ) {
      byte* msgInfo = allocMem(32);
      data->connAck = True;
      data->masterNID = __getNID(msg);
      /* Get software info */
      msgInfo[0] = __makePacket(msgInfo+1, NETWORK_GROUP, NETWORK_MODULINFO, MODE_REQ, 4, data->NID, data->masterNID, 2, 0, 0, 0, 0, 0);
      TraceOp.trc( name, TRCLEVEL_MONITOR, __LINE__, 9999, "request info" );
      ThreadOp.post(data->writer, (obj)msgInfo);
    }
    break;
  case NETWORK_MODULINFO:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "NETWORK MODULINFO: %d %d %d %d", msg[9], msg[10], msg[11], msg[12] );
    break;
  }
}


static void __evaluateMsg( iOZimoCAN zimocan, byte* msg ) {
  iOZimoCANData data    = Data(zimocan);
  int group = msg[3];
  int id    = msg[5] + (msg[6] * 256);
  int addr  = msg[7] + (msg[8] * 256);

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "Message from 0x%04X addr=0x%04X", id, addr );

  switch( group ) {
  case SYSTEM_CONTROL_GROUP:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "SYSTEM CONTROL GROUP" );
    break;
  case NETWORK_GROUP:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "NETWORK GROUP" );
    __evauluateNetworkGroup(zimocan, msg);
    break;
  default:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "GROUP: 0x%02X", group );
    break;
  }
}


static void __reader( void* threadinst ) {
  iOThread      th      = (iOThread)threadinst;
  iOZimoCAN     zimocan = (iOZimoCAN)ThreadOp.getParm( th );
  iOZimoCANData data    = Data(zimocan);
  byte msg[256];
  int size = 0;

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoCAN reader started." );

  while( data->run ) {
    if( !data->subAvailable( (obj)zimocan) ) {
      ThreadOp.sleep( 10 );
      continue;
    }
    else {
      TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "CAN message available" );
    }

    size = data->subRead( (obj)zimocan, msg );
    __evaluateMsg(zimocan, msg);
    ThreadOp.sleep(10);
  }

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoCAN reader ended." );
}


static void __writer( void* threadinst ) {
  iOThread      th      = (iOThread)threadinst;
  iOZimoCAN     zimocan = (iOZimoCAN)ThreadOp.getParm( th );
  iOZimoCANData data    = Data(zimocan);
  int connAckTimer = 0;
  byte eyeCatcher[] = {'Z','1','1','Z',0};
  byte msg[256];

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoCAN writer started." );

  while( data->run ) {
    byte * post = NULL;
    int len = 0;

    post = (byte*)ThreadOp.getPost( th );

    if (post != NULL) {
      /* first byte is the message length */
      len = post[0];
      MemOp.copy( msg, post+1, len);
      freeMem( post);
      if( data->subWrite((obj)zimocan, msg, len) ) {

      }
    }

    if( !data->connAck ) {
      connAckTimer++;
      if( connAckTimer > 5000 ) {
        if( data->subWrite((obj)zimocan, eyeCatcher, 4) ) {

        }
      }
    }

    ThreadOp.sleep(10);
  }

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoCAN writer ended." );
}


/**  */
static struct OZimoCAN* _inst( const iONode ini ,const iOTrace trc ) {
  iOZimoCAN __ZimoCAN = allocMem( sizeof( struct OZimoCAN ) );
  iOZimoCANData data = allocMem( sizeof( struct OZimoCANData ) );
  MemOp.basecpy( __ZimoCAN, &ZimoCANOp, 0, sizeof( struct OZimoCAN ), data );

  TraceOp.set( trc );
  SystemOp.inst();
  /* Initialize data->xxx members... */

  data->ini = ini;
  data->iid = StrOp.dup( wDigInt.getiid( ini ) );
  data->NID = 0xC200;
  data->masterNID = 0xAFFA; /* Dummy */

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "ZimoCAN %d.%d.%d", vmajor, vminor, patch );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "iid    = %s", data->iid );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "sublib = %s", wDigInt.getsublib( ini ) );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );

  /* choose interface: */
  if( StrOp.equals( wDigInt.sublib_serial, wDigInt.getsublib( ini ) ) ) {
    /* serial */
    data->subConnect    = SerialConnect;
    data->subDisconnect = SerialDisconnect;
    data->subRead       = SerialRead;
    data->subWrite      = SerialWrite;
    data->subAvailable  = SerialAvailable;
  }
  else  {
    /* UDP */
    data->subConnect    = UDPConnect;
    data->subDisconnect = UDPDisconnect;
    data->subRead       = UDPRead;
    data->subWrite      = UDPWrite;
    data->subAvailable  = UDPAvailable;
  }

  data->connOK = data->subConnect((obj)__ZimoCAN);
  data->run = True;

  data->reader = ThreadOp.inst( "zcanreader", &__reader, __ZimoCAN );
  ThreadOp.start( data->reader );

  data->writer = ThreadOp.inst( "zcanwriter", &__writer, __ZimoCAN );
  ThreadOp.start( data->writer );


  instCnt++;
  return __ZimoCAN;
}


iIDigInt rocGetDigInt( const iONode ini ,const iOTrace trc )
{
  return (iIDigInt)_inst(ini,trc);
}


/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
#include "rocdigs/impl/zimocan.fm"
/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/