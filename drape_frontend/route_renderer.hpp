#pragma once

#include "drape_frontend/circles_pack_shape.hpp"
#include "drape_frontend/frame_values.hpp"
#include "drape_frontend/route_builder.hpp"

#include "shaders/program_manager.hpp"

#include "drape/drape_global.hpp"
#include "drape/pointers.hpp"

#include "geometry/screenbase.hpp"

#include <chrono>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace df
{
extern std::string const kRouteColor;
extern std::string const kRouteOutlineColor;
extern std::string const kRoutePedestrian;
extern std::string const kRouteBicycle;
extern std::string const kTransitStopInnerMarkerColor;

class RouteRenderer final
{
public:
  using CacheRouteArrowsCallback = std::function<void(dp::DrapeID, std::vector<ArrowBorders> const &)>;
  using PreviewPointsRequestCallback = std::function<void(uint32_t)>;

  struct PreviewInfo
  {
    m2::PointD m_startPoint;
    m2::PointD m_finishPoint;
  };

  struct SubrouteInfo
  {
    dp::DrapeID m_subrouteId = 0;
    SubrouteConstPtr m_subroute;
    double m_length = 0.0;
    std::vector<drape_ptr<SubrouteData>> m_subrouteData;

    drape_ptr<SubrouteArrowsData> m_arrowsData;
    std::vector<ArrowBorders> m_arrowBorders;
    float m_baseHalfWidth = 0.0f;

    drape_ptr<SubrouteMarkersData> m_markersData;
  };
  using Subroutes = std::vector<SubrouteInfo>;

  explicit RouteRenderer(PreviewPointsRequestCallback && previewPointsRequest);

  void UpdateRoute(ScreenBase const & screen, CacheRouteArrowsCallback const & callback);

  void RenderRoute(ScreenBase const & screen, bool trafficShown, ref_ptr<gpu::ProgramManager> mng,
                   FrameValues const & frameValues);

  void AddSubrouteData(drape_ptr<SubrouteData> && subrouteData, ref_ptr<gpu::ProgramManager> mng);
  Subroutes const & GetSubroutes() const;

  void RemoveSubrouteData(dp::DrapeID subrouteId);

  void AddSubrouteArrowsData(drape_ptr<SubrouteArrowsData> && subrouteArrowsData,
                             ref_ptr<gpu::ProgramManager> mng);

  void AddSubrouteMarkersData(drape_ptr<SubrouteMarkersData> && subrouteMarkersData,
                              ref_ptr<gpu::ProgramManager> mng);

  void AddPreviewRenderData(drape_ptr<CirclesPackRenderData> && renderData,
                            ref_ptr<gpu::ProgramManager> mng);

  void UpdatePreview(ScreenBase const & screen);

  void Clear();
  void ClearObsoleteData(int currentRecacheId);
  void ClearGLDependentResources();

  void UpdateDistanceFromBegin(double distanceFromBegin);
  void SetFollowingEnabled(bool enabled);

  void AddPreviewSegment(dp::DrapeID id, PreviewInfo && info);
  void RemovePreviewSegment(dp::DrapeID id);
  void RemoveAllPreviewSegments();

  void SetSubrouteVisibility(dp::DrapeID id, bool isVisible);

  bool HasTransitData() const;
  bool HasData() const;

private:
  void RenderSubroute(SubrouteInfo const & subrouteInfo, size_t subrouteDataIndex,
                      ScreenBase const & screen, bool trafficShown, ref_ptr<gpu::ProgramManager> mng,
                      FrameValues const & frameValues);
  void RenderSubrouteArrows(SubrouteInfo const & subrouteInfo, ScreenBase const & screen,
                            ref_ptr<gpu::ProgramManager> mng,
                            FrameValues const & frameValues);
  void RenderSubrouteMarkers(SubrouteInfo const & subrouteInfo, ScreenBase const & screen,
                             ref_ptr<gpu::ProgramManager> mng, FrameValues const & frameValues);
  void RenderPreviewData(ScreenBase const & screen, ref_ptr<gpu::ProgramManager> mng,
                         FrameValues const & frameValues);
  void ClearPreviewHandles();
  CirclesPackHandle * GetPreviewHandle(size_t & index);
  dp::Color GetMaskColor(RouteType routeType, double baseDistance, bool arrows) const;

  double m_distanceFromBegin;
  bool m_followingEnabled;
  Subroutes m_subroutes;
  std::unordered_set<dp::DrapeID> m_hiddenSubroutes;

  PreviewPointsRequestCallback m_previewPointsRequest;
  std::vector<drape_ptr<CirclesPackRenderData>> m_previewRenderData;
  std::vector<std::pair<CirclesPackHandle *, size_t>> m_previewHandlesCache;
  bool m_waitForPreviewRenderData;
  std::unordered_map<dp::DrapeID, PreviewInfo> m_previewSegments;
  m2::PointD m_previewPivot = m2::PointD::Zero();
};
}  // namespace df
