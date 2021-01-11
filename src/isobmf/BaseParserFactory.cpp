
#include "BaseParserFactory.h"
#include "IsoBmfUtils.h"
#include "ContainerBoxParser.h"
#include "FileTypeBoxParser.h"
#include "FreeSpaceBoxParser.h"
#include "MovieDataBoxParser.h"
#include "MovieHeaderBoxParser.h"
#include "MovieFragmentHeaderBoxParser.h"
#include "TrackHeaderBoxParser.h"
#include "TrackFragmentHeaderBoxParser.h"
#include "TrackFragmentRunBoxParser.h"
#include "UserExtensionBoxParser.h"

#include <sstream>

namespace isobmf {

namespace {

enum : std::uint32_t
{
    // Container boxes
    BoxType_Movie               = makeBoxType("moov"),
    BoxType_Track               = makeBoxType("trak"),
    BoxType_MovieFragment       = makeBoxType("moof"),
    BoxType_TrackFragment       = makeBoxType("traf"),

    // Full boxes
    BoxType_FileType            = makeBoxType("ftyp"),
    BoxType_FreeSpace           = makeBoxType("free"),
    BoxType_MovieData           = makeBoxType("mdat"),
    BoxType_UserExtension       = makeBoxType("uuid"),

    BoxType_MovieHeader         = makeBoxType("mvhd"),
    BoxType_TrackHeader         = makeBoxType("trhd"),

    BoxType_MovieFragmentHeader = makeBoxType("mfhd"),
    BoxType_TrackFragmentHeader = makeBoxType("tfhd"),
    BoxType_TrackFragmentRun    = makeBoxType("trun"),
};

} // namespace

#define CASE_CONT_BOX(type, name) \
    case BoxType_##type: \
        return std::make_unique<ContainerBoxParser>(name, boxSize, parentBox, *this); \

#define CASE_FULL_BOX(name) \
    case BoxType_##name: \
        return std::make_unique<name##BoxParser>(boxSize, parentBox);

std::unique_ptr<BoxParser> BaseParserFactory::createParser(
        BoxType boxType,
        BoxSize boxSize,
        const BoxParser* parentBox) const
{
    switch (boxType) {
    // Container boxes
    CASE_CONT_BOX(Movie, "Movie")
    CASE_CONT_BOX(Track, "Track")
    CASE_CONT_BOX(MovieFragment, "Movie Fragment")
    CASE_CONT_BOX(TrackFragment, "Track Fragment")

    // Full boxes
    CASE_FULL_BOX(FileType)
    CASE_FULL_BOX(FreeSpace)
    CASE_FULL_BOX(MovieData)
    CASE_FULL_BOX(UserExtension)

    CASE_FULL_BOX(MovieHeader)
    CASE_FULL_BOX(TrackHeader)

    CASE_FULL_BOX(MovieFragmentHeader)
    CASE_FULL_BOX(TrackFragmentHeader)
    CASE_FULL_BOX(TrackFragmentRun)
    }

    return {};
}

std::unique_ptr<BoxParser> BaseParserFactory::createUnknownParser(
        BoxType boxType,
        BoxSize boxSize,
        const BoxParser* parentBox) const
{
    std::ostringstream os;
    os << "Unknown (" << makeBoxName(boxType) << ")";
    return std::make_unique<FullBoxParser>(os.str(), boxSize, parentBox);
}

} // namespace
