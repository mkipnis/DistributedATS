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
 * @file HeaderTypeObjectSupport.cxx
 * Source file containing the implementation to register the TypeObject representation of the described types in the IDL file
 *
 * This file was generated by the tool fastddsgen (version: 4.1.0).
 */

#include "HeaderTypeObjectSupport.hpp"

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

#include "Header.hpp"


using namespace eprosima::fastdds::dds::xtypes;

namespace DistributedATS {
// TypeIdentifier is returned by reference: dependent structures/unions are registered in this same method
void register_Header_type_identifier(
        TypeIdentifierPair& type_ids_Header)
{

    ReturnCode_t return_code_Header {eprosima::fastdds::dds::RETCODE_OK};
    return_code_Header =
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
        "DistributedATS::Header", type_ids_Header);
    if (eprosima::fastdds::dds::RETCODE_OK != return_code_Header)
    {
        StructTypeFlag struct_flags_Header = TypeObjectUtils::build_struct_type_flag(eprosima::fastdds::dds::xtypes::ExtensibilityKind::APPENDABLE,
                false, false);
        QualifiedTypeName type_name_Header = "DistributedATS::Header";
        eprosima::fastcdr::optional<AppliedBuiltinTypeAnnotations> type_ann_builtin_Header;
        eprosima::fastcdr::optional<AppliedAnnotationSeq> ann_custom_Header;
        CompleteTypeDetail detail_Header = TypeObjectUtils::build_complete_type_detail(type_ann_builtin_Header, ann_custom_Header, type_name_Header.to_string());
        CompleteStructHeader header_Header;
        header_Header = TypeObjectUtils::build_complete_struct_header(TypeIdentifier(), detail_Header);
        CompleteStructMemberSeq member_seq_Header;
        {
            TypeIdentifierPair type_ids_BeginString;
            ReturnCode_t return_code_BeginString {eprosima::fastdds::dds::RETCODE_OK};
            return_code_BeginString =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_BeginString);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_BeginString)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_BeginString))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_BeginString = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_BeginString = 0x00000000;
            bool common_BeginString_ec {false};
            CommonStructMember common_BeginString {TypeObjectUtils::build_common_struct_member(member_id_BeginString, member_flags_BeginString, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_BeginString, common_BeginString_ec))};
            if (!common_BeginString_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure BeginString member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_BeginString = "BeginString";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_BeginString;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_BeginString = TypeObjectUtils::build_complete_member_detail(name_BeginString, member_ann_builtin_BeginString, ann_custom_Header);
            CompleteStructMember member_BeginString = TypeObjectUtils::build_complete_struct_member(common_BeginString, detail_BeginString);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_BeginString);
        }
        {
            TypeIdentifierPair type_ids_BodyLength;
            ReturnCode_t return_code_BodyLength {eprosima::fastdds::dds::RETCODE_OK};
            return_code_BodyLength =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_int32_t", type_ids_BodyLength);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_BodyLength)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "BodyLength Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_BodyLength = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_BodyLength = 0x00000001;
            bool common_BodyLength_ec {false};
            CommonStructMember common_BodyLength {TypeObjectUtils::build_common_struct_member(member_id_BodyLength, member_flags_BodyLength, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_BodyLength, common_BodyLength_ec))};
            if (!common_BodyLength_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure BodyLength member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_BodyLength = "BodyLength";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_BodyLength;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_BodyLength = TypeObjectUtils::build_complete_member_detail(name_BodyLength, member_ann_builtin_BodyLength, ann_custom_Header);
            CompleteStructMember member_BodyLength = TypeObjectUtils::build_complete_struct_member(common_BodyLength, detail_BodyLength);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_BodyLength);
        }
        {
            TypeIdentifierPair type_ids_MsgType;
            ReturnCode_t return_code_MsgType {eprosima::fastdds::dds::RETCODE_OK};
            return_code_MsgType =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_MsgType);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_MsgType)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_MsgType))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_MsgType = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_MsgType = 0x00000002;
            bool common_MsgType_ec {false};
            CommonStructMember common_MsgType {TypeObjectUtils::build_common_struct_member(member_id_MsgType, member_flags_MsgType, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_MsgType, common_MsgType_ec))};
            if (!common_MsgType_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure MsgType member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_MsgType = "MsgType";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_MsgType;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_MsgType = TypeObjectUtils::build_complete_member_detail(name_MsgType, member_ann_builtin_MsgType, ann_custom_Header);
            CompleteStructMember member_MsgType = TypeObjectUtils::build_complete_struct_member(common_MsgType, detail_MsgType);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_MsgType);
        }
        {
            TypeIdentifierPair type_ids_SenderCompID;
            ReturnCode_t return_code_SenderCompID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_SenderCompID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_SenderCompID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_SenderCompID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_SenderCompID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_SenderCompID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_SenderCompID = 0x00000003;
            bool common_SenderCompID_ec {false};
            CommonStructMember common_SenderCompID {TypeObjectUtils::build_common_struct_member(member_id_SenderCompID, member_flags_SenderCompID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_SenderCompID, common_SenderCompID_ec))};
            if (!common_SenderCompID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure SenderCompID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_SenderCompID = "SenderCompID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_SenderCompID;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_SenderCompID = TypeObjectUtils::build_complete_member_detail(name_SenderCompID, member_ann_builtin_SenderCompID, ann_custom_Header);
            CompleteStructMember member_SenderCompID = TypeObjectUtils::build_complete_struct_member(common_SenderCompID, detail_SenderCompID);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_SenderCompID);
        }
        {
            TypeIdentifierPair type_ids_TargetCompID;
            ReturnCode_t return_code_TargetCompID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_TargetCompID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_TargetCompID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_TargetCompID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_TargetCompID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_TargetCompID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_TargetCompID = 0x00000004;
            bool common_TargetCompID_ec {false};
            CommonStructMember common_TargetCompID {TypeObjectUtils::build_common_struct_member(member_id_TargetCompID, member_flags_TargetCompID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_TargetCompID, common_TargetCompID_ec))};
            if (!common_TargetCompID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure TargetCompID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_TargetCompID = "TargetCompID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_TargetCompID;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_TargetCompID = TypeObjectUtils::build_complete_member_detail(name_TargetCompID, member_ann_builtin_TargetCompID, ann_custom_Header);
            CompleteStructMember member_TargetCompID = TypeObjectUtils::build_complete_struct_member(common_TargetCompID, detail_TargetCompID);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_TargetCompID);
        }
        {
            TypeIdentifierPair type_ids_MsgSeqNum;
            ReturnCode_t return_code_MsgSeqNum {eprosima::fastdds::dds::RETCODE_OK};
            return_code_MsgSeqNum =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_int32_t", type_ids_MsgSeqNum);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_MsgSeqNum)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "MsgSeqNum Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_MsgSeqNum = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_MsgSeqNum = 0x00000005;
            bool common_MsgSeqNum_ec {false};
            CommonStructMember common_MsgSeqNum {TypeObjectUtils::build_common_struct_member(member_id_MsgSeqNum, member_flags_MsgSeqNum, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_MsgSeqNum, common_MsgSeqNum_ec))};
            if (!common_MsgSeqNum_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure MsgSeqNum member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_MsgSeqNum = "MsgSeqNum";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_MsgSeqNum;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_MsgSeqNum = TypeObjectUtils::build_complete_member_detail(name_MsgSeqNum, member_ann_builtin_MsgSeqNum, ann_custom_Header);
            CompleteStructMember member_MsgSeqNum = TypeObjectUtils::build_complete_struct_member(common_MsgSeqNum, detail_MsgSeqNum);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_MsgSeqNum);
        }
        {
            TypeIdentifierPair type_ids_SenderSubID;
            ReturnCode_t return_code_SenderSubID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_SenderSubID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_SenderSubID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_SenderSubID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_SenderSubID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_SenderSubID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_SenderSubID = 0x00000006;
            bool common_SenderSubID_ec {false};
            CommonStructMember common_SenderSubID {TypeObjectUtils::build_common_struct_member(member_id_SenderSubID, member_flags_SenderSubID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_SenderSubID, common_SenderSubID_ec))};
            if (!common_SenderSubID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure SenderSubID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_SenderSubID = "SenderSubID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_SenderSubID;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_SenderSubID = TypeObjectUtils::build_complete_member_detail(name_SenderSubID, member_ann_builtin_SenderSubID, ann_custom_Header);
            CompleteStructMember member_SenderSubID = TypeObjectUtils::build_complete_struct_member(common_SenderSubID, detail_SenderSubID);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_SenderSubID);
        }
        {
            TypeIdentifierPair type_ids_TargetSubID;
            ReturnCode_t return_code_TargetSubID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_TargetSubID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_TargetSubID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_TargetSubID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_TargetSubID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_TargetSubID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_TargetSubID = 0x00000007;
            bool common_TargetSubID_ec {false};
            CommonStructMember common_TargetSubID {TypeObjectUtils::build_common_struct_member(member_id_TargetSubID, member_flags_TargetSubID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_TargetSubID, common_TargetSubID_ec))};
            if (!common_TargetSubID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure TargetSubID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_TargetSubID = "TargetSubID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_TargetSubID;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_TargetSubID = TypeObjectUtils::build_complete_member_detail(name_TargetSubID, member_ann_builtin_TargetSubID, ann_custom_Header);
            CompleteStructMember member_TargetSubID = TypeObjectUtils::build_complete_struct_member(common_TargetSubID, detail_TargetSubID);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_TargetSubID);
        }
        {
            TypeIdentifierPair type_ids_SendingTime;
            ReturnCode_t return_code_SendingTime {eprosima::fastdds::dds::RETCODE_OK};
            return_code_SendingTime =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_uint64_t", type_ids_SendingTime);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_SendingTime)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "SendingTime Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_SendingTime = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_SendingTime = 0x00000008;
            bool common_SendingTime_ec {false};
            CommonStructMember common_SendingTime {TypeObjectUtils::build_common_struct_member(member_id_SendingTime, member_flags_SendingTime, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_SendingTime, common_SendingTime_ec))};
            if (!common_SendingTime_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure SendingTime member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_SendingTime = "SendingTime";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_SendingTime;
            ann_custom_Header.reset();
            CompleteMemberDetail detail_SendingTime = TypeObjectUtils::build_complete_member_detail(name_SendingTime, member_ann_builtin_SendingTime, ann_custom_Header);
            CompleteStructMember member_SendingTime = TypeObjectUtils::build_complete_struct_member(common_SendingTime, detail_SendingTime);
            TypeObjectUtils::add_complete_struct_member(member_seq_Header, member_SendingTime);
        }
        CompleteStructType struct_type_Header = TypeObjectUtils::build_complete_struct_type(struct_flags_Header, header_Header, member_seq_Header);
        if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                TypeObjectUtils::build_and_register_struct_type_object(struct_type_Header, type_name_Header.to_string(), type_ids_Header))
        {
            EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                    "DistributedATS::Header already registered in TypeObjectRegistry for a different type.");
        }
    }
}

} // namespace DistributedATS

