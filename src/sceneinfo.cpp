#include <lwpp/sceneinfo.h>
namespace lwpp
{

	const char *SceneInfo::filterNames[] = 
	{
		"Classic", 
		"Box", "Box (Sharp)", "Box (Soft)", 
		"Gaussian", "Gaussian (Sharp)", "Gaussian (Soft)", 
		"Mitchell", "Mitchell (Sharp)", "Mitchell (Soft)", 
		"Lanczos", "Lanczos (Sharp)", "Lanczos (Soft)",
		"unknown"
	};

	Matrix4x4d getWorld2Screen(LWItem cid, LWTime time)
	{
		if (lwpp::LightWave::isModeler()) return Matrix4x4d();
		lwpp::Vector3d u, v, n, r;
		cid.Param(LWIP_RIGHT,      time, u);
		cid.Param(LWIP_UP,         time, v);
		cid.Param(LWIP_FORWARD,    time, n);
		cid.Param(LWIP_W_POSITION, time, r);
		CameraInfo ci(cid.GetID());
		double e = -ci.zoomFactor(time);

		return Matrix4x4d	(	u.x, v.x, n.x, -n.x / e,
												u.y, v.y, n.y, -n.y / e,
												u.z, v.z, n.z, -n.z / e,
											 -r.Dot(u), -r.Dot(v), -r.Dot(n) + e, 1.0 - ((-r.Dot(n) + e) / e) );
	}

	Matrix4x4d getWorld2Screen(LWItemID id, LWTime time)
	{
		return getWorld2Screen(LWItem (id), time);
	}

	Matrix4x4d getWorld2Screen(LWTime time)
	{
		lwpp::SceneInfo si;
		return getWorld2Screen(si.renderCamera( time ), time);
	}

	bool World2Screen(lwpp::Point3d pt, double scoord[2], lwpp::Matrix4x4d &CameraTransform, double frameAspect)
	{
		if (lwpp::LightWave::isModeler()) return false;
		CameraTransform(&pt);
		scoord[0] = 0.5 + pt.x * 0.5 / frameAspect;
		scoord[1] = 0.5 - pt.y * 0.5;
		return true;
	}

	bool World2Screen(lwpp::Vector3d dir, double scoord[2], lwpp::Matrix4x4d &CameraTransform, double frameAspect)
	{
		if (lwpp::LightWave::isModeler()) return false;
		//lwpp::SceneInfo si;
		//double w = CameraTransform(&pt);
		CameraTransform(dir);
		//double zDist = w * pt.z - e;
		//Convert from meters (on the projection plane) to pixels. 
		scoord[0] = dir.x * 0.5 / frameAspect;
		scoord[1] = dir.y * 0.5;
		return true;
	}

	bool World2Screen(lwpp::Point3d pt, double scoord[2], LWItemID camID, LWTime time, double pixelAspect)
	{
		if (lwpp::LightWave::isModeler()) return false;
		lwpp::Matrix4x4d transform = getWorld2Screen(camID, time);
		return World2Screen(pt, scoord, transform, pixelAspect);
	}

	bool World2Screen(lwpp::Point3d pt, double scoord[2], LWTime time)
	{
		lwpp::SceneInfo si;
		return World2Screen(pt, scoord, si.renderCamera( time ), time, si.pixelAspect()); 
	}
}