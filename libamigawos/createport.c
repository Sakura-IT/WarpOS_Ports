
#pragma pack(push,2)
#include <exec/types.h>
#include <proto/exec.h>
#include <powerpc/powerpc.h>
#include <powerpc/powerpc_protos.h>
#pragma pack(pop)

/****************************************************************************/

struct MsgPort *CreatePort (STRPTR name, LONG pri)
{
    struct MsgPort *port = NULL;
    UBYTE portsig;

    if ((BYTE)(portsig = AllocSignal (-1)) >= 0) {
	if (!(port = AllocVecPPC (sizeof (struct MsgPort), MEMF_CLEAR | MEMF_PUBLIC, 32L)))
	    FreeSignal(portsig);
	else {
	    port->mp_Node.ln_Type = NT_MSGPORT;
	    port->mp_Node.ln_Pri  = pri;
	    port->mp_Node.ln_Name = (char *)name;
	    port->mp_Flags = PA_SIGNAL;
	    port->mp_SigBit  = portsig;
	    port->mp_SigTask = FindTask (NULL);
	    NewListPPC (&port->mp_MsgList);
	    //SetCache(CACHE_DCACHEFLUSH, port, sizeof (struct MsgPort));
	    if (port->mp_Node.ln_Name)
		AddPort(port);
	}
    }
    return port;
}

void DeletePort (struct MsgPort *port)
{
    int i;

    if (port->mp_Node.ln_Name != NULL)
	RemPort (port);
    i = -1;
    port->mp_Node.ln_Type = i;
    port->mp_MsgList.lh_Head = (struct Node *)i;
    FreeSignal (port->mp_SigBit);
    FreeVecPPC (port);
}
