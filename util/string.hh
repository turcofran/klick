/*
 * Copyright (C) 2007-2008  Dominic Sacré  <dominic.sacre@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef _DAS_STRING_HH
#define _DAS_STRING_HH

#include <string>
#include <sstream>

namespace das {


class make_string
{
  public:
    template <typename T>
    make_string & operator<< (T const& t) {
        _stream << t;
        return *this;
    }

    make_string & operator<< (std::ostream & (*pf)(std::ostream &)) {
        pf(_stream);
        return *this;
    }

    operator std::string() {
        return _stream.str();
    }

  private:
    std::ostringstream _stream;
};


std::string indent(const std::string & s, int n);


} // namespace das

#endif // _DAS_STRING_HH