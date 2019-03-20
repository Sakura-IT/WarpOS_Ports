
#pragma pack(push,2)
#include <clib/alib_protos.h>
#include <proto/graphics.h>
#pragma pack(pop)

/****************************************************************************/

void waitbeam(LONG pos)
{
  do{}while(pos>VBeamPos());
}
