
#pragma pack(push,2)
#include <exec/tasks.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <powerpc/powerpc_protos.h>

struct newMemList {
  struct Node     nml_Node;
  UWORD           nml_NumEntries;
  struct MemEntry nml_ME[2];
};

const struct newMemList MemTemplate = {
  {0,},
  2,
  { {MEMF_CLEAR|MEMF_PUBLIC, sizeof(struct Task)},
    {MEMF_CLEAR, 0} }
};

#pragma pack(pop)

/****************************************************************************/

struct Task *CreateTask(char* name, LONG pri, APTR initpc, ULONG stacksize)
{ struct Library *SysBase = *(struct Library **)4L;
  struct newMemList nml;
  struct MemList *ml;
  struct Task *newtask;
  APTR task2;

  stacksize=(stacksize+3)&~3;

  { long *p1,*p2;
    int i;

    for (p1=(long *)&nml,p2=(long*)&MemTemplate,i=7; i; *p1++=*p2++,i--) ;
    *p1=stacksize;
  }

  ml = AllocEntry((struct MemList *)&nml);

  if (!((ULONG)ml & 1<<31)) {
    newtask=ml->ml_ME[0].me_Addr;
    newtask->tc_Node.ln_Type = NT_TASK;
    newtask->tc_Node.ln_Pri  = pri;
    newtask->tc_Node.ln_Name = name;
    newtask->tc_SPReg        = (APTR)((ULONG)ml->ml_ME[1].me_Addr+stacksize);
    newtask->tc_SPLower      = ml->ml_ME[1].me_Addr;
    newtask->tc_SPUpper      = newtask->tc_SPReg;
    NewListPPC(&newtask->tc_MemEntry);
    AddHeadPPC(&newtask->tc_MemEntry,&ml->ml_Node);
    task2=AddTask(newtask,initpc,0);
    if (SysBase->lib_Version>36 && !task2) {
      FreeEntry(ml); newtask = NULL;
    }
  }
  else
    newtask = NULL;

  return newtask;
}

void DeleteTask(struct Task *task)
{
  RemTask(task);
}
