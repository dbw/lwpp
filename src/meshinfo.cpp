#include <lwpp/meshinfo.h>

namespace lwpp
{
	size_t MeshInfo::scanPolys(MeshPolygonScan *scanner)
	{
		scanner->setMeshInfo(this);
		return scanPolys(scanner->LWPolyScanFunc, scanner);
	}
}