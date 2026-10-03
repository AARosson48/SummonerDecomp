// Original: D:\projects\Summoner\pccode\Engine\Objects\objects.cpp

struct ObjNode {
	ObjNode* next;
	ObjNode* prev;
	void* object;
};

extern "C" int fn_00469DD0();
extern "C" void fn_00531CB0(const char* file, int line, const char* message);

extern "C" int fn_00469C40(void* object)
{
	if (!object)
		return -1;
	ObjNode* free_node = *(ObjNode**)0x9438ac;
	if (!free_node) {
		for (;;)
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\Engine\\Objects\\objects.cpp", 0x180, "No more free objects.  Please increase MAX_OBJECTS");
	}
	if (free_node->next != free_node) {
		*(ObjNode**)0x9438ac = free_node->next;
		free_node->prev->next = free_node->next;
		free_node->next->prev = free_node->prev;
	} else {
		*(ObjNode**)0x9438ac = 0;
	}
	free_node->next = 0;
	free_node->prev = 0;
	free_node->object = object;
	ObjNode* used = *(ObjNode**)0x9438a4;
	if (used) {
		free_node->prev = used->prev;
		free_node->next = used;
		used->prev->next = free_node;
		used->prev = free_node;
	} else {
		free_node->next = free_node;
		free_node->prev = free_node;
		*(ObjNode**)0x9438a4 = free_node;
	}
	int index = (int)((char*)free_node - 0x9438b0) / 0xc + fn_00469DD0();
	*(int*)((char*)object + 0x80) = index;
	int slot = -1;
	if ((*(int*)0x5fbfd8 & 2) && *(int*)0x651598 && (*(int*)(*(int*)0x651598 + 0x10) & 1))
		slot = index;
	*(int*)((char*)object + 0x8c) = slot;
	return index;
}
