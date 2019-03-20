
#pragma pack(push,2)
#include <exec/types.h>
#include <proto/exec.h>
#include <powerpc/powerpc.h>
#include <powerpc/powerpc_protos.h>
#pragma pack(pop)

/****************************************************************************/

struct IORequest *CreateExtIO (struct MsgPort *port, long iosize)
{
    struct IORequest *ioreq=NULL;

    if (port && (ioreq = AllocVecPPC (iosize, MEMF_CLEAR | MEMF_PUBLIC, 32L))) {
	ioreq->io_Message.mn_Node.ln_Type = NT_REPLYMSG;
	ioreq->io_Message.mn_ReplyPort    = port;
	ioreq->io_Message.mn_Length       = iosize;
	//SetCache(CACHE_DCACHEFLUSH, ioreq, iosize);
    }
    return ioreq;
}

struct IOStdReq *CreateStdIO (struct MsgPort *port)
{
    return (struct IOStdReq *)CreateExtIO (port, sizeof (struct IOStdReq));
}

void DeleteExtIO (struct IORequest *ioreq)
{
    int i;

    i=-1;
    ioreq->io_Message.mn_Node.ln_Type = i;
    ioreq->io_Device = (struct Device *)i;
    ioreq->io_Unit = (struct Unit *)i;
    FreeVecPPC (ioreq);
}

void DeleteStdIO (struct IORequest *ioreq)
{
    DeleteExtIO (ioreq);
}
