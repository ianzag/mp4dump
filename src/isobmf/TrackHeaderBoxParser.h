
#pragma once

#include "FullBoxParser.h"

namespace isobmf {

/*
8.3.2 Track Header Box
8.3.2.1 Definition
Box Type:   ‘tkhd’
Container:  Track Box (‘trak’)
Mandatory:  Yes
Quantity:   Exactly one

8.3.2.2  Syntax
aligned(8) class TrackHeaderBox extends FullBox(‘tkhd’, version, flags) {
    if (version==1) {
        unsigned int(64) creation_time;
        unsigned int(64) modification_time;
        unsigned int(32) track_ID;
        const unsigned int(32) reserved = 0;
        unsigned int(64) duration;
    } else { // version==0
        unsigned int(32) creation_time;
        unsigned int(32) modification_time;
        unsigned int(32) track_ID;
        const unsigned int(32) reserved = 0;
        unsigned int(32) duration;
    }
    const unsigned int(32)[2] reserved = 0;
    template int(16) layer = 0;
    template int(16) alternate_group = 0;
    template int(16) volume = {if track_is_audio 0x0100 else 0};
    const unsigned int(16) reserved = 0;
    template int(32)[9] matrix=
    { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
    // unity matrix
    unsigned int(32) width;
    unsigned int(32) height;
}
*/
class TrackHeaderBoxParser final : public FullBoxParser
{
public:
    TrackHeaderBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Track Header", boxSize, parentBox) {}

    // TODO: Add box parser

private:
};

} // namespace
