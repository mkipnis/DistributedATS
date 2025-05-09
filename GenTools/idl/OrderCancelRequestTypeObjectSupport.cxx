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
 * @file OrderCancelRequestTypeObjectSupport.cxx
 * Source file containing the implementation to register the TypeObject representation of the described types in the IDL file
 *
 * This file was generated by the tool fastddsgen (version: 4.1.0).
 */

#include "OrderCancelRequestTypeObjectSupport.hpp"

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

#include "OrderCancelRequest.hpp"

#include "Header.hpp"

using namespace eprosima::fastdds::dds::xtypes;

namespace DistributedATS_OrderCancelRequest {
// TypeIdentifier is returned by reference: dependent structures/unions are registered in this same method
void register_OrderCancelRequest_type_identifier(
        TypeIdentifierPair& type_ids_OrderCancelRequest)
{

    ReturnCode_t return_code_OrderCancelRequest {eprosima::fastdds::dds::RETCODE_OK};
    return_code_OrderCancelRequest =
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
        "DistributedATS_OrderCancelRequest::OrderCancelRequest", type_ids_OrderCancelRequest);
    if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrderCancelRequest)
    {
        StructTypeFlag struct_flags_OrderCancelRequest = TypeObjectUtils::build_struct_type_flag(eprosima::fastdds::dds::xtypes::ExtensibilityKind::APPENDABLE,
                false, false);
        QualifiedTypeName type_name_OrderCancelRequest = "DistributedATS_OrderCancelRequest::OrderCancelRequest";
        eprosima::fastcdr::optional<AppliedBuiltinTypeAnnotations> type_ann_builtin_OrderCancelRequest;
        eprosima::fastcdr::optional<AppliedAnnotationSeq> ann_custom_OrderCancelRequest;
        CompleteTypeDetail detail_OrderCancelRequest = TypeObjectUtils::build_complete_type_detail(type_ann_builtin_OrderCancelRequest, ann_custom_OrderCancelRequest, type_name_OrderCancelRequest.to_string());
        CompleteStructHeader header_OrderCancelRequest;
        header_OrderCancelRequest = TypeObjectUtils::build_complete_struct_header(TypeIdentifier(), detail_OrderCancelRequest);
        CompleteStructMemberSeq member_seq_OrderCancelRequest;
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_fix_header = TypeObjectUtils::build_complete_member_detail(name_fix_header, member_ann_builtin_fix_header, ann_custom_OrderCancelRequest);
            CompleteStructMember member_fix_header = TypeObjectUtils::build_complete_struct_member(common_fix_header, detail_fix_header);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_fix_header);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_DATS_Source = TypeObjectUtils::build_complete_member_detail(name_DATS_Source, member_ann_builtin_DATS_Source, ann_custom_OrderCancelRequest);
            CompleteStructMember member_DATS_Source = TypeObjectUtils::build_complete_struct_member(common_DATS_Source, detail_DATS_Source);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_DATS_Source);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_DATS_Destination = TypeObjectUtils::build_complete_member_detail(name_DATS_Destination, member_ann_builtin_DATS_Destination, ann_custom_OrderCancelRequest);
            CompleteStructMember member_DATS_Destination = TypeObjectUtils::build_complete_struct_member(common_DATS_Destination, detail_DATS_Destination);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_DATS_Destination);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_DATS_SourceUser = TypeObjectUtils::build_complete_member_detail(name_DATS_SourceUser, member_ann_builtin_DATS_SourceUser, ann_custom_OrderCancelRequest);
            CompleteStructMember member_DATS_SourceUser = TypeObjectUtils::build_complete_struct_member(common_DATS_SourceUser, detail_DATS_SourceUser);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_DATS_SourceUser);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_DATS_DestinationUser = TypeObjectUtils::build_complete_member_detail(name_DATS_DestinationUser, member_ann_builtin_DATS_DestinationUser, ann_custom_OrderCancelRequest);
            CompleteStructMember member_DATS_DestinationUser = TypeObjectUtils::build_complete_struct_member(common_DATS_DestinationUser, detail_DATS_DestinationUser);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_DATS_DestinationUser);
        }
        {
            TypeIdentifierPair type_ids_OrigClOrdID;
            ReturnCode_t return_code_OrigClOrdID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_OrigClOrdID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_OrigClOrdID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrigClOrdID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_OrigClOrdID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_OrigClOrdID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_OrigClOrdID = 0x00000005;
            bool common_OrigClOrdID_ec {false};
            CommonStructMember common_OrigClOrdID {TypeObjectUtils::build_common_struct_member(member_id_OrigClOrdID, member_flags_OrigClOrdID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_OrigClOrdID, common_OrigClOrdID_ec))};
            if (!common_OrigClOrdID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure OrigClOrdID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_OrigClOrdID = "OrigClOrdID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_OrigClOrdID;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_OrigClOrdID = TypeObjectUtils::build_complete_member_detail(name_OrigClOrdID, member_ann_builtin_OrigClOrdID, ann_custom_OrderCancelRequest);
            CompleteStructMember member_OrigClOrdID = TypeObjectUtils::build_complete_struct_member(common_OrigClOrdID, detail_OrigClOrdID);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_OrigClOrdID);
        }
        {
            TypeIdentifierPair type_ids_ClOrdID;
            ReturnCode_t return_code_ClOrdID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_ClOrdID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_ClOrdID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_ClOrdID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_ClOrdID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_ClOrdID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_ClOrdID = 0x00000006;
            bool common_ClOrdID_ec {false};
            CommonStructMember common_ClOrdID {TypeObjectUtils::build_common_struct_member(member_id_ClOrdID, member_flags_ClOrdID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_ClOrdID, common_ClOrdID_ec))};
            if (!common_ClOrdID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure ClOrdID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_ClOrdID = "ClOrdID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_ClOrdID;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_ClOrdID = TypeObjectUtils::build_complete_member_detail(name_ClOrdID, member_ann_builtin_ClOrdID, ann_custom_OrderCancelRequest);
            CompleteStructMember member_ClOrdID = TypeObjectUtils::build_complete_struct_member(common_ClOrdID, detail_ClOrdID);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_ClOrdID);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_Symbol = TypeObjectUtils::build_complete_member_detail(name_Symbol, member_ann_builtin_Symbol, ann_custom_OrderCancelRequest);
            CompleteStructMember member_Symbol = TypeObjectUtils::build_complete_struct_member(common_Symbol, detail_Symbol);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_Symbol);
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
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_SecurityExchange = TypeObjectUtils::build_complete_member_detail(name_SecurityExchange, member_ann_builtin_SecurityExchange, ann_custom_OrderCancelRequest);
            CompleteStructMember member_SecurityExchange = TypeObjectUtils::build_complete_struct_member(common_SecurityExchange, detail_SecurityExchange);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_SecurityExchange);
        }
        {
            TypeIdentifierPair type_ids_Side;
            ReturnCode_t return_code_Side {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Side =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_char", type_ids_Side);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Side)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "Side Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_Side = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Side = 0x00000009;
            bool common_Side_ec {false};
            CommonStructMember common_Side {TypeObjectUtils::build_common_struct_member(member_id_Side, member_flags_Side, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Side, common_Side_ec))};
            if (!common_Side_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Side member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Side = "Side";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Side;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_Side = TypeObjectUtils::build_complete_member_detail(name_Side, member_ann_builtin_Side, ann_custom_OrderCancelRequest);
            CompleteStructMember member_Side = TypeObjectUtils::build_complete_struct_member(common_Side, detail_Side);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_Side);
        }
        {
            TypeIdentifierPair type_ids_TransactTime;
            ReturnCode_t return_code_TransactTime {eprosima::fastdds::dds::RETCODE_OK};
            return_code_TransactTime =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_uint64_t", type_ids_TransactTime);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_TransactTime)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "TransactTime Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_TransactTime = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_TransactTime = 0x0000000a;
            bool common_TransactTime_ec {false};
            CommonStructMember common_TransactTime {TypeObjectUtils::build_common_struct_member(member_id_TransactTime, member_flags_TransactTime, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_TransactTime, common_TransactTime_ec))};
            if (!common_TransactTime_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure TransactTime member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_TransactTime = "TransactTime";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_TransactTime;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_TransactTime = TypeObjectUtils::build_complete_member_detail(name_TransactTime, member_ann_builtin_TransactTime, ann_custom_OrderCancelRequest);
            CompleteStructMember member_TransactTime = TypeObjectUtils::build_complete_struct_member(common_TransactTime, detail_TransactTime);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_TransactTime);
        }
        {
            TypeIdentifierPair type_ids_OrderQty;
            ReturnCode_t return_code_OrderQty {eprosima::fastdds::dds::RETCODE_OK};
            return_code_OrderQty =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_int32_t", type_ids_OrderQty);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrderQty)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "OrderQty Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_OrderQty = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_OrderQty = 0x0000000b;
            bool common_OrderQty_ec {false};
            CommonStructMember common_OrderQty {TypeObjectUtils::build_common_struct_member(member_id_OrderQty, member_flags_OrderQty, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_OrderQty, common_OrderQty_ec))};
            if (!common_OrderQty_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure OrderQty member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_OrderQty = "OrderQty";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_OrderQty;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_OrderQty = TypeObjectUtils::build_complete_member_detail(name_OrderQty, member_ann_builtin_OrderQty, ann_custom_OrderCancelRequest);
            CompleteStructMember member_OrderQty = TypeObjectUtils::build_complete_struct_member(common_OrderQty, detail_OrderQty);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_OrderQty);
        }
        {
            TypeIdentifierPair type_ids_Text;
            ReturnCode_t return_code_Text {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Text =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_Text);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Text)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_Text))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_Text = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Text = 0x0000000c;
            bool common_Text_ec {false};
            CommonStructMember common_Text {TypeObjectUtils::build_common_struct_member(member_id_Text, member_flags_Text, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Text, common_Text_ec))};
            if (!common_Text_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Text member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Text = "Text";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Text;
            ann_custom_OrderCancelRequest.reset();
            CompleteMemberDetail detail_Text = TypeObjectUtils::build_complete_member_detail(name_Text, member_ann_builtin_Text, ann_custom_OrderCancelRequest);
            CompleteStructMember member_Text = TypeObjectUtils::build_complete_struct_member(common_Text, detail_Text);
            TypeObjectUtils::add_complete_struct_member(member_seq_OrderCancelRequest, member_Text);
        }
        CompleteStructType struct_type_OrderCancelRequest = TypeObjectUtils::build_complete_struct_type(struct_flags_OrderCancelRequest, header_OrderCancelRequest, member_seq_OrderCancelRequest);
        if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                TypeObjectUtils::build_and_register_struct_type_object(struct_type_OrderCancelRequest, type_name_OrderCancelRequest.to_string(), type_ids_OrderCancelRequest))
        {
            EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                    "DistributedATS_OrderCancelRequest::OrderCancelRequest already registered in TypeObjectRegistry for a different type.");
        }
    }
}

} // namespace DistributedATS_OrderCancelRequest

