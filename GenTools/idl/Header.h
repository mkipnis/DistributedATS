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
 * @file Header.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_HEADER_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_HEADER_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>



#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(HEADER_SOURCE)
#define HEADER_DllAPI __declspec( dllexport )
#else
#define HEADER_DllAPI __declspec( dllimport )
#endif // HEADER_SOURCE
#else
#define HEADER_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define HEADER_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima



namespace DistributedATS {



/*!
 * @brief This class represents the structure Header defined by the user in the IDL file.
 * @ingroup Header
 */
class Header
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport Header();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~Header();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DistributedATS::Header that will be copied.
     */
    eProsima_user_DllExport Header(
            const Header& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DistributedATS::Header that will be copied.
     */
    eProsima_user_DllExport Header(
            Header&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DistributedATS::Header that will be copied.
     */
    eProsima_user_DllExport Header& operator =(
            const Header& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DistributedATS::Header that will be copied.
     */
    eProsima_user_DllExport Header& operator =(
            Header&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS::Header object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const Header& x) const;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS::Header object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const Header& x) const;

    /*!
     * @brief This function copies the value in member BeginString
     * @param _BeginString New value to be copied in member BeginString
     */
    eProsima_user_DllExport void BeginString(
            const std::string& _BeginString);

    /*!
     * @brief This function moves the value in member BeginString
     * @param _BeginString New value to be moved in member BeginString
     */
    eProsima_user_DllExport void BeginString(
            std::string&& _BeginString);

    /*!
     * @brief This function returns a constant reference to member BeginString
     * @return Constant reference to member BeginString
     */
    eProsima_user_DllExport const std::string& BeginString() const;

    /*!
     * @brief This function returns a reference to member BeginString
     * @return Reference to member BeginString
     */
    eProsima_user_DllExport std::string& BeginString();


    /*!
     * @brief This function sets a value in member BodyLength
     * @param _BodyLength New value for member BodyLength
     */
    eProsima_user_DllExport void BodyLength(
            int32_t _BodyLength);

    /*!
     * @brief This function returns the value of member BodyLength
     * @return Value of member BodyLength
     */
    eProsima_user_DllExport int32_t BodyLength() const;

    /*!
     * @brief This function returns a reference to member BodyLength
     * @return Reference to member BodyLength
     */
    eProsima_user_DllExport int32_t& BodyLength();


    /*!
     * @brief This function copies the value in member MsgType
     * @param _MsgType New value to be copied in member MsgType
     */
    eProsima_user_DllExport void MsgType(
            const std::string& _MsgType);

    /*!
     * @brief This function moves the value in member MsgType
     * @param _MsgType New value to be moved in member MsgType
     */
    eProsima_user_DllExport void MsgType(
            std::string&& _MsgType);

    /*!
     * @brief This function returns a constant reference to member MsgType
     * @return Constant reference to member MsgType
     */
    eProsima_user_DllExport const std::string& MsgType() const;

    /*!
     * @brief This function returns a reference to member MsgType
     * @return Reference to member MsgType
     */
    eProsima_user_DllExport std::string& MsgType();


    /*!
     * @brief This function copies the value in member SenderCompID
     * @param _SenderCompID New value to be copied in member SenderCompID
     */
    eProsima_user_DllExport void SenderCompID(
            const std::string& _SenderCompID);

    /*!
     * @brief This function moves the value in member SenderCompID
     * @param _SenderCompID New value to be moved in member SenderCompID
     */
    eProsima_user_DllExport void SenderCompID(
            std::string&& _SenderCompID);

    /*!
     * @brief This function returns a constant reference to member SenderCompID
     * @return Constant reference to member SenderCompID
     */
    eProsima_user_DllExport const std::string& SenderCompID() const;

    /*!
     * @brief This function returns a reference to member SenderCompID
     * @return Reference to member SenderCompID
     */
    eProsima_user_DllExport std::string& SenderCompID();


    /*!
     * @brief This function copies the value in member TargetCompID
     * @param _TargetCompID New value to be copied in member TargetCompID
     */
    eProsima_user_DllExport void TargetCompID(
            const std::string& _TargetCompID);

    /*!
     * @brief This function moves the value in member TargetCompID
     * @param _TargetCompID New value to be moved in member TargetCompID
     */
    eProsima_user_DllExport void TargetCompID(
            std::string&& _TargetCompID);

    /*!
     * @brief This function returns a constant reference to member TargetCompID
     * @return Constant reference to member TargetCompID
     */
    eProsima_user_DllExport const std::string& TargetCompID() const;

    /*!
     * @brief This function returns a reference to member TargetCompID
     * @return Reference to member TargetCompID
     */
    eProsima_user_DllExport std::string& TargetCompID();


    /*!
     * @brief This function sets a value in member MsgSeqNum
     * @param _MsgSeqNum New value for member MsgSeqNum
     */
    eProsima_user_DllExport void MsgSeqNum(
            int32_t _MsgSeqNum);

    /*!
     * @brief This function returns the value of member MsgSeqNum
     * @return Value of member MsgSeqNum
     */
    eProsima_user_DllExport int32_t MsgSeqNum() const;

    /*!
     * @brief This function returns a reference to member MsgSeqNum
     * @return Reference to member MsgSeqNum
     */
    eProsima_user_DllExport int32_t& MsgSeqNum();


    /*!
     * @brief This function copies the value in member SenderSubID
     * @param _SenderSubID New value to be copied in member SenderSubID
     */
    eProsima_user_DllExport void SenderSubID(
            const std::string& _SenderSubID);

    /*!
     * @brief This function moves the value in member SenderSubID
     * @param _SenderSubID New value to be moved in member SenderSubID
     */
    eProsima_user_DllExport void SenderSubID(
            std::string&& _SenderSubID);

    /*!
     * @brief This function returns a constant reference to member SenderSubID
     * @return Constant reference to member SenderSubID
     */
    eProsima_user_DllExport const std::string& SenderSubID() const;

    /*!
     * @brief This function returns a reference to member SenderSubID
     * @return Reference to member SenderSubID
     */
    eProsima_user_DllExport std::string& SenderSubID();


    /*!
     * @brief This function copies the value in member TargetSubID
     * @param _TargetSubID New value to be copied in member TargetSubID
     */
    eProsima_user_DllExport void TargetSubID(
            const std::string& _TargetSubID);

    /*!
     * @brief This function moves the value in member TargetSubID
     * @param _TargetSubID New value to be moved in member TargetSubID
     */
    eProsima_user_DllExport void TargetSubID(
            std::string&& _TargetSubID);

    /*!
     * @brief This function returns a constant reference to member TargetSubID
     * @return Constant reference to member TargetSubID
     */
    eProsima_user_DllExport const std::string& TargetSubID() const;

    /*!
     * @brief This function returns a reference to member TargetSubID
     * @return Reference to member TargetSubID
     */
    eProsima_user_DllExport std::string& TargetSubID();


    /*!
     * @brief This function sets a value in member SendingTime
     * @param _SendingTime New value for member SendingTime
     */
    eProsima_user_DllExport void SendingTime(
            uint64_t _SendingTime);

    /*!
     * @brief This function returns the value of member SendingTime
     * @return Value of member SendingTime
     */
    eProsima_user_DllExport uint64_t SendingTime() const;

    /*!
     * @brief This function returns a reference to member SendingTime
     * @return Reference to member SendingTime
     */
    eProsima_user_DllExport uint64_t& SendingTime();

private:

    std::string m_BeginString;
    int32_t m_BodyLength{0};
    std::string m_MsgType;
    std::string m_SenderCompID;
    std::string m_TargetCompID;
    int32_t m_MsgSeqNum{0};
    std::string m_SenderSubID;
    std::string m_TargetSubID;
    uint64_t m_SendingTime{0};

};

} // namespace DistributedATS

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_HEADER_H_


