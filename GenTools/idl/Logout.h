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
 * @file Logout.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_LOGOUT_LOGOUT_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_LOGOUT_LOGOUT_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>

#include "Header.h"


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
#if defined(LOGOUT_SOURCE)
#define LOGOUT_DllAPI __declspec( dllexport )
#else
#define LOGOUT_DllAPI __declspec( dllimport )
#endif // LOGOUT_SOURCE
#else
#define LOGOUT_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define LOGOUT_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima



namespace DistributedATS_Logout {



/*!
 * @brief This class represents the structure Logout defined by the user in the IDL file.
 * @ingroup Logout
 */
class Logout
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport Logout();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~Logout();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DistributedATS_Logout::Logout that will be copied.
     */
    eProsima_user_DllExport Logout(
            const Logout& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DistributedATS_Logout::Logout that will be copied.
     */
    eProsima_user_DllExport Logout(
            Logout&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DistributedATS_Logout::Logout that will be copied.
     */
    eProsima_user_DllExport Logout& operator =(
            const Logout& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DistributedATS_Logout::Logout that will be copied.
     */
    eProsima_user_DllExport Logout& operator =(
            Logout&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_Logout::Logout object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const Logout& x) const;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_Logout::Logout object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const Logout& x) const;

    /*!
     * @brief This function copies the value in member header
     * @param _header New value to be copied in member header
     */
    eProsima_user_DllExport void header(
            const DistributedATS::Header& _header);

    /*!
     * @brief This function moves the value in member header
     * @param _header New value to be moved in member header
     */
    eProsima_user_DllExport void header(
            DistributedATS::Header&& _header);

    /*!
     * @brief This function returns a constant reference to member header
     * @return Constant reference to member header
     */
    eProsima_user_DllExport const DistributedATS::Header& header() const;

    /*!
     * @brief This function returns a reference to member header
     * @return Reference to member header
     */
    eProsima_user_DllExport DistributedATS::Header& header();


    /*!
     * @brief This function copies the value in member Text
     * @param _Text New value to be copied in member Text
     */
    eProsima_user_DllExport void Text(
            const std::string& _Text);

    /*!
     * @brief This function moves the value in member Text
     * @param _Text New value to be moved in member Text
     */
    eProsima_user_DllExport void Text(
            std::string&& _Text);

    /*!
     * @brief This function returns a constant reference to member Text
     * @return Constant reference to member Text
     */
    eProsima_user_DllExport const std::string& Text() const;

    /*!
     * @brief This function returns a reference to member Text
     * @return Reference to member Text
     */
    eProsima_user_DllExport std::string& Text();

private:

    DistributedATS::Header m_header;
    std::string m_Text;

};

} // namespace DistributedATS_Logout

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_LOGOUT_LOGOUT_H_



