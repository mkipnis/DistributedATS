// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file OrderMassStatusRequestTypeObject.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSSTATUSREQUEST_ORDERMASSSTATUSREQUEST_TYPE_OBJECT_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSSTATUSREQUEST_ORDERMASSSTATUSREQUEST_TYPE_OBJECT_H_

#include "HeaderTypeObject.h"

#include <fastrtps/types/TypeObject.h>
#include <map>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif
#else
#define eProsima_user_DllExport
#endif

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(OrderMassStatusRequest_SOURCE)
#define OrderMassStatusRequest_DllAPI __declspec( dllexport )
#else
#define OrderMassStatusRequest_DllAPI __declspec( dllimport )
#endif // OrderMassStatusRequest_SOURCE
#else
#define OrderMassStatusRequest_DllAPI
#endif
#else
#define OrderMassStatusRequest_DllAPI
#endif // _WIN32

using namespace eprosima::fastrtps::types;

eProsima_user_DllExport void registerOrderMassStatusRequestTypes();

namespace DistributedATS_OrderMassStatusRequest {


    eProsima_user_DllExport const TypeIdentifier* GetOrderMassStatusRequestIdentifier(bool complete = false);
    eProsima_user_DllExport const TypeObject* GetOrderMassStatusRequestObject(bool complete = false);
    eProsima_user_DllExport const TypeObject* GetMinimalOrderMassStatusRequestObject();
    eProsima_user_DllExport const TypeObject* GetCompleteOrderMassStatusRequestObject();

} // namespace DistributedATS_OrderMassStatusRequest

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSSTATUSREQUEST_ORDERMASSSTATUSREQUEST_TYPE_OBJECT_H_