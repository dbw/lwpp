#ifndef LWPP_SPATIALQUERY_H
#define LWPP_SPATIALQUERY_H

#include <lwpp/global.h>
#include <lwspatialquery.h>
#include <vector>
#include <lwpp/item.h>
#include <lwpp/vector3d.h>

namespace lwpp 
{
  //! Class to add support for the Communication Ring to plugins
  //! @ingroup Entities
  /*!
   * You must inherit this class in EVERY plugin where you'd want to use a ComRing
   */
  //constexpr char SpatialQueryName[] = LWSPATIALQUERYFUNCS_SERVICE;
  //class SpatialQuery : protected SimpleGlobal<LWSpatialQueryFuncs, SpatialQueryName>
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
      dout << "raycast( " << mID << ", " << item << ", <"
        << origin[0] << ", " << origin[1] << ", " << origin[2] << ">, <"
        << direction[0] << ", " << direction[1] << ", " << direction[2] << ">, "
        << data << ") = " << ret << "\n";
      dout << "LWRayCastInfo.polygon = " << data->polygon << ", .hitpoint = <"
        << data->hitpoint[0] << ", " << data->hitpoint[1] << ", " << data->hitpoint[2] << ">, .distance = "
        << data->distance << " , .item = " << data->item << "\n";
#endif 
      return (ret != 0);
    }

    bool raycast(lwpp::LWItem &item,
      const lwpp::Vector3d &origin,
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

} // end namespace
#endif // LWPP_SPATIALQUERY_H