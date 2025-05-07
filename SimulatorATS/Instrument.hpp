/*
   Copyright (C) 2021 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef Instrument_h
#define Instrument_h

#include <ostream>

namespace SimulatorATS
{
    struct Instrument
    {
        Instrument( const char* securityExchangeIn, const char* symbolIn )
        : securityExchange( securityExchangeIn  ),  symbol( symbolIn )
        {
        };
    
        Instrument( std::string& securityExchangeIn, std::string& symbolIn )
        : securityExchange( securityExchangeIn  ),  symbol( symbolIn )
        {
        };
        
    
        std::string securityExchange;
        std::string symbol;
        
        friend std::ostream& operator<<( std::ostream& stream, const Instrument& instrument )
        {
            stream << instrument.securityExchange << ":" << instrument.symbol ;
            return stream;
        }
    
        friend bool operator<( const Instrument& i1, const Instrument& i2 )
        {
            return ( i1.symbol + i1.securityExchange < i2.symbol + i2.securityExchange );
        };
    
        friend bool operator==( const Instrument& i1, const Instrument& i2 )
        {
            return ( i1.symbol == i2.symbol && i1.securityExchange ==  i2.securityExchange );
        };
    };
};

#endif /* Instrument_h */
