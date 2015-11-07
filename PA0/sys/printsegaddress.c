#include <stdio.h>

extern int etext, edata, end;

void printsegaddress()
{
	kprintf( "Call function printsegaddress.\n\n" );
	kprintf( "The address of end of text segment: 0x%08x\n", &etext );
	kprintf( "The 4-byte contents preceding the end of text segment: 0x%08x\n", *(&etext-1) );
	kprintf( "The 4-byte contents after the end of text segment: 0x%08x\n\n", *(&etext+1) );
	kprintf( "The address of end of data segment: 0x%08x\n", &edata );
	kprintf( "The 4-byte contents preceding the end of data segment: 0x%08x\n", *(&edata-1) );
	kprintf( "The 4-byte contents after the end of data segment: 0x%08x\n\n", *(&edata+1) );
	kprintf( "The address of end of BSS segment: 0x%08x\n", &end );
	kprintf( "The 4-byte contents preceding the end of BSS segment: 0x%08x\n", *(&end-1) );
	kprintf( "The 4-byte contents after the end of BSS segment: 0x%08x\n\n", *(&end+1) );
}
