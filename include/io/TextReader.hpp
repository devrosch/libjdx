#ifndef IO_TEXTREADER_HPP
#define IO_TEXTREADER_HPP

#ifdef __EMSCRIPTEN__
#include "io/BufferedTextReader.hpp"
#else
#include "io/SimpleTextReader.hpp"
#endif

namespace sciformats::io
{
// use BufferedTextReader for EMSCRIPTEN build as it drastically improves data
// reading performance in browser context
#ifdef __EMSCRIPTEN__
typedef BufferedTextReader TextReader;
#else
typedef SimpleTextReader TextReader;
#endif
}

#endif
