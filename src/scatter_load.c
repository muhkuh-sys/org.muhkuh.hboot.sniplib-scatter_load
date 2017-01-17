
#include <string.h>

#include "netx_io_areas.h"
#include "rdy_run.h"
#include "systime.h"

#include "start_app_cpu.h"
#include "vectors_com_intram.h"


/*-------------------------------------------------------------------------*/

static void copy_dw(unsigned long *pulDst, const unsigned long *pulSrc, unsigned long ulSizeBytes)
{
	const unsigned long *pulSrcCnt;
	const unsigned long *pulSrcEnd;
	unsigned long ulSizeDw;


	ulSizeDw = (ulSizeBytes + 3U) >> 2U;
	pulSrcCnt = pulSrc;
	pulSrcEnd = pulSrc + ulSizeDw;
	while( pulSrcCnt<pulSrcEnd )
	{
		*(pulDst++) = *(pulSrcCnt++);
	}
}



typedef union UNION_EXEC_CHUNK
{
	struct STRUCT_EXEC_CHUNK
	{
		PFN_HBOOT_EXEC pfn;
		unsigned long ulR0;
		unsigned long ulR1;
		unsigned long ulR2;
		unsigned long ulR3;
	} s;
	unsigned long aul[5];
} UNION_EXEC_T;



void start(const unsigned long *pulCopyList);
void start(const unsigned long *pulCopyList)
{
	UNION_EXEC_T tExec;
	const unsigned long *pulCnt;
	unsigned long *pulDst;
	const unsigned long *pulSrc;
	unsigned long ulSizeBytes;
	unsigned int uiCnt;
	unsigned long ulValue;


	pulCnt = pulCopyList;
	do
	{
		/* Get the next destination. EOL is NULL. */
		pulDst = (unsigned long*)(*(pulCnt++));
		if( pulDst!=NULL )
		{
			pulSrc = (const unsigned long*)(*(pulCnt++));
			ulSizeBytes = *(pulCnt++);

			copy_dw(pulDst, pulSrc, ulSizeBytes);
		}
	} while( pulDst!=NULL );

	/* Get the start parameter. */
	for(uiCnt=0; uiCnt<5; ++uiCnt)
	{
		tExec.aul[uiCnt] = *(pulCnt++);
	}

	/* Start the APP CPU? */
	ulValue = *(pulCnt++);
	if( ulValue!=0 )
	{
		app_cpu_start();
	}

	/* Write the address to the vector. */
	tVectorsComIntram.pfnReset = tExec.s.pfn;

	__asm__("DSB");
	__asm__("ISB");

	/* Call the reset vector. */
	tVectorsComIntram.pfnReset(tExec.s.ulR0, tExec.s.ulR1, tExec.s.ulR2, tExec.s.ulR3);
}
