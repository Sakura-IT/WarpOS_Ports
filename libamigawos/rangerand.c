
#pragma pack(push,2)
#include <exec/types.h>
#pragma pack(pop)

/****************************************************************************/

ULONG RangeSeed;

ULONG RangeRand(ULONG maxValue)
{ ULONG a=RangeSeed;
  UWORD i=maxValue-1;
  do
  { ULONG b=a;
    a<<=1;
    if((LONG)b<=0)
      a^=0x1d872b41;
  }while((i>>=1));
  RangeSeed=a;
  if((UWORD)maxValue)
    return (UWORD)((UWORD)a*(UWORD)maxValue>>16);
  return (UWORD)a;
}
