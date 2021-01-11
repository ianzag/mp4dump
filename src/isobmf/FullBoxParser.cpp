
#include "FullBoxParser.h"

#include <iostream>

namespace isobmf {

void FullBoxParser::endParse()
{
    printIndent(std::cout) << "Box type: '" << getName() << "' size: " << std::dec << getSize() << " ";
    printDetails(std::cout) << std::endl;
}

} // namespace
