#ifndef LWPP_SPATIALQUERY_H
#define LWPP_SPATIALQUERY_H

#include <lwpp/global.h>
#include <lwspatialquery.h>
#include <vector>
#include <lwpp/item.h>
#include <lwpp/vector3d.h>

namespace lwpp 
{
  class SpatialQuery : protected GlobalBase<LWSpatialQueryFuncs>
  {
    LWSpatialQueryID mID = nullptr;

  public:
    SpatialQuery() { mID = globPtr->acquire(); }
    virtual ~SpatialQuery() { globPtr->release(mID); }

    bool raycast(LWItemID item,
      const LWDVector origin,
      const LWDVector direction,
      LWRayCastInfo* data)
    {
      auto ret = globPtr->raycast(mID, item, origin, direction, data);
#ifdef _DEBUG
      /*
      dout << "raycast( " << mID << ", " << item << ", <"
        << origin[0] << ", " << origin[1] << ", " << origin[2] << ">, <"
        << direction[0] << ", " << direction[1] << ", " << direction[2] << ">, "
        << data << ") = " << ret << "\n";
      dout << "LWRayCastInfo.polygon = " << data->polygon << ", .hitpoint = <"
        << data->hitpoint[0] << ", " << data->hitpoint[1] << ", " << data->hitpoint[2] << ">, .distance = "
        << data->distance << " , .item = " << data->item << "\n";
        */
#endif 
      
      return (ret != 0);
    }

    bool raycast(lwpp::LWItem &item,
      const lwpp::Point3d &origin,
      const lwpp::Vector3d &direction,
      LWRayCastInfo* data)
    {
      return raycast(item.GetID() , origin.asLWVector(), direction.asLWVector(), data);
    }

    std::vector<LWItemID> getItems()
    {
      auto n = globPtr->numItems();
      std::vector<LWItemID> ret;
      if (n)
      {
        ret.resize(n);
        globPtr->items(&ret[0]);
      }
      return ret;
    }

    void clear()
    {
      auto it = getItems();
      for (auto i : it)
        globPtr->destroy(i);
    }

    void update(LWItemID id) { globPtr->update(id); }
    void destroy(LWItemID id) { globPtr->destroy(id); }
    void trace(LWItemID id) { globPtr->trace(id); }
    void untrace(LWItemID id) { globPtr->untrace(id); }

  };
  inline bool intersect(lwpp::Point3d orig, lwpp::Vector3d dir, lwpp::Vector3d min, lwpp::Vector3d max)
  {
    float tmin = (min.x - orig.x) / dir.x;
    float tmax = (max.x - orig.x) / dir.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - orig.y) / dir.y;
    float tymax = (max.y - orig.y) / dir.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    if (tymin > tmin)
      tmin = tymin;

    if (tymax < tmax)
      tmax = tymax;

    float tzmin = (min.z - orig.z) / dir.z;
    float tzmax = (max.z - orig.z) / dir.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
      return false;

    if (tzmin > tmin)
      tmin = tzmin;

    if (tzmax < tmax)
      tmax = tzmax;

    return true;
  }

  // check the ray against all bounding boxes
  inline std::vector<LWItem> findCameraItems(const Point3d origin, const Vector3d direction)
  {
    LWItem item(LWI_OBJECT);
    std::vector<LWItem> ret;
    auto t = GetTime();
    while (item.exists())
    {
      if ((item.Flags() & LWITEMF_ACTIVE) == LWITEMF_ACTIVE)
      {
        ObjectInfo object(item.GetID());
        if (((object.flags() & LWOBJF_UNSEEN_BY_CAMERA) == 0) && object.numPolygons() > 0)
        {
          Vector3d min, max;
          object.bounds(min.asLWVector(), max.asLWVector());
          if (intersect(origin, direction, min, max))
          {
            ret.push_back(item);
          }
        }
      }
      item.Next();
    }
    return ret;
  }

  // check the ray against all bounding boxes
  inline std::vector<LWItem> findItems(const Point3d origin, const Vector3d direction, bool includeHidden = false)
  {
    LWItem item(LWI_OBJECT);
    std::vector<LWItem> ret;
    auto t = GetTime();
    while (item.exists())
    {
      if (((item.Flags() & LWITEMF_ACTIVE) == LWITEMF_ACTIVE) | includeHidden)
      {
        ObjectInfo object(item.GetID());
        if (object.numPolygons() > 0)
        {
          Vector3d min, max;
          object.bounds(min.asLWVector(), max.asLWVector());
          // LWIP_PIVOT
          // LWIP_PIVOT_ROT
          if (intersect(origin, direction, min, max))
          {
            ret.push_back(item);
          }
        }
      }
      item.Next();
    }
    return ret;
  }
} // end namespace
#endif // LWPP_SPATIALQUERY_H