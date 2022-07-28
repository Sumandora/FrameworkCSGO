#include "Netvars.hpp"
#include "Interfaces.hpp"
#include "Utils/VMT.hpp"
#include "SDK/ClientClass.hpp"

void ReadTable(RecvTable* recvTable, int depth) {
	for(int i = 0; i < depth; i++)
		printf("\t");
	printf("%s\n", recvTable->m_pNetTableName);

	for(int i = 0; i < recvTable->m_nProps; i++) {
		RecvProp* prop = &recvTable->m_pProps[i];
		if(!prop) continue;

		for(int i = 0; i < depth + 1; i++)
			printf("\t");
		printf("%s\t0x%x\n", prop->m_pVarName, prop->m_Offset);
		
		if(prop->m_pDataTable)
			ReadTable(prop->m_pDataTable, depth + 1);
	}
	
}

void Netvars::DumpNetvars() {
	/*
	 * GetAllClasses assembly:
	 *	mov    0x1988841(%rip),%rax	; Take RIP + offset and dereference it into RAX
	 *	push   %rbp					; Create Stackframe	|These
	 *	mov    %rsp,%rbp			; Create Stackframe	|Instructions
	 *	pop    %rbp					; Destroy Stackframe|Are Useless
	 *	mov    (%rax),%rax			; Dereference it again
	 *	ret
	 */
	 
	void* getAllClasses = Utils::GetVTable(Interfaces::baseClient)[8];
	int* ripOffset = reinterpret_cast<int*>(static_cast<char*>(getAllClasses) + 3);
	void* addr = static_cast<char*>(reinterpret_cast<void*>(ripOffset)) + 4 + *ripOffset;
	addr = **reinterpret_cast<void***>(addr);

	for(ClientClass* cClass = reinterpret_cast<ClientClass*>(addr); cClass != nullptr; cClass = cClass->m_pNext) {
		RecvTable* table = cClass->m_pRecvTable;
		printf("%s\n", cClass->m_pNetworkName);
		ReadTable(table, 1);
	}
	
	__asm ("int3");
}
