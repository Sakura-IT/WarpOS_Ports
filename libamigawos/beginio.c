
#pragma pack(push,2)
#include <exec/io.h>
#include <exec/devices.h>
#include <powerpc/powerpc_protos.h>
#include <powerpc/powerpc.h>
#pragma pack(pop)

/****************************************************************************/

void BeginIO(struct IORequest *iorequest)
{
	struct PPCArgs pa;
	pa.PP_Code = (ULONG*) iorequest->io_Device;
	pa.PP_Offset = DEV_BEGINIO;
	pa.PP_Stack = 0;
	pa.PP_StackSize = 0;
	pa.PP_Regs[PPREG_A1] = (ULONG) iorequest;
	pa.PP_Regs[PPREG_A6] = (ULONG) iorequest->io_Device;
	Run68K(&pa);
	return;
}	