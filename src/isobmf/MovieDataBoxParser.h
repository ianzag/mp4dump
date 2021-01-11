
#pragma once

#include "FullBoxParser.h"

namespace isobmf {

/*
8.1.1 Media Data Box
8.1.1.1 Definition
Box Type:   ‘mdat’
Container:  File
Mandatory:  No
Quantity:   Zero or more

8.1.1.2 Syntax
aligned(8) class MediaDataBox extends Box(‘mdat’) {
    bit(8) data[];
}
*/
class MovieDataBoxParser final : public FullBoxParser
{
public:
    MovieDataBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Movie Data", boxSize, parentBox) {}

private:
};

} // namespace
