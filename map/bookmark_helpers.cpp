#include "map/bookmark_helpers.hpp"

#include "kml/serdes.hpp"
#include "kml/serdes_binary.hpp"

#include "coding/file_reader.hpp"
#include "coding/file_writer.hpp"

std::unique_ptr<kml::FileData> LoadKmlFile(std::string const & file, bool useBinary)
{
  auto kmlData = LoadKmlData(FileReader(file), useBinary);

  if (kmlData == nullptr)
    LOG(LWARNING, ("Loading bookmarks failed, file", file));
  return kmlData;
}

std::unique_ptr<kml::FileData> LoadKmlData(Reader const & reader, bool useBinary)
{
  auto data = std::make_unique<kml::FileData>();
  try
  {
    if (useBinary)
    {
      kml::binary::DeserializerKml des(*data.get());
      des.Deserialize(reader);
    }
    else
    {
      kml::DeserializerKml des(*data.get());
      des.Deserialize(reader);
    }
  }
  catch (Reader::Exception const & e)
  {
    LOG(LWARNING, ("KML", useBinary ? "binary" : "text", "reading failure:", e.what()));
    return nullptr;
  }
  catch (std::exception const & e)
  {
    LOG(LWARNING, ("KML", useBinary ? "binary" : "text", "deserialization failure:", e.what()));
    return nullptr;
  }
  return data;
}

bool SaveKmlFile(kml::FileData & kmlData, std::string const & file, bool useBinary)
{
  bool success = false;
  try
  {
    FileWriter writer(file);
    success = SaveKmlData(kmlData, writer, useBinary);
  }
  catch (std::exception const & e)
  {
    LOG(LWARNING, ("KML", useBinary ? "binary" : "text", "saving failure:", e.what()));
    success = false;
  }
  if (!success)
    LOG(LWARNING, ("Saving bookmarks failed, file", file));
  return success;
}

bool SaveKmlData(kml::FileData & kmlData, Writer & writer, bool useBinary)
{
  try
  {
    if (useBinary)
    {
      kml::binary::SerializerKml ser(kmlData);
      ser.Serialize(writer);
    }
    else
    {
      kml::SerializerKml ser(kmlData);
      ser.Serialize(writer);
    }
  }
  catch (Writer::Exception const & e)
  {
    LOG(LWARNING, ("KML", useBinary ? "binary" : "text", "writing failure:", e.what()));
    return false;
  }
  catch (std::exception const & e)
  {
    LOG(LWARNING, ("KML", useBinary ? "binary" : "text", "serialization failure:", e.what()));
    return false;
  }
  return true;
}
