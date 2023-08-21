#include "../Debug.hpp"

#include "imgui.h"

#include <dlfcn.h>
#include <link.h>

static void constructInner(link_map* linkMap)
{
	if (ImGui::TreeNode(linkMap->l_name)) {
		ImGui::Text("Address: %p", reinterpret_cast<void*>(linkMap->l_addr));
		ImGui::Text("Dynamic section: %p", linkMap->l_ld);

		if (linkMap->l_prev)
			constructInner(linkMap->l_prev);
		if (linkMap->l_next)
			constructInner(linkMap->l_next);
		ImGui::TreePop();
	}
}

void Gui::Windows::linkMaps()
{
	static link_map* first = [] {
		void* handle = dlmopen(LM_ID_BASE, nullptr, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
		auto* linkMap = reinterpret_cast<link_map*>(handle);
		while (linkMap->l_prev)
			linkMap = linkMap->l_prev;
		dlclose(handle);
		return linkMap;
	}();

	link_map* linkMap = first;
	while (linkMap) {
		constructInner(linkMap);
		linkMap = linkMap->l_next;
	}
}
