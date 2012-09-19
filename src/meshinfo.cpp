#include <lwpp/meshinfo.h>

namespace lwpp
{
	int MeshInfo::scanPolys(MeshPolygonScan *scanner)
	{
		scanner->setMeshInfo(this);
		return scanPolys(scanner->LWPolyScanFunc, scanner);
	}
}