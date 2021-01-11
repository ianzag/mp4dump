
#pragma once

#include "FullBoxParser.h"

namespace isobmf {

/*
8.2.2  Movie Header Box
8.2.2.1 Definition
Box Type:   ‘mvhd’
Container:  Movie Box (‘moov’)
Mandatory:  Yes
Quantity:   Exactly one

8.2.2.2 Syntax
aligned(8) class MovieHeaderBox extends FullBox(‘mvhd’, version, 0) {
    if (version==1) {
        unsigned int(64) creation_time;
        unsigned int(64) modification_time;
        unsigned int(32) timescale;
        unsigned int(64) duration;
    } else { // version==0
        unsigned int(32) creation_time;
        unsigned int(32) modification_time;
        unsigned int(32) timescale;
        unsigned int(32) duration;
    }
    template int(32) rate = 0x00010000; // typically 1.0
    template int(16) volume = 0x0100;
    // typically, full volume
    const bit(16) reserved = 0;
    const unsigned int(32)[2] reserved = 0;
    template int(32)[9] matrix =
    { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
    // Unity matrix
    bit(32)[6] pre_defined = 0;
    unsigned int(32) next_track_ID;
}
*/
class MovieHeaderBoxParser final : public FullBoxParser
{
public:
    MovieHeaderBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Movie Header", boxSize, parentBox) {}

    // TODO: Add box parser

private:
};

} // namespace
