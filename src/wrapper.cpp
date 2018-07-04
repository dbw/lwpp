#include <lwpp/wrapper.h>

namespace lwpp
{
	size_t ContextMenuCallback::countFn (void *inst)
	{
		if (!inst) return 0;
		ContextMenuCallback *cmc = static_cast<ContextMenuCallback *> (inst);
		return cmc->count();
	}

// #pragma optimize( "gy", off )
	const char* ContextMenuCallback::nameFn (void *inst, int n)
	{
		if (!inst) return 0;			
		ContextMenuCallback *cmc = static_cast<ContextMenuCallback *> (inst);
		return const_cast<char*>(cmc->name(n));
	}
}