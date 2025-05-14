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

#pragma once

#include <quickfix/Message.h>

namespace DistributedATS
{

  inline void convert_dds_string_to_fix( const std::string& string_in, const int FIX_TAG, FIX::FieldMap& fix_field_map_out )
  {
	if ( string_in.length() > 0 )
	{
		FIX::StringField fixField(FIX_TAG, string_in);
		fix_field_map_out.setField( fixField );
	};	
  };

  inline void convert_dds_timestamp_to_fix( unsigned long long timestamp_in, const int FIX_TAG, FIX::FieldMap& fix_field_map_out )
  {
	if ( timestamp_in > 0 )
	{
		FIX::UtcTimeStamp utcTimeStamp( (time_t)(timestamp_in/1000000) );
        	utcTimeStamp.setMicrosecond( timestamp_in-((timestamp_in/1000000)*1000000) );
		FIX::UtcTimeStampField timestampField( FIX_TAG, utcTimeStamp, 6 );
		fix_field_map_out.setField( timestampField );
	};	
  };

}
