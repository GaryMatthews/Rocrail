/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2013 Rob Versluis, Rocrail.net

 Without an official permission commercial use is not permitted.
 Forking this project is not permitted.

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

#include "rocdigs/impl/raspi_impl.h"

#include "rocs/public/mem.h"

#include "rocrail/wrapper/public/DigInt.h"
#include "rocrail/wrapper/public/RasPi.h"
#include "rocrail/wrapper/public/SysCmd.h"
#include "rocrail/wrapper/public/Command.h"
#include "rocrail/wrapper/public/FunCmd.h"
#include "rocrail/wrapper/public/Loc.h"
#include "rocrail/wrapper/public/LocList.h"
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

#include "rocdigs/impl/raspi/io.h"

static int instCnt = 0;

/** ----- OBase ----- */
static void __del( void* inst ) {
  if( inst != NULL ) {
    iORasPiData data = Data(inst);
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
  iORasPiData data = Data(inst);
  return data->ini;
}

static const char* __id( void* inst ) {
  iORasPiData data = Data(inst);
  return data->iid;
}

static void* __event( void* inst, const void* evt ) {
  return NULL;
}

/** ----- ORasPi ----- */

static void __reportState(iORasPi inst) {
  iORasPiData data = Data(inst);

  if( data->listenerFun != NULL && data->listenerObj != NULL ) {
    iONode node = NodeOp.inst( wState.name(), NULL, ELEMENT_NODE );

    if( data->iid != NULL )
      wState.setiid( node, data->iid );
    wState.setpower( node, data->power );
    wState.setsensorbus( node, True );
    wState.setaccessorybus( node, True );

    data->listenerFun( data->listenerObj, node, TRCLEVEL_INFO );
  }
}

static iONode __translate(iORasPi inst, iONode node) {
  iORasPiData data = Data(inst);
  iONode rsp = NULL;

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "cmd=%s", NodeOp.getName( node ) );

  /* System command. */
  if( StrOp.equals( NodeOp.getName( node ), wSysCmd.name() ) ) {
    const char* cmdstr = wSysCmd.getcmd( node );

    if( StrOp.equals( cmdstr, wSysCmd.stop ) ) {
      data->power = False;
      __reportState(inst);
    }
    else if( StrOp.equals( cmdstr, wSysCmd.go ) ) {
      data->power = True;
      __reportState(inst);
    }
  }

  return rsp;
}

/**  */
static iONode _cmd( obj inst ,const iONode cmd ) {
  iORasPiData data = Data(inst);
  iONode rsp = NULL;
  if( cmd != NULL ) {
    rsp = __translate( (iORasPi)inst, cmd );
    NodeOp.base.del(cmd);
  }
  return rsp;
}


/**  */
static byte* _cmdRaw( obj inst ,const byte* cmd ) {
  return 0;
}


/**  */
static void _halt( obj inst ,Boolean poweroff ) {
  iORasPiData data = Data(inst);
  data->run = False;
  ThreadOp.sleep(50);
}


/**  */
static Boolean _setListener( obj inst ,obj listenerObj ,const digint_listener listenerFun ) {
  iORasPiData data = Data(inst);
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
static int patch  = 0;
static int _version( obj inst ) {
  iORasPiData data = Data(inst);
  return vmajor*10000 + vminor*100 + patch;
}


static void __worker( void* threadinst ) {
  iOThread  th    = (iOThread)threadinst;
  iORasPi   raspi = (iORasPi)ThreadOp.getParm( th );
  iORasPiData data = Data(raspi);

  ThreadOp.sleep(500);
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "RasPi worker started." );

  do {
    ThreadOp.sleep(10);
  } while( data->run );

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "RasPi worker stopped." );
}


/**  */
static struct ORasPi* _inst( const iONode ini ,const iOTrace trc ) {
  iORasPi __RasPi = allocMem( sizeof( struct ORasPi ) );
  iORasPiData data = allocMem( sizeof( struct ORasPiData ) );
  MemOp.basecpy( __RasPi, &RasPiOp, 0, sizeof( struct ORasPi ), data );

  /* Initialize data->xxx members... */
  data->ini = ini;
  if( wDigInt.getraspi(ini) == NULL ) {
    iONode raspiIni = NodeOp.inst( wRasPi.name(), ini, ELEMENT_NODE );
    NodeOp.addChild(ini, raspiIni);
  }

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "RasPi %d.%d.%d", vmajor, vminor, patch );
#ifdef __arm__
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "YES!" );
  raspiSetupIO(__RasPi);
#else
  TraceOp.trc( name, TRCLEVEL_WARNING, __LINE__, 9999, "Not running on a Raspberry Pi" );
#endif

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "----------------------------------------" );

  data->run = True;
  data->worker = ThreadOp.inst( "raspiworker", &__worker, __RasPi );
  ThreadOp.start( data->worker );


  instCnt++;
  return __RasPi;
}


/* Support for dynamic Loading */
iIDigInt rocGetDigInt( const iONode ini ,const iOTrace trc )
{
  return (iIDigInt)_inst(ini,trc);
}

/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
#include "rocdigs/impl/raspi.fm"
/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/