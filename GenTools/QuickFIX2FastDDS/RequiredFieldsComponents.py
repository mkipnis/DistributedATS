"""
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

"""

import xml.etree.ElementTree as xml_parser


class RequiredFieldsComponents:

    def __init__(self, filename):

        self.requirements = xml_parser.parse(filename)
        self.required_messages = {}
        self.required_components = {}
        self.required_fields = {}
        self.types = {}

        messages = self.requirements.find('messages')
        self.populate_elements(messages, self.required_messages)

        components = self.requirements.find('components')
        self.populate_elements(components, self.required_components)

        fields = self.requirements.find('fields')
        self.populate_elements(fields, self.required_fields)

        types = self.requirements.find("types")
        for type_item in types:
            fix_type = type_item.get("fix")
            self.types[fix_type] = type_item

    # elements : messages or components
    def populate_elements(self, elements, required_elements):
        for item in elements:
            element_name = item.get("name")
            required_elements[element_name] = item

