#ifndef JDX_TEXTREADER_HPP
#define JDX_TEXTREADER_HPP

#ifdef  __EMSCRIPTEN__
#include "jdx/BufferedTextReader.hpp"
#else
#include "jdx/SimpleTextReader.hpp"
#endif

namespace sciformats::jdx
{
// use BufferedTextReader for EMSCRIPTEN build as it drastically improves data reading performance in browser context
#ifdef  __EMSCRIPTEN__
typedef BufferedTextReader TextReader;
#else
typedef SimpleTextReader TextReader;
#endif
}

#endif
