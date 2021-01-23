
# 1 "sralloc.c"

# 126
typedef union _SALLOC
{
unsigned char byte;
struct _BITS
{
unsigned count:7;
unsigned alloc:1;
} bits;
} SALLOC;


# 143
#pragma udata _SRAM_ALLOC_HEAP
unsigned char _uDynamicHeap[0x200];


# 153
#pragma udata _SRAM_ALLOC

# 161
 unsigned char _SRAMmerge(SALLOC *  pSegA);

# 191
unsigned char *  SRAMalloc( unsigned char nBytes)
{
SALLOC *  pHeap;
SALLOC *  temp;
 SALLOC segHeader;
 unsigned char segLen;


if (nBytes > (0x7F - 1)) return (0);


pHeap = (SALLOC *)_uDynamicHeap;

while (1)
{

segHeader = *pHeap;


segLen = segHeader.bits.count - 1;


if (segHeader.byte == 0) return (0);


if (!(segHeader.bits.alloc))
{

if (nBytes > segLen)
{

if (!(_SRAMmerge(pHeap))) pHeap += segHeader.bits.count;
}
else



if (nBytes == segLen)
{

(*pHeap).bits.alloc = 1;


return ((unsigned char *)(pHeap + 1));
}


else
{

(*pHeap).byte = nBytes + 0x81;


temp = pHeap + 1;


pHeap += (nBytes + 1);


(*pHeap).byte = segLen - nBytes;


return ((unsigned char *) temp);
}
}


else
{
pHeap += segHeader.bits.count;
}
}
}

# 287
void SRAMfree(unsigned char *  pSRAM)
{

(*(SALLOC *)(pSRAM - 1)).bits.alloc = 0;
}

# 313
void SRAMInitHeap(void)
{
unsigned char *  pHeap;
 unsigned int count;

pHeap = _uDynamicHeap;
count = 0x200-1;

while (1)
{
if (count > 0x7F)
{
*pHeap = 0x7F;
pHeap += 0x7F;
count = count - 0x7F;
}
else
{
*pHeap = count;
*(pHeap + count) = 0;
return;
}
}
}

# 359
 unsigned char _SRAMmerge(SALLOC *  pSegA)
{
SALLOC *  pSegB;
 SALLOC uSegA, uSegB, uSum;


pSegB = pSegA + (*pSegA).byte;


uSegA = *pSegA;
uSegB = *pSegB;


if (uSegB.byte == 0) return (0);


if (uSegA.bits.alloc || uSegB.bits.alloc) return (0);


if (uSegA.bits.count == 0x7F) return (0);


uSum.byte = uSegA.byte + uSegB.byte;

# 387
if ((uSum.byte) > 0x7F)
{
(*pSegA).byte = 0x7F;
pSegA += 0x7F;
pSegB += uSegB.byte;
(*pSegA).byte = pSegB - pSegA;

return (0x7F);
}


else
{
return ((*pSegA).byte = uSum.byte);
}
}
