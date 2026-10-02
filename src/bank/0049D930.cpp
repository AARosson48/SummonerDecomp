// Bank 0049D930. AI path nodes. The assert string in this block says
// the node table overflow should be reported to DaveA or Allender.

extern "C" {

int fn_0049D970(void)
{
	int count = *(int*)0x245c9dc + 1;
	*(int*)0x245c9dc = count;
	return count;
}

// Push a node onto the free list at 0x2452d30. The link is at +0x24.
void fn_004A0440(int* node)
{
	*(int*)((char*)node + 0x24) = *(int*)0x2452d30;
	*(int*)0x2452d30 = (int)node;
	(*(int*)0x2452d4c)--;
}

}
