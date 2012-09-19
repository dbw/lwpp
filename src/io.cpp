#include <lwpp/io.h>
#include <lwpp/storeable.h>

namespace lwpp
{

	void SaveState::WriteItem(const LWBlockIdent &id, Storeable &object, bool leaf) const
	{
		Begin(id, leaf);
		object.Save(ss);
		End();
	}
	void SaveState::WriteItem(const LWBlockIdent &id, Storeable *object, bool leaf) const
	{
		Begin(id, leaf);
		object->Save(ss);
		End();
	}

	LWError LoadState::read(lwpp::Storeable &object) const
	{
		return object.Load(*this);
	}

	LWError File::Save(Storeable &item)
	{
		return item.Save(getSaveState());
	}

	LWError File::Load(Storeable &item)
	{
		return item.Load(getLoadState());
	}
}
