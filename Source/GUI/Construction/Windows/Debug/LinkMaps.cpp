#include "../Debug.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include <dlfcn.h>
#include <link.h>

static void ConstructInner(link_map* linkMap)
{
	if (ImGui::TreeNode(linkMap->l_name)) {
		ImGui::Text(xorstr_("Address: %p"), linkMap->l_addr);
		ImGui::Text(xorstr_("Dynamic section: %p"), linkMap->l_ld);

		if (linkMap->l_prev)
			ConstructInner(linkMap->l_prev);
		if (linkMap->l_next)
			ConstructInner(linkMap->l_next);
		ImGui::TreePop();
	}
}

void Gui::Windows::LinkMaps()
{
	static link_map* first = [] {
		void* handle = dlmopen(LM_ID_BASE, nullptr, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
		link_map* linkMap = reinterpret_cast<link_map*>(handle);
		while (linkMap->l_prev)
			linkMap = linkMap->l_prev;
		dlclose(handle);
		return linkMap;
	}();

	link_map* linkMap = first;
	while (linkMap) {
		ConstructInner(linkMap);
		linkMap = linkMap->l_next;
	}
}
