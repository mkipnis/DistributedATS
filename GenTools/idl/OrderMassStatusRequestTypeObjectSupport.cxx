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
 * @file OrderMassStatusRequestTypeObjectSupport.cxx
 * Source file containing the implementation to register the TypeObject representation of the described types in the IDL file
 *
 * This file was generated by the tool fastddsgen (version: 4.1.0).
 */

#include "OrderMassStatusRequestTypeObjectSupport.hpp"

#include <mutex>
#include <string>

#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/log/Log.hpp>
#include <fastdds/dds/xtypes/common.hpp>
#include <fastdds/dds/xtypes/type_representation/ITypeObjectRegistry.hpp>
#include <fastdds/dds/xtypes/type_representation/TypeObject.hpp>
#include <fastdds/dds/xtypes/type_representation/TypeObjectUtils.hpp>

#include "OrderMassStatusRequest.hpp"

#include "Header.hpp"

using namespace eprosima::fastdds::dds::xtypes;

namespace DistributedATS_OrderMassStatusRequest {
// TypeIdentifier is returned by reference: dependent structures/unions are registered in this same method
void register_OrderMassStatusRequest_type_identifier(
        TypeIdentifierPair& type_ids_OrderMassStatusRequest)
{

    ReturnCode_t return_code_OrderMassStatusRequest {eprosima::fastdds::dds::RETCODE_OK};
    return_code_OrderMassStatusRequest =
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
        "DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest", type_ids_OrderMassStatusRequest);
    if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrderMassStatusRequest)
    {
        StructTypeFlag struct_flags_OrderMassStatusRequest = TypeObjectUtils::build_struct_type_flag(eprosima::fastdds::dds::xtypes::ExtensibilityKind::APPENDABLE,
                false, false);
        QualifiedTypeName type_name_OrderMassStatusRequest = "DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest";
        eprosima::fastcdr::optional<AppliedBuiltinTypeAnnotations> type_ann_builtin_OrderMassStatusRequest;
        eprosima::fastcdr::optional<AppliedAnnotationSeq> ann_custom_OrderMassStatusRequest;
        CompleteTypeDetail detail_OrderMassStatusRequest = TypeObjectUtils::build_complete_type_detail(type_ann_builtin_OrderMassStatusRequest, ann_custom_OrderMassStatusRequest, type_name_OrderMassStatusRequest.to_string());
        CompleteStructHeader header_OrderMassStatusRequest;
        header_OrderMassStatusRequest = TypeObjectUtils::build_complete_struct_header(TypeIdentifier(), detail_OrderMassStatusRequest);
        CompleteStructMemberSeq member_seq_OrderMassStatusRequest;
        {
            TypeIdentifierPair type_ids_fix_header;
            ReturnCode_t return_code_fix_header {eprosima::fastdds::dds::RETCODE_OK};
            return_code_fix_header =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "DistributedATS::Header", type_ids_fix_header);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_fix_header)
            {
                DistributedATS::register_Header_type_identifier(type_ids_fix_header);
            }
            StructMemberFlag member_flags_fix_header = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_fix_header = 0x00000000;
            bool common_fix_header_ec {false};
            CommonStructMember common_fix_header {TypeObjectUtils::build_common_struct_member(member_id_fix_header, member_flags_fix_header, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_fix_header, common_fix_header_ec))};
            if (!common_fix_header_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure fix_header member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_fix_header = "fix_header";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_fix_header;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_fix_header = TypeObjectUtils::build_complete_member_detail(name_fix_header, member_ann_builtin_fix_header, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_fix_header = TypeObjectUtils::build_complete_struct_member(common_fix_header, detail_fix_header);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_fix_header);
        }
        {
            TypeIdentifierPair type_ids_DATS_Source;
            ReturnCode_t return_code_DATS_Source {eprosima::fastdds::dds::RETCODE_OK};
            return_code_DATS_Source =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_DATS_Source);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_DATS_Source)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_DATS_Source))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_DATS_Source = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_DATS_Source = 0x00000001;
            bool common_DATS_Source_ec {false};
            CommonStructMember common_DATS_Source {TypeObjectUtils::build_common_struct_member(member_id_DATS_Source, member_flags_DATS_Source, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_DATS_Source, common_DATS_Source_ec))};
            if (!common_DATS_Source_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure DATS_Source member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_DATS_Source = "DATS_Source";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_DATS_Source;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_DATS_Source = TypeObjectUtils::build_complete_member_detail(name_DATS_Source, member_ann_builtin_DATS_Source, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_DATS_Source = TypeObjectUtils::build_complete_struct_member(common_DATS_Source, detail_DATS_Source);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_DATS_Source);
        }
        {
            TypeIdentifierPair type_ids_DATS_Destination;
            ReturnCode_t return_code_DATS_Destination {eprosima::fastdds::dds::RETCODE_OK};
            return_code_DATS_Destination =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_DATS_Destination);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_DATS_Destination)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_DATS_Destination))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_DATS_Destination = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_DATS_Destination = 0x00000002;
            bool common_DATS_Destination_ec {false};
            CommonStructMember common_DATS_Destination {TypeObjectUtils::build_common_struct_member(member_id_DATS_Destination, member_flags_DATS_Destination, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_DATS_Destination, common_DATS_Destination_ec))};
            if (!common_DATS_Destination_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure DATS_Destination member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_DATS_Destination = "DATS_Destination";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_DATS_Destination;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_DATS_Destination = TypeObjectUtils::build_complete_member_detail(name_DATS_Destination, member_ann_builtin_DATS_Destination, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_DATS_Destination = TypeObjectUtils::build_complete_struct_member(common_DATS_Destination, detail_DATS_Destination);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_DATS_Destination);
        }
        {
            TypeIdentifierPair type_ids_DATS_SourceUser;
            ReturnCode_t return_code_DATS_SourceUser {eprosima::fastdds::dds::RETCODE_OK};
            return_code_DATS_SourceUser =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_DATS_SourceUser);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_DATS_SourceUser)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_DATS_SourceUser))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_DATS_SourceUser = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_DATS_SourceUser = 0x00000003;
            bool common_DATS_SourceUser_ec {false};
            CommonStructMember common_DATS_SourceUser {TypeObjectUtils::build_common_struct_member(member_id_DATS_SourceUser, member_flags_DATS_SourceUser, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_DATS_SourceUser, common_DATS_SourceUser_ec))};
            if (!common_DATS_SourceUser_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure DATS_SourceUser member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_DATS_SourceUser = "DATS_SourceUser";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_DATS_SourceUser;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_DATS_SourceUser = TypeObjectUtils::build_complete_member_detail(name_DATS_SourceUser, member_ann_builtin_DATS_SourceUser, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_DATS_SourceUser = TypeObjectUtils::build_complete_struct_member(common_DATS_SourceUser, detail_DATS_SourceUser);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_DATS_SourceUser);
        }
        {
            TypeIdentifierPair type_ids_DATS_DestinationUser;
            ReturnCode_t return_code_DATS_DestinationUser {eprosima::fastdds::dds::RETCODE_OK};
            return_code_DATS_DestinationUser =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_DATS_DestinationUser);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_DATS_DestinationUser)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_DATS_DestinationUser))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_DATS_DestinationUser = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_DATS_DestinationUser = 0x00000004;
            bool common_DATS_DestinationUser_ec {false};
            CommonStructMember common_DATS_DestinationUser {TypeObjectUtils::build_common_struct_member(member_id_DATS_DestinationUser, member_flags_DATS_DestinationUser, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_DATS_DestinationUser, common_DATS_DestinationUser_ec))};
            if (!common_DATS_DestinationUser_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure DATS_DestinationUser member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_DATS_DestinationUser = "DATS_DestinationUser";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_DATS_DestinationUser;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_DATS_DestinationUser = TypeObjectUtils::build_complete_member_detail(name_DATS_DestinationUser, member_ann_builtin_DATS_DestinationUser, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_DATS_DestinationUser = TypeObjectUtils::build_complete_struct_member(common_DATS_DestinationUser, detail_DATS_DestinationUser);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_DATS_DestinationUser);
        }
        {
            TypeIdentifierPair type_ids_MassStatusReqID;
            ReturnCode_t return_code_MassStatusReqID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_MassStatusReqID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_MassStatusReqID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_MassStatusReqID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_MassStatusReqID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_MassStatusReqID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_MassStatusReqID = 0x00000005;
            bool common_MassStatusReqID_ec {false};
            CommonStructMember common_MassStatusReqID {TypeObjectUtils::build_common_struct_member(member_id_MassStatusReqID, member_flags_MassStatusReqID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_MassStatusReqID, common_MassStatusReqID_ec))};
            if (!common_MassStatusReqID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure MassStatusReqID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_MassStatusReqID = "MassStatusReqID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_MassStatusReqID;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_MassStatusReqID = TypeObjectUtils::build_complete_member_detail(name_MassStatusReqID, member_ann_builtin_MassStatusReqID, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_MassStatusReqID = TypeObjectUtils::build_complete_struct_member(common_MassStatusReqID, detail_MassStatusReqID);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_MassStatusReqID);
        }
        {
            TypeIdentifierPair type_ids_MassStatusReqType;
            ReturnCode_t return_code_MassStatusReqType {eprosima::fastdds::dds::RETCODE_OK};
            return_code_MassStatusReqType =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_int32_t", type_ids_MassStatusReqType);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_MassStatusReqType)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "MassStatusReqType Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_MassStatusReqType = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_MassStatusReqType = 0x00000006;
            bool common_MassStatusReqType_ec {false};
            CommonStructMember common_MassStatusReqType {TypeObjectUtils::build_common_struct_member(member_id_MassStatusReqType, member_flags_MassStatusReqType, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_MassStatusReqType, common_MassStatusReqType_ec))};
            if (!common_MassStatusReqType_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure MassStatusReqType member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_MassStatusReqType = "MassStatusReqType";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_MassStatusReqType;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_MassStatusReqType = TypeObjectUtils::build_complete_member_detail(name_MassStatusReqType, member_ann_builtin_MassStatusReqType, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_MassStatusReqType = TypeObjectUtils::build_complete_struct_member(common_MassStatusReqType, detail_MassStatusReqType);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_MassStatusReqType);
        }
        {
            TypeIdentifierPair type_ids_Symbol;
            ReturnCode_t return_code_Symbol {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Symbol =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_Symbol);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Symbol)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_Symbol))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_Symbol = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Symbol = 0x00000007;
            bool common_Symbol_ec {false};
            CommonStructMember common_Symbol {TypeObjectUtils::build_common_struct_member(member_id_Symbol, member_flags_Symbol, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Symbol, common_Symbol_ec))};
            if (!common_Symbol_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Symbol member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Symbol = "Symbol";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Symbol;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_Symbol = TypeObjectUtils::build_complete_member_detail(name_Symbol, member_ann_builtin_Symbol, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_Symbol = TypeObjectUtils::build_complete_struct_member(common_Symbol, detail_Symbol);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_Symbol);
        }
        {
            TypeIdentifierPair type_ids_SecurityExchange;
            ReturnCode_t return_code_SecurityExchange {eprosima::fastdds::dds::RETCODE_OK};
            return_code_SecurityExchange =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_SecurityExchange);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_SecurityExchange)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_SecurityExchange))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_SecurityExchange = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_SecurityExchange = 0x00000008;
            bool common_SecurityExchange_ec {false};
            CommonStructMember common_SecurityExchange {TypeObjectUtils::build_common_struct_member(member_id_SecurityExchange, member_flags_SecurityExchange, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_SecurityExchange, common_SecurityExchange_ec))};
            if (!common_SecurityExchange_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure SecurityExchange member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_SecurityExchange = "SecurityExchange";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_SecurityExchange;
            ann_custom_OrderMassStatusRequest.reset();
            CompleteMemberDetail detail_SecurityExchange = TypeObjectUtils::build_complete_member_detail(name_SecurityExchange, member_ann_builtin_SecurityExchange, ann_custom_OrderMassStatusRequest);
            CompleteStructMember member_SecurityExchange = TypeObjectUtils::build_complete_struct_member(common_SecurityExchange, detail_SecurityExchange);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderMassStatusRequest, member_SecurityExchange);
        }
        CompleteStructType struct_type_OrderMassStatusRequest = TypeObjectUtils::build_complete_struct_type(struct_flags_OrderMassStatusRequest, header_OrderMassStatusRequest, member_seq_OrderMassStatusRequest);
        if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                TypeObjectUtils::build_and_register_struct_type_object(struct_type_OrderMassStatusRequest, type_name_OrderMassStatusRequest.to_string(), type_ids_OrderMassStatusRequest))
        {
            EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                    "DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest already registered in TypeObjectRegistry for a different type.");
        }
    }
}

} // namespace DistributedATS_OrderMassStatusRequest

