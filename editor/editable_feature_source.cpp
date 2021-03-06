#include "editor/editable_feature_source.hpp"

#include "editor/osm_editor.hpp"

FeatureStatus EditableFeatureSource::GetFeatureStatus(uint32_t index) const
{
  osm::Editor & editor = osm::Editor::Instance();
  return editor.GetFeatureStatus(m_handle.GetId(), index);
}

bool EditableFeatureSource::GetModifiedFeature(uint32_t index, FeatureType & feature) const
{
  osm::Editor & editor = osm::Editor::Instance();
  return editor.GetEditedFeature(m_handle.GetId(), index, feature);
}

void EditableFeatureSource::ForEachInRectAndScale(
    m2::RectD const & rect, int scale, std::function<void(FeatureID const &)> const & fn) const
{
  osm::Editor & editor = osm::Editor::Instance();
  editor.ForEachFeatureInMwmRectAndScale(m_handle.GetId(), fn, rect, scale);
}

void EditableFeatureSource::ForEachInRectAndScale(
    m2::RectD const & rect, int scale, std::function<void(FeatureType &)> const & fn) const
{
  osm::Editor & editor = osm::Editor::Instance();
  editor.ForEachFeatureInMwmRectAndScale(m_handle.GetId(), fn, rect, scale);
}
