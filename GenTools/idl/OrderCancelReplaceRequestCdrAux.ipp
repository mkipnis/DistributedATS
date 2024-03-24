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
 * @file OrderCancelReplaceRequestCdrAux.ipp
 * This source file contains some declarations of CDR related functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUESTCDRAUX_IPP_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUESTCDRAUX_IPP_

#include "OrderCancelReplaceRequestCdrAux.hpp"

#include <fastcdr/Cdr.h>
#include <fastcdr/CdrSizeCalculator.hpp>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

namespace eprosima {
namespace fastcdr {



template<>
eProsima_user_DllExport size_t calculate_serialized_size(
        eprosima::fastcdr::CdrSizeCalculator& calculator,
        const DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& data,
        size_t& current_alignment)
{
    using namespace DistributedATS_OrderCancelReplaceRequest;

    static_cast<void>(data);

    eprosima::fastcdr::EncodingAlgorithmFlag previous_encoding = calculator.get_encoding();
    size_t calculated_size {calculator.begin_calculate_type_serialized_size(
                                eprosima::fastcdr::CdrVersion::XCDRv2 == calculator.get_cdr_version() ?
                                eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
                                eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
                                current_alignment)};


        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(0),
                data.header(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(1),
                data.OrigClOrdID(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(2),
                data.ClOrdID(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(3),
                data.ExecInst(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(4),
                data.Symbol(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(5),
                data.SecurityExchange(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(6),
                data.Side(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(7),
                data.TransactTime(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(8),
                data.OrderQty(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(9),
                data.OrdType(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(10),
                data.Price(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(11),
                data.StopPx(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(12),
                data.TimeInForce(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(13),
                data.Text(), current_alignment);


    calculated_size += calculator.end_calculate_type_serialized_size(previous_encoding, current_alignment);

    return calculated_size;
}

template<>
eProsima_user_DllExport void serialize(
        eprosima::fastcdr::Cdr& scdr,
        const DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& data)
{
    using namespace DistributedATS_OrderCancelReplaceRequest;

    eprosima::fastcdr::Cdr::state current_state(scdr);
    scdr.begin_serialize_type(current_state,
            eprosima::fastcdr::CdrVersion::XCDRv2 == scdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR);

    scdr
        << eprosima::fastcdr::MemberId(0) << data.header()
        << eprosima::fastcdr::MemberId(1) << data.OrigClOrdID()
        << eprosima::fastcdr::MemberId(2) << data.ClOrdID()
        << eprosima::fastcdr::MemberId(3) << data.ExecInst()
        << eprosima::fastcdr::MemberId(4) << data.Symbol()
        << eprosima::fastcdr::MemberId(5) << data.SecurityExchange()
        << eprosima::fastcdr::MemberId(6) << data.Side()
        << eprosima::fastcdr::MemberId(7) << data.TransactTime()
        << eprosima::fastcdr::MemberId(8) << data.OrderQty()
        << eprosima::fastcdr::MemberId(9) << data.OrdType()
        << eprosima::fastcdr::MemberId(10) << data.Price()
        << eprosima::fastcdr::MemberId(11) << data.StopPx()
        << eprosima::fastcdr::MemberId(12) << data.TimeInForce()
        << eprosima::fastcdr::MemberId(13) << data.Text()
;
    scdr.end_serialize_type(current_state);
}

template<>
eProsima_user_DllExport void deserialize(
        eprosima::fastcdr::Cdr& cdr,
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& data)
{
    using namespace DistributedATS_OrderCancelReplaceRequest;

    cdr.deserialize_type(eprosima::fastcdr::CdrVersion::XCDRv2 == cdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
            [&data](eprosima::fastcdr::Cdr& dcdr, const eprosima::fastcdr::MemberId& mid) -> bool
            {
                bool ret_value = true;
                switch (mid.id)
                {
                                        case 0:
                                                dcdr >> data.header();
                                            break;

                                        case 1:
                                                dcdr >> data.OrigClOrdID();
                                            break;

                                        case 2:
                                                dcdr >> data.ClOrdID();
                                            break;

                                        case 3:
                                                dcdr >> data.ExecInst();
                                            break;

                                        case 4:
                                                dcdr >> data.Symbol();
                                            break;

                                        case 5:
                                                dcdr >> data.SecurityExchange();
                                            break;

                                        case 6:
                                                dcdr >> data.Side();
                                            break;

                                        case 7:
                                                dcdr >> data.TransactTime();
                                            break;

                                        case 8:
                                                dcdr >> data.OrderQty();
                                            break;

                                        case 9:
                                                dcdr >> data.OrdType();
                                            break;

                                        case 10:
                                                dcdr >> data.Price();
                                            break;

                                        case 11:
                                                dcdr >> data.StopPx();
                                            break;

                                        case 12:
                                                dcdr >> data.TimeInForce();
                                            break;

                                        case 13:
                                                dcdr >> data.Text();
                                            break;

                    default:
                        ret_value = false;
                        break;
                }
                return ret_value;
            });
}

void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& data)
{
    using namespace DistributedATS_OrderCancelReplaceRequest;

    static_cast<void>(scdr);
    static_cast<void>(data);
}




} // namespace fastcdr
} // namespace eprosima

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUESTCDRAUX_IPP_
