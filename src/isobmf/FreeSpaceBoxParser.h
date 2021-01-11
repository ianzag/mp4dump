
#pragma once

#include "FullBoxParser.h"

namespace isobmf {

/*
8.1.2 Free Space Box
8.1.2.1 Definition
Box Types:  ‘free’, ‘skip’
Container:  File or other box
Mandatory:  No
Quantity:   Zero or more

8.1.2.2  Syntax
aligned(8) class FreeSpaceBox extends Box(free_type) {
    unsigned int(8) data[];
}
*/
class FreeSpaceBoxParser final : public FullBoxParser
{
public:
    FreeSpaceBoxParser(BoxSize boxSize, const BoxParser* parentBox)
        : FullBoxParser("Free Space", boxSize, parentBox) {}
};

} // namespace
