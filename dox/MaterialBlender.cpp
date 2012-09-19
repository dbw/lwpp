#include <lwpp/node_handler.h>
#include <lwpp/vparm.h>

const char pluginName[] = "MaterialBlender";

#define IO_MODE LWID_('M','O','D','E')	//!<
#define IO_OPAC LWID_('O','P','A','C')	//!<

static LWBlockIdent idcomp[] =
{
	IO_MODE, "Mode", // 0
	IO_OPAC, "Opacity", // 1
	0
};

class MaterialComponent : public lwpp::Storeable
{
public:
	lwpp::LWNodeInput *inMode;
	lwpp::LWNodeInput *inOpacity;
	lwpp::LWNodeUtility nodeUtil;
	lwpp::VParm *Opacity;
	BlendingMode Mode;
	MaterialComponent()
		: Mode(Blend_Normal),
			inMode(0), inOpacity(0),
			Opacity(0)
	{
		;
	}
	MaterialComponent &operator=(const MaterialComponent &from)
	{
		*Opacity = *from.Opacity;
		Mode = from.Mode;
		return *this;
	}
	void Initialize(lwpp::NodeHandler &node, std::string name)
	{
		inOpacity = node.addScalarInput(name + " Opacity");
		inMode = node.addIntegerInput(name + " Mode");
		Opacity = new lwpp::VParm(LWVP_PERCENT , name+"Opacity", pluginName, node.chanGrp());
	}
	void evaluate(LWNodalAccess *na, LWDVector bg, LWDVector fg, LWDVector &out)
	{
		double opacity = Opacity->GetValue();
		inOpacity->evaluate(na, opacity);

		int mode = (int)Mode;

		inMode->evaluate(na, mode);
		nodeUtil.Blend(out, bg, fg, opacity, (BlendingMode)lwpp::Clamp(mode, 0, 13));
	}
	void evaluate(LWNodalAccess *na, double bg, double fg, double &out)
	{
		double opacity = Opacity->GetValue();
		inOpacity->evaluate(na, opacity);

		int mode = (int)Mode;
		inMode->evaluate(na, mode);

		LWDVector f = {fg, 0.0, 0.0};
		LWDVector b = {bg, 0.0, 0.0};
		LWDVector o = {out, 0.0, 0.0};

		nodeUtil.Blend(o, b, f, opacity, (BlendingMode)lwpp::Clamp(mode, 0, 13));
		out = o[0];
	}
	void *ModeGet ()
	{
		if (!inMode->isConnected()) return &Mode;
		return 0;
	}
	void *OpacityGet ()
	{
		if (!inOpacity->isConnected()) return Opacity->ID();
		return 0;
	}
	LWError NewTime (LWFrame frame, LWTime time)
	{
		Opacity->NewTime(time);
		return 0;
	}
	virtual LWError	Load (const lwpp::LoadState &ls)
	{
		LWError err = 0;
		while(LWID id = ls.Find(idcomp))
		{
			switch( id )
			{
				case IO_MODE: Mode = (BlendingMode)ls.readInt(); break;

				case IO_OPAC: ls.read(*Opacity); break;

				default: break;
			}
			ls.End();
		}
		return err;
	}
	virtual LWError	Save (const lwpp::SaveState &ss)
	{
		LWError err = 0;
		ss.WriteBlock(idcomp[0], (int)Mode);
		ss.WriteItem(idcomp[1], *Opacity);
		return 0;
	}
};

class MaterialMixer : public lwpp::XPanelNodeHandler
{
	private:
		lwpp::LWNodeUtility nodeUtil;
		lwpp::LWNodeOutput *outMaterial;
		lwpp::LWNodeOutput *outDiffuse, *outSpecular, *outReflection, *outRefraction, *outTransparency;
		lwpp::LWNodeInput *inBaseMaterial, *inTopMaterial;
		lwpp::LWNodeInput *inBaseDiffuse, *inBaseSpecular, *inBaseReflection, *inBaseRefraction, *inBaseTransparency;
		lwpp::LWNodeInput *inTopDiffuse, *inTopSpecular, *inTopReflection, *inTopRefraction, *inTopTransparency;
		MaterialComponent Diffuse, Specular, Reflection, Refraction, Transparency;
	public:
	MaterialMixer (void *priv, void *context, LWError *err);
	~MaterialMixer ();
	virtual void 	Evaluate (LWNodalAccess *na, NodeOutputID outID, NodeValue value);
	virtual int 	Interface (int version, LWInterface *local, void *serverdata);
	virtual void *DataGet (unsigned int vid);
	virtual LWXPRefreshCode DataSet (unsigned int vid, void *value);
	virtual MaterialMixer &operator=(const MaterialMixer &from);
	virtual LWError NewTime (LWFrame frame, LWTime time);
	virtual void 	ChangeNotify (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type);
	virtual LWError	Load (const lwpp::LoadState &ls);
	virtual LWError	Save (const lwpp::SaveState &ss);
};

ServerTagInfo tags[] =
{
	{"Material Blender",SRVTAG_USERNAME},
	{"db&w/Materials",SRVTAG_NODEGROUP},
	{ NULL }
};
lwpp::XPanelNodeAdaptor<MaterialMixer> mixerInstance(pluginName, tags);

MaterialMixer::MaterialMixer (void *priv, void *context, LWError *err)
		: lwpp::XPanelNodeHandler(priv, context, err)
{
	if ( (lwpp::LightWave::Major() < 9) ||
	     ( (lwpp::LightWave::Major() == 9) && (lwpp::LightWave::Minor() < 2) ) )
	{
		*err = "This plugin needs a newer version of LightWave 3D (9.2 as a minimum)";
		return;
	}

	inBaseMaterial =	addMaterialInput("Fg Material");
	inBaseDiffuse =	addColourInput("Fg Diffuse");
	inBaseSpecular =	addColourInput("Fg Specular");
	inBaseReflection =	addColourInput("Fg Reflection");
	inBaseRefraction =	addColourInput("Fg Refraction");
	inBaseTransparency =	addScalarInput("Fg Transparency");

	inTopMaterial =	addMaterialInput("Bg Material");
	inTopDiffuse =	addColourInput("Bg Diffuse");
	inTopSpecular =	addColourInput("Bg Specular");
	inTopReflection =	addColourInput("Bg Reflection");
	inTopRefraction =	addColourInput("Bg Refraction");
	inTopTransparency =	addScalarInput("Bg Transparency");

	outMaterial =	addMaterialOutput("Material");
	outDiffuse =	addColorOutput("Diffuse");
	outSpecular =	addColorOutput("Specular");
	outReflection =	addColorOutput("Reflection");
	outRefraction =	addColorOutput("Refraction");
	outTransparency =	addScalarOutput("Transparency");

	Diffuse.Initialize(*this, "Diffuse");
	Specular.Initialize(*this, "Specular");
	Reflection.Initialize(*this, "Reflection");
	Refraction.Initialize(*this, "Refraction");
	Transparency.Initialize(*this, "Transparency");
}

MaterialMixer::~MaterialMixer ()
{
	;
}

MaterialMixer &MaterialMixer::operator=(const MaterialMixer &from)
{
	if (&from != this)
	{
		Diffuse = from.Diffuse;
		Specular = from.Specular;
		Reflection = from.Reflection;
		Refraction = from.Refraction;
		Transparency = from.Transparency;
	}
	return *this;
}

LWError MaterialMixer::NewTime (LWFrame frame, LWTime time)
{
	Diffuse.NewTime(frame, time);
	Specular.NewTime(frame, time);
	Reflection.NewTime(frame, time);
	Refraction.NewTime(frame, time);
	Transparency.NewTime(frame, time);
	return 0;
}

void 	MaterialMixer::Evaluate (LWNodalAccess *na, NodeOutputID outID, NodeValue value)
{
	LWNodalMaterial bgMat = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},0};
	LWNodalMaterial fgMat = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},0};
	LWNodalMaterial outMat= {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},0};

	inBaseMaterial->evaluate(na, bgMat);
	inTopMaterial->evaluate(na, fgMat);

	if (outMaterial->isID(outID))
	{
		inBaseDiffuse->evaluate(na, bgMat.diffuse);
		inBaseSpecular->evaluate(na, bgMat.specular);
		inBaseReflection->evaluate(na, bgMat.reflection);
		inBaseRefraction->evaluate(na, bgMat.refraction);
		inBaseTransparency->evaluate(na, bgMat.transparency);

		inTopDiffuse->evaluate(na, fgMat.diffuse);
		inTopSpecular->evaluate(na, fgMat.specular);
		inTopReflection->evaluate(na, fgMat.reflection);
		inTopRefraction->evaluate(na, fgMat.refraction);
		inTopTransparency->evaluate(na, fgMat.transparency);

		Diffuse.evaluate(na, bgMat.diffuse, fgMat.diffuse, outMat.diffuse);
		Specular.evaluate(na, bgMat.specular, fgMat.specular, outMat.specular);
		Reflection.evaluate(na, bgMat.reflection, fgMat.reflection, outMat.reflection);
		Refraction.evaluate(na, bgMat.refraction, fgMat.refraction, outMat.refraction);
		Transparency.evaluate(na, bgMat.transparency, fgMat.transparency, outMat.transparency);

		setValue(value, outMat);
	}
	else if (outDiffuse->isID(outID))
	{
		inBaseDiffuse->evaluate(na, bgMat.diffuse);
		inTopDiffuse->evaluate(na, fgMat.diffuse);
		Diffuse.evaluate(na, bgMat.diffuse, fgMat.diffuse, outMat.diffuse);
		setValue(value, outMat.diffuse);
	}
	else if (outSpecular->isID(outID))
	{
		inBaseSpecular->evaluate(na, bgMat.specular);
		inTopSpecular->evaluate(na, fgMat.specular);
		Specular.evaluate(na, bgMat.specular, fgMat.specular, outMat.specular);
		setValue(value, outMat.specular);
	}
	else if (outReflection->isID(outID))
	{
		inBaseReflection->evaluate(na, bgMat.reflection);
		inTopReflection->evaluate(na, fgMat.reflection);
		Reflection.evaluate(na, bgMat.reflection, fgMat.reflection, outMat.reflection);
		setValue(value, outMat.reflection);
	}
	else if (outRefraction->isID(outID))
	{
		inBaseRefraction->evaluate(na, bgMat.refraction);
		inTopRefraction->evaluate(na, fgMat.refraction);
		Refraction.evaluate(na, bgMat.refraction, fgMat.refraction, outMat.refraction);
		setValue(value, outMat.refraction);
	}
	else if (outTransparency->isID(outID))
	{
		inBaseTransparency->evaluate(na, bgMat.transparency);
		inTopTransparency->evaluate(na, fgMat.transparency);
		Transparency.evaluate(na, bgMat.transparency, fgMat.transparency, outMat.transparency);
		setValue(value, outMat.transparency);
	}
}

enum { BDiff = 0x9000, MDiff, BSpec, MSpec, BRefl, MRefl, BRefr, MRefr, BTran, MTran};

static char *blendModeS[] =
{
	"Normal", "Additive", "Subtractive", "Multiply", "Screen", "Darken", "Lighten", "Difference", "Negative", "Colour Dodge", "Colour Burn", "Red", "Green", "Blue",	0
};

int MaterialMixer::Interface (int version, LWInterface *local, void *serverdata)
{
	static LWXPanelControl ctrl[] =
	{
		{MDiff, "Diffuse Mode", "iPopChoice"},
		{BDiff, "Diffuse Opacity", "percent-env"},
		{MSpec, "Specular Mode", "iPopChoice"},
		{BSpec, "Specular Opacity", "percent-env"},
		{MRefl, "Reflection Mode", "iPopChoice"},
		{BRefl, "Reflection Opacity", "percent-env"},
		{MRefr, "Refraction Mode", "iPopChoice"},
		{BRefr, "Refraction Opacity", "percent-env"},
		{MTran, "Transparency Mode", "iPopChoice"},
		{BTran, "Transparency Opacity", "percent-env"},
		0
	};
	static LWXPanelDataDesc desc[] =
	{
		{MDiff, "Diffuse Mode", "integer"},
		{BDiff, "Diffuse Opacity", "float-env"},
		{MSpec, "Specular Mode", "integer"},
		{BSpec, "Specular Opacity", "float-env"},
		{MRefl, "Reflection Mode", "integer"},
		{BRefl, "Reflection Opacity", "float-env"},
		{MRefr, "Refraction Mode", "integer"},
		{BRefr, "Refraction Opacity", "float-env"},
		{MTran, "Transparency Mode", "integer"},
		{BTran, "Transparency Opacity", "float-env"},
	};

	static LWXPanelHint hints[] =
	{
		XpSTRLIST(MDiff, blendModeS),
		XpSTRLIST(MSpec, blendModeS),
		XpSTRLIST(MRefl, blendModeS),
		XpSTRLIST(MRefr, blendModeS),
		XpSTRLIST(MTran, blendModeS),
		XpDIVADD(BDiff),
		XpDIVADD(BSpec),
		XpDIVADD(BRefl),
		XpDIVADD(BRefr),
		XpEND,
	};

	CreateViewXPanel (ctrl, desc, hints);
	local->panel = LW_XPanel.getID();
	return AFUNC_OK;
}

void *MaterialMixer::DataGet (unsigned int vid)
{
	switch (vid)
	{
		case MDiff:	return Diffuse.ModeGet();	break;
		case MSpec: return Specular.ModeGet(); break;
		case MRefl: return Reflection.ModeGet(); break;
		case MRefr: return Refraction.ModeGet(); break;
		case MTran: return Transparency.ModeGet(); break;

		case BDiff:	return Diffuse.OpacityGet();	break;
		case BSpec: return Specular.OpacityGet(); break;
		case BRefl: return Reflection.OpacityGet(); break;
		case BRefr: return Refraction.OpacityGet(); break;
		case BTran: return Transparency.OpacityGet(); break;

		default:
			break;
	}
	return 0;
}

LWXPRefreshCode MaterialMixer::DataSet (unsigned int vid, void *value)
{
	int *i = static_cast<int *>(value);
	switch (vid)
	{
		case MDiff: Diffuse.Mode = (BlendingMode)*i; break;
		case MSpec: Specular.Mode = (BlendingMode)*i; break;
		case MRefl: Reflection.Mode = (BlendingMode)*i; break;
    case MRefr: Refraction.Mode = (BlendingMode)*i; break;
    case MTran: Transparency.Mode = (BlendingMode)*i; break;
		default:
			break;
	}
	return LWXPRC_DFLT;
}

 void MaterialMixer::ChangeNotify (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type)
 {
		if (event_type == LWXPEVENT_TRACK)
		{
			UpdateNodePreview();
		}
		else if (event_type == LWXPEVENT_VALUE)
		{
			Update();
		}
 }

// defines etc. for file i/o
#define IO_CDIF LWID_('C','D','I','F')	//!<
#define IO_CSPC LWID_('C','S','P','C')	//!<
#define IO_CRFL LWID_('C','R','F','L')	//!<
#define IO_CRFR LWID_('C','R','F','R')	//!<
#define IO_CTRN LWID_('C','T','R','N')	//!<

static LWBlockIdent idroot[] =
{
	IO_CDIF, "Diffuse", // 0
	IO_CSPC, "Specular", // 1
	IO_CRFL, "Reflection", // 2
	IO_CRFR, "Refrection", // 3
	IO_CTRN, "Transparency", // 4
	0
};

LWError	MaterialMixer::Load (const lwpp::LoadState &ls)
{
	LWError err = 0;
	while(LWID id = ls.Find(idroot))
	{
		switch( id )
		{
			case IO_CDIF: ls.read(Diffuse); break;

	    case IO_CSPC: ls.read(Specular); break;

	    case IO_CRFL: ls.read(Reflection); break;

	    case IO_CRFR: ls.read(Refraction); break;

			case IO_CTRN: ls.read(Transparency); break;

			default: break;
		}
		ls.End();
	}
	return err;
}
LWError	MaterialMixer::Save (const lwpp::SaveState &ss)
{
	LWError err = 0;
	ss.WriteItem(idroot[0], Diffuse);
	ss.WriteItem(idroot[1], Specular);
	ss.WriteItem(idroot[2], Reflection);
	ss.WriteItem(idroot[3], Refraction);
	ss.WriteItem(idroot[4], Transparency);
	return 0;
}