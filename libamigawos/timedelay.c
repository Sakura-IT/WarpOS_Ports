
#pragma pack(push,2)
#include <exec/memory.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <powerpc/powerpc_protos.h>
#pragma pack(pop)

/****************************************************************************/

LONG TimeDelay(LONG unit,ULONG secs,ULONG microsecs)
{ APTR SysBase = *(APTR *)4L;
  struct PortIO {
    struct timerequest treq;
    struct MsgPort     port;
  } *portio;
  LONG ret=-1;

  if ((portio=(struct PortIO *)AllocVecPPC(sizeof(*portio),MEMF_CLEAR|MEMF_PUBLIC, 32L))) {
    portio->port.mp_Node.ln_Type=NT_MSGPORT;
    if ((BYTE)(portio->port.mp_SigBit=AllocSignal(-1))>=0) {
      portio->port.mp_SigTask=FindTask(NULL);
      NewListPPC(&portio->port.mp_MsgList);
      portio->treq.tr_node.io_Message.mn_Node.ln_Type=NT_REPLYMSG;
      portio->treq.tr_node.io_Message.mn_ReplyPort=&portio->port;
      //SetCache(CACHE_DCHACHEFLUSH, portio, sizeof(struct PortIO);
      if (!(OpenDevice(TIMERNAME,unit,&portio->treq.tr_node,0))) {
        portio->treq.tr_node.io_Command=TR_ADDREQUEST;
        portio->treq.tr_time.tv_secs=secs;
        portio->treq.tr_time.tv_micro=microsecs;
        //SetCache(CACHE_DCHACHEFLUSH, portio, sizeof(struct PortIO);

        if (!DoIO(&portio->treq.tr_node))
          ret=0;
        CloseDevice(&portio->treq.tr_node);
      }
      FreeSignal(portio->port.mp_SigBit);
    }
    FreeVecPPC(portio);
  }

  return ret;
}
