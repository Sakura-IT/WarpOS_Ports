
#pragma pack(push,2)
#include <exec/types.h>
#pragma pack(pop)

/****************************************************************************/

ULONG FastRand(ULONG seed)
{ ULONG a=seed<<1;
  if((LONG)seed<=0)
    a^=0x1d872b41;
  return a;
}
