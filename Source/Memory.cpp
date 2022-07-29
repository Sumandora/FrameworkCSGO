#include "Memory.hpp"

#include "ReturnAddr/ReturnAddr.hpp"
#include "Utils/VMT.hpp"
#include "Interfaces.hpp"

void Memory::Create() {
	// Set the address for the return address spoofer
	if(!ret_instruction_addr) {
		ret_instruction_addr =
			Framework::ReturnAddr::leave_ret_instruction.searchPattern(
				Utils::GetVTable(Interfaces::baseClient)[0] // random code piece
			);
	}
}
