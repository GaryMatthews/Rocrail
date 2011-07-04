/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2011 - Rob Versluis <r.j.versluis@rocrail.net>

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

#include "rocdigs/impl/cbus_impl.h"

#include "rocdigs/impl/cbus/flim.h"
#include "rocdigs/impl/cbus/utils.h"

#include "rocs/public/trace.h"
#include "rocs/public/mem.h"
#include "rocs/public/str.h"
#include "rocs/public/system.h"

#include "rocdigs/impl/cbus/cbusdefs.h"

/*
 * The frame has the incoming FLiM message.
 * A Program node is created if the client should be informed.
 * The extraMsg is a pointer to an empty byte array which can be used
 * to request more info of the node before creating a Program response
 * for clients.
 */
iONode processFLiM(obj inst, int opc, byte *frame, byte **extraMsg) {
  iOCBUSData data = Data(inst);
  byte cmd[32];

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "FLiM: 0x%02X", opc );

  switch(opc) {
  case OPC_NNACK:
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "FLiM: request node parameters" );
    *extraMsg = allocMem(32);
    cmd[0] = OPC_RQNP;
    makeFrame(inst, *extraMsg, PRIORITY_NORMAL, cmd, 0 );
    break;
  }

  case OPC_PARAMS:
  {
    /* :SB020NEFA5520320020000; */
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "FLiM: node parameters received" );
    break;
  }

  return NULL;
}



