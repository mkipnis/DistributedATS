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
 * @file LogonCdrAux.ipp
 * This source file contains some declarations of CDR related functions.
 *
 * This file was generated by the tool fastddsgen (version: 4.1.0).
 */

#ifndef FAST_DDS_GENERATED__DISTRIBUTEDATS_LOGON_LOGONCDRAUX_IPP
#define FAST_DDS_GENERATED__DISTRIBUTEDATS_LOGON_LOGONCDRAUX_IPP

#include "LogonCdrAux.hpp"

#include <fastcdr/Cdr.h>
#include <fastcdr/CdrSizeCalculator.hpp>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

namespace eprosima {
namespace fastcdr {

template<>
eProsima_user_DllExport size_t calculate_serialized_size(
        eprosima::fastcdr::CdrSizeCalculator& calculator,
        const DistributedATS_Logon::Logon& data,
        size_t& current_alignment)
{
    using namespace DistributedATS_Logon;

    static_cast<void>(data);

    eprosima::fastcdr::EncodingAlgorithmFlag previous_encoding = calculator.get_encoding();
    size_t calculated_size {calculator.begin_calculate_type_serialized_size(
                                eprosima::fastcdr::CdrVersion::XCDRv2 == calculator.get_cdr_version() ?
                                eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
                                eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
                                current_alignment)};


        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(0),
                data.fix_header(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(1),
                data.DATS_Source(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(2),
                data.DATS_Destination(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(3),
                data.DATS_SourceUser(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(4),
                data.DATS_DestinationUser(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(5),
                data.EncryptMethod(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(6),
                data.HeartBtInt(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(7),
                data.RawData(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(8),
                data.Username(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(9),
                data.Password(), current_alignment);


    calculated_size += calculator.end_calculate_type_serialized_size(previous_encoding, current_alignment);

    return calculated_size;
}

template<>
eProsima_user_DllExport void serialize(
        eprosima::fastcdr::Cdr& scdr,
        const DistributedATS_Logon::Logon& data)
{
    using namespace DistributedATS_Logon;

    eprosima::fastcdr::Cdr::state current_state(scdr);
    scdr.begin_serialize_type(current_state,
            eprosima::fastcdr::CdrVersion::XCDRv2 == scdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR);

    scdr
        << eprosima::fastcdr::MemberId(0) << data.fix_header()
        << eprosima::fastcdr::MemberId(1) << data.DATS_Source()
        << eprosima::fastcdr::MemberId(2) << data.DATS_Destination()
        << eprosima::fastcdr::MemberId(3) << data.DATS_SourceUser()
        << eprosima::fastcdr::MemberId(4) << data.DATS_DestinationUser()
        << eprosima::fastcdr::MemberId(5) << data.EncryptMethod()
        << eprosima::fastcdr::MemberId(6) << data.HeartBtInt()
        << eprosima::fastcdr::MemberId(7) << data.RawData()
        << eprosima::fastcdr::MemberId(8) << data.Username()
        << eprosima::fastcdr::MemberId(9) << data.Password()
;
    scdr.end_serialize_type(current_state);
}

template<>
eProsima_user_DllExport void deserialize(
        eprosima::fastcdr::Cdr& cdr,
        DistributedATS_Logon::Logon& data)
{
    using namespace DistributedATS_Logon;

    cdr.deserialize_type(eprosima::fastcdr::CdrVersion::XCDRv2 == cdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
            [&data](eprosima::fastcdr::Cdr& dcdr, const eprosima::fastcdr::MemberId& mid) -> bool
            {
                bool ret_value = true;
                switch (mid.id)
                {
                                        case 0:
                                                dcdr >> data.fix_header();
                                            break;

                                        case 1:
                                                dcdr >> data.DATS_Source();
                                            break;

                                        case 2:
                                                dcdr >> data.DATS_Destination();
                                            break;

                                        case 3:
                                                dcdr >> data.DATS_SourceUser();
                                            break;

                                        case 4:
                                                dcdr >> data.DATS_DestinationUser();
                                            break;

                                        case 5:
                                                dcdr >> data.EncryptMethod();
                                            break;

                                        case 6:
                                                dcdr >> data.HeartBtInt();
                                            break;

                                        case 7:
                                                dcdr >> data.RawData();
                                            break;

                                        case 8:
                                                dcdr >> data.Username();
                                            break;

                                        case 9:
                                                dcdr >> data.Password();
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
        const DistributedATS_Logon::Logon& data)
{
    using namespace DistributedATS_Logon;
            extern void serialize_key(
                    Cdr& scdr,
                    const DistributedATS::Header& data);











    static_cast<void>(scdr);
    static_cast<void>(data);
                        serialize_key(scdr, data.fix_header());

                        scdr << data.DATS_Source();

                        scdr << data.DATS_Destination();

                        scdr << data.DATS_SourceUser();

                        scdr << data.DATS_DestinationUser();

                        scdr << data.EncryptMethod();

                        scdr << data.HeartBtInt();

                        scdr << data.RawData();

                        scdr << data.Username();

                        scdr << data.Password();

}



} // namespace fastcdr
} // namespace eprosima

#endif // FAST_DDS_GENERATED__DISTRIBUTEDATS_LOGON_LOGONCDRAUX_IPP

