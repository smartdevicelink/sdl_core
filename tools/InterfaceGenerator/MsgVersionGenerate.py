"""
Generate file with major and minor msg_version.
"""
import xml.etree.ElementTree
from string import Template
import re
from generator.parsers import RPCBase
 
def generate_msg_version(file_name, path_to_storage):
    """Parses MOBILE_API.xml in order to
    receive major_version, minor_version, and patch_version
    """
    tree = xml.etree.ElementTree.parse(file_name)
    root = tree.getroot()
    if (root.tag == "interface" and "version" and "minVersion" in root.attrib):
        check_version_format(root.attrib["version"])
        array = (root.attrib["version"]).split(".")
        major_version = array[0]
        minor_version = array[1]
        patch_version = array[2]

        check_minimum_version_format(root.attrib["minVersion"])
        minimum_version_array = (root.attrib["minVersion"]).split(".")
        if (len(minimum_version_array) == 2):
            minimum_version_array.append("0")
        minimum_major_version = minimum_version_array[0]
        minimum_minor_version = minimum_version_array[1]
        minimum_patch_version = minimum_version_array[2]

        if (major_version.isdigit() and minor_version.isdigit() and patch_version.isdigit() and 
            minimum_major_version.isdigit() and minimum_minor_version.isdigit() and minimum_patch_version.isdigit()):
            data_for_storage = prepare_data_for_storage(major_version, minor_version, patch_version,  
                minimum_major_version,  minimum_minor_version,  minimum_patch_version)
            store_data_to_file(path_to_storage, data_for_storage)  
        else:
            raise RPCBase.ParseError("Attribute version has incorect value in MOBILE_API.xml")
    else:
        raise RPCBase.ParseError("Check MOBILE_API.xml file, parser can not find first element "
                                 " with tag interface or atribute version")

def store_data_to_file(path_to_storage, data_for_storage):
    """Stores data with major and minor version
    to file generated_msg_version.h
    """
    path_to_storage = path_to_storage + "/generated_msg_version.h"
    fh = open(path_to_storage, 'w')
    fh.write(data_for_storage)
    fh.close()

def check_version_format(version):
    """Checks correctness of format of version
    """
    p = re.compile('\d+\\.\d+\\.\d+')
    result = p.match(version)
    if result == None or (result.end() != len(version)):
        raise RPCBase.ParseError("Incorrect format of version please check MOBILE_API.xml. "
                                 "Need format of version major_version.minor_version.patch_version")


def check_minimum_version_format(version):
    """Checks correctness of format of version
    """
    p = re.compile('\d+\\.\d+\\.\d+|\d+\\.\d+')
    result = p.match(version)
    if result == None or (result.end() != len(version)):
        raise RPCBase.ParseError("Incorrect format of version please check MOBILE_API.xml. "
                                 "Need format of minVersion major_version.minor_version or major_version.minor_version.patch_version")
def prepare_data_for_storage(major_version, minor_version, patch_version, minimum_major_version, minimum_minor_version, minimum_patch_version):
    """Prepares data to store to file.
    """
    temp = Template(
    u'''/*Copyright (c) 2016, Ford Motor Company\n'''
    u'''All rights reserved.\n'''
    u'''Redistribution and use in source and binary forms, with or without\n'''
    u'''modification, are permitted provided that the following conditions are met:\n'''
    u'''Redistributions of source code must retain the above copyright notice, this\n'''
    u'''list of conditions and the following disclaimer.\n'''
    u'''Redistributions in binary form must reproduce the above copyright notice,\n'''
    u'''this list of conditions and the following\n'''
    u'''disclaimer in the documentation and/or other materials provided with the\n'''
    u'''distribution.\n'''
    u'''Neither the name of the Ford Motor Company nor the names of its contributors\n'''
    u'''may be used to endorse or promote products derived from this software\n'''
    u'''without specific prior written permission.\n'''
    u'''THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n'''
    u'''AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n'''
    u'''IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n'''
    u'''ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n'''
    u'''LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n'''
    u'''CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n'''
    u'''SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n'''
    u'''INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n'''
    u'''CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n'''
    u'''ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n'''
    u'''POSSIBILITY OF SUCH DAMAGE.\n'''
    u'''*/\n'''
    u'''#ifndef GENERATED_MSG_VERSION_H\n'''
    u'''#define GENERATED_MSG_VERSION_H\n\n'''
    u'''namespace application_manager {\n\n'''
    u'''const uint16_t major_version = $m_version;\n'''
    u'''const uint16_t minor_version = $min_version;\n'''
    u'''const uint16_t patch_version = $p_version;\n'''
    u'''const uint16_t minimum_major_version = $min_major_version;\n'''
    u'''const uint16_t minimum_minor_version = $min_minor_version;\n'''
    u'''const uint16_t minimum_patch_version = $min_patch_version;\n'''
    u'''}  // namespace application_manager\n'''
    u'''#endif  // GENERATED_MSG_VERSION_H''')
    data_to_file = temp.substitute(m_version = major_version, min_version = minor_version, p_version = patch_version,
        min_major_version = minimum_major_version, min_minor_version = minimum_minor_version, min_patch_version = minimum_patch_version)
    return data_to_file
    
