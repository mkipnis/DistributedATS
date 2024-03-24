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
 * @file Logout.cpp
 * This source file contains the implementation of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "Logout.h"

#include "LogoutTypeObject.h"

#include <fastcdr/Cdr.h>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>


namespace DistributedATS_Logout {



Logout::Logout()
{
    // Just to register all known types
    registerLogoutTypes();
}

Logout::~Logout()
{
}

Logout::Logout(
        const Logout& x)
{
    m_header = x.m_header;
    m_Text = x.m_Text;
}

Logout::Logout(
        Logout&& x) noexcept
{
    m_header = std::move(x.m_header);
    m_Text = std::move(x.m_Text);
}

Logout& Logout::operator =(
        const Logout& x)
{

    m_header = x.m_header;
    m_Text = x.m_Text;
    return *this;
}

Logout& Logout::operator =(
        Logout&& x) noexcept
{

    m_header = std::move(x.m_header);
    m_Text = std::move(x.m_Text);
    return *this;
}

bool Logout::operator ==(
        const Logout& x) const
{
    return (m_header == x.m_header &&
           m_Text == x.m_Text);
}

bool Logout::operator !=(
        const Logout& x) const
{
    return !(*this == x);
}

/*!
 * @brief This function copies the value in member header
 * @param _header New value to be copied in member header
 */
void Logout::header(
        const DistributedATS::Header& _header)
{
    m_header = _header;
}

/*!
 * @brief This function moves the value in member header
 * @param _header New value to be moved in member header
 */
void Logout::header(
        DistributedATS::Header&& _header)
{
    m_header = std::move(_header);
}

/*!
 * @brief This function returns a constant reference to member header
 * @return Constant reference to member header
 */
const DistributedATS::Header& Logout::header() const
{
    return m_header;
}

/*!
 * @brief This function returns a reference to member header
 * @return Reference to member header
 */
DistributedATS::Header& Logout::header()
{
    return m_header;
}


/*!
 * @brief This function copies the value in member Text
 * @param _Text New value to be copied in member Text
 */
void Logout::Text(
        const std::string& _Text)
{
    m_Text = _Text;
}

/*!
 * @brief This function moves the value in member Text
 * @param _Text New value to be moved in member Text
 */
void Logout::Text(
        std::string&& _Text)
{
    m_Text = std::move(_Text);
}

/*!
 * @brief This function returns a constant reference to member Text
 * @return Constant reference to member Text
 */
const std::string& Logout::Text() const
{
    return m_Text;
}

/*!
 * @brief This function returns a reference to member Text
 * @return Reference to member Text
 */
std::string& Logout::Text()
{
    return m_Text;
}




} // namespace DistributedATS_Logout
// Include auxiliary functions like for serializing/deserializing.
#include "LogoutCdrAux.ipp"
