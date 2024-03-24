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

constexpr const char* LOGON_TOPIC_NAME = "LOGON_TOPIC";
constexpr const char* LOGOUT_TOPIC_NAME = "LOGOUT_TOPIC";

constexpr const char* NEW_ORDER_SINGLE_TOPIC_NAME = "NEW_ORDER_SINGLE_TOPIC";
constexpr const char* EXECUTION_REPORT_TOPIC_NAME = "EXECUTION_REPORT_TOPIC";
constexpr const char* ORDER_CANCEL_REQUEST_TOPIC_NAME = "ORDER_CANCEL_REQUEST_TOPIC";
constexpr const char* ORDER_CANCEL_REJECT_TOPIC_NAME = "ORDER_CANCEL_REJECT_TOPIC";


constexpr const char* ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME = "ORDER_MASS_CANCEL_REQUEST_TOPIC";
constexpr const char* ORDER_MASS_CANCEL_REPORT_TOPIC_NAME = "ORDER_MASS_CANCEL_REPORT_TOPIC";

constexpr const char* MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME = "MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME";
constexpr const char* MARKET_DATA_REQUEST_TOPIC_NAME = "MARKET_DATA_REQUEST_TOPIC_NAME";
constexpr const char* MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME = "MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME";

constexpr const char* SECURITY_LIST_REQUEST_TOPIC_NAME = "SECURITY_LIST_REQUEST_TOPIC_NAME";
constexpr const char* SECURITY_LIST_TOPIC_NAME = "SECURITY_LIST_TOPIC";

constexpr const char* ORDER_MASS_STATUS_REQUEST_TOPIC_NAME = "ORDER_MASS_STATUS_REQUEST_TOPIC";

constexpr const char* ORDER_CANCEL_REPLACE_REQUEST_TOPIC_NAME = "ORDER_CANCEL_REPLACE_REQUEST_TOPIC";
