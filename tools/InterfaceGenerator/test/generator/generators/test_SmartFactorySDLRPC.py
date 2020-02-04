"""Test for SDLRPC SmartFactory generator.

Verifies format specific functions and produced source code.

"""
import codecs
import collections
import os
import unittest
import uuid
from pathlib import Path
from unittest.mock import MagicMock
from unittest.mock import call

import sys

sys.path.append(Path(__file__).absolute().parents[3].as_posix())
sys.path.append(Path(__file__).absolute().parents[4].joinpath('rpc_spec/InterfaceParser').as_posix())
try:
    from generator.generators import SmartFactorySDLRPC
    from model.array import Array
    from model.boolean import Boolean
    from model.float import Float
    from model.enum import Enum
    from model.enum_element import EnumElement
    from model.enum_subset import EnumSubset
    from model.function import Function
    from model.integer import Integer
    from model.interface import Interface
    from model.issue import Issue
    from model.param import Param
    from model.struct import Struct
except ModuleNotFoundError as error:
    print('{}.\nProbably you did not initialize submodule'.format(error))
    sys.exit(1)

EXPECTED_NOTIFICATION_RESULT = (
    u"""params_members[ns_smart_device_link::ns_json_handler::"""
    u"""strings::S_FUNCTION_ID] = SMember("""
    u"""TEnumSchemaItem<FunctionID::eType>::create("""
    u"""function_id_items), true);\n"""
    u"""params_members[ns_smart_device_link::ns_json_handler::"""
    u"""strings::S_MESSAGE_TYPE] = SMember("""
    u"""TEnumSchemaItem<messageType::eType>::create("""
    u"""message_type_items), true);\n"""
    u"""params_members[ns_smart_device_link::ns_json_handler::"""
    u"""strings::S_PROTOCOL_VERSION] = SMember("""
    u"""TNumberSchemaItem<int>::create(), true);\n"""
    u"""params_members[ns_smart_device_link::ns_json_handler::"""
    u"""strings::S_PROTOCOL_TYPE] = SMember("""
    u"""TNumberSchemaItem<int>::create(), true);\n""")

EXPECTED_REQ_RESP_RESULT = "".join([EXPECTED_NOTIFICATION_RESULT, (
    u"""params_members[ns_smart_device_link::ns_json_handler::"""
    u"""strings::S_CORRELATION_ID] = SMember("""
    u"""TNumberSchemaItem<int>::create(), true);\n""")])

DESCRIPTION = [u"Description Line1", u"Description Line2"]

DESIGN_DESCRIPTION = [u"Design Line1"]

ISSUES = [Issue(value=u"Issue1"),
          Issue(value=u"Issue2"),
          Issue(value=u"Issue3")]

TODOS = [u"Do1", u"Do2"]


class Test(unittest.TestCase):
    """Test for SLDRPC SmartFactory generator.

    This class holds set of test cases for the SDLRPC SmartFactory generator.

    """

    def test_gen_schema_params_fill(self):
        """Test feature that allows to create format specific PARAMS.

        Verifies SDLRPC implementation of the _gen_schema_params_fill
        method.

        """

        generator = SmartFactorySDLRPC.CodeGenerator()

        self.assertEqual(generator._gen_schema_params_fill("request"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation for request")

        self.assertEqual(generator._gen_schema_params_fill(u"request"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation for request")

        self.assertEqual(generator._gen_schema_params_fill("response"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation for response")

        self.assertEqual(generator._gen_schema_params_fill(u"response"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation for response")

        self.assertEqual(generator._gen_schema_params_fill("notification"),
                         EXPECTED_NOTIFICATION_RESULT,
                         "Invalid code generation for notification")

        self.assertEqual(generator._gen_schema_params_fill(u"notification"),
                         EXPECTED_NOTIFICATION_RESULT,
                         "Invalid code generation for notification")

        self.assertEqual(generator._gen_schema_params_fill("some text"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation")

        self.assertEqual(generator._gen_schema_params_fill(u"some text"),
                         EXPECTED_REQ_RESP_RESULT,
                         "Invalid code generation")

    def test_full_generation(self):
        """Test full generation using SDLPRC SmartSchema generator.

        Creates output files which is captured by the mock and compare them
        with sample files with correct code. This test requires valid
        test_expected_sdlrpc.h and test_expected_sdlrpc.cc in the same as
        this module.

        """
        self.maxDiff = None
        expected_h_file_content = Path(__file__).parents[0].joinpath('test_expected_sdlrpc.h').read_text()
        expected_cc_file_content = Path(__file__).parents[0].joinpath('test_expected_sdlrpc.cc').read_text()

        generator = SmartFactorySDLRPC.CodeGenerator()

        message_type_elements = collections.OrderedDict()
        message_type_elements[u"request"] = EnumElement(name=u"request")
        message_type_elements[u"response"] = EnumElement(
            name=u"response")
        message_type_elements[u"notification"] = EnumElement(
            name=u"notification")

        message_type = Enum(name=u"messageType",
                            elements=message_type_elements)

        elements1 = collections.OrderedDict()
        elements1[u"name1"] = EnumElement(
            name=u"name1",
            design_description=DESIGN_DESCRIPTION,
            todos=TODOS,
            value=u"1")
        elements1[u"name2"] = EnumElement(
            name="name2",
            description=DESCRIPTION,
            issues=ISSUES,
            internal_name=u"internal_name2")

        enum1 = Enum(name=u"Enum1",
                     todos=TODOS,
                     elements=elements1)

        elements2 = collections.OrderedDict()
        elements2[u"xxx"] = EnumElement(name=u"xxx",
                                        internal_name=u"val_1")
        elements2[u"yyy"] = EnumElement(name=u"yyy",
                                        internal_name=u"val_2",
                                        value=u"100")
        elements2[u"zzz"] = EnumElement(name=u"val_3")

        enum2 = Enum(name=u"E2",
                     elements=elements2)

        elements3 = collections.OrderedDict()
        elements3["1"] = EnumElement(name="xxx",
                                     internal_name="_1")
        elements3["2"] = EnumElement(name="xxx",
                                     internal_name="_2")
        elements3["3"] = EnumElement(name="xxx",
                                     internal_name="_3")
        enum3 = Enum(name="Enum_new2",
                     elements=elements3)

        elements4 = collections.OrderedDict()
        elements4["name1"] = EnumElement(name="xxx",
                                         internal_name="_11")
        elements4["name2"] = EnumElement(name="xxx",
                                         internal_name="_22")
        enum4 = Enum(name="Enum_new4",
                     elements=elements4)

        enums = collections.OrderedDict()
        enums["Enum1"] = enum1
        enums["Enum2"] = enum2
        enums["Enum3"] = enum3
        enums["Enum4"] = enum4
        enums["messageType"] = message_type

        params1 = collections.OrderedDict()
        params1["1"] = Param(
            name="param1",
            design_description=DESIGN_DESCRIPTION,
            description=DESCRIPTION,
            issues=ISSUES,
            todos=TODOS,
            param_type=enum4,
            default_value=elements4["name1"])
        params1["2"] = Param(
            name="param2",
            param_type=EnumSubset(
                name="sub1",
                enum=enum1,
                allowed_elements={"e1": elements1["name1"]}),
            default_value=elements1["name1"])

        functions = collections.OrderedDict()
        functions["Function1"] = Function(
            name="Function1",
            function_id=elements1["name1"],
            message_type=message_type_elements["request"],
            params=params1)
        functions["Function2"] = Function(
            name="Function2",
            function_id=elements2["xxx"],
            message_type=message_type_elements["response"])
        functions["Function3"] = Function(
            name="Function2",
            function_id=elements2["yyy"],
            message_type=message_type_elements["notification"])

        members1 = collections.OrderedDict()
        members1["m1"] = Param(name="intParam",
                               param_type=Integer(max_value=2))
        members1["m11"] = Param(name="doubleParam",
                                param_type=Float(min_value=0.333),
                                is_mandatory=False)
        members1["m222"] = Param(name="boolParam",
                                 param_type=Boolean())
        members1["m2"] = Param(name="structParam",
                               param_type=Struct(name="Struct2"))
        members1["aaa"] = Param(name="enumParam",
                                param_type=enum1)
        members1["bbb"] = Param(name="enumParam1",
                                param_type=enum1)
        members1["xxx"] = Param(
            name="enumSubset1",
            param_type=EnumSubset(
                name="sub",
                enum=enum1,
                allowed_elements={"e1": elements1["name1"]}),
            is_mandatory=False)
        members1["1"] = Param(
            name="arrayOfInt",
            param_type=Array(min_size=0,
                             max_size=20,
                             element_type=Boolean()),
            is_mandatory=False)
        members1["2"] = Param(
            name="arrayOfEnum1",
            param_type=Array(min_size=0,
                             max_size=20,
                             element_type=enum1),
            is_mandatory=False)
        members1["3"] = Param(
            name="arrayOfEnum3",
            param_type=Array(min_size=10,
                             max_size=40,
                             element_type=enum3),
            is_mandatory=True)
        members1["4"] = Param(
            name="arrayOfEnum4",
            param_type=Array(
                min_size=10,
                max_size=41,
                element_type=EnumSubset(
                    name="sub1",
                    enum=enum1,
                    allowed_elements={"e1": elements1["name1"]})))
        members1["5"] = Param(
            name="arrayOfEnum5",
            param_type=Array(
                min_size=10,
                max_size=42,
                element_type=EnumSubset(
                    name="sub2",
                    enum=enum1,
                    allowed_elements={"e1": elements1["name2"]})))
        members1["6"] = Param(
            name="arrayOfEnum6",
            param_type=Array(
                min_size=10,
                max_size=43,
                element_type=EnumSubset(
                    name="sub3",
                    enum=enum4,
                    allowed_elements={"e1": elements4["name2"]})))

        structs = collections.OrderedDict()
        structs["Struct1"] = Struct(
            name="Struct1",
            design_description=DESIGN_DESCRIPTION,
            issues=ISSUES,
            members=members1)
        structs["Struct2"] = Struct(name="Struct2",
                                    issues=ISSUES)

        interface = Interface(enums=enums,
                              structs=structs,
                              functions=functions,
                              params={"param1": "value1",
                                      "param2": "value2"})

        os.path.exists = MagicMock(return_value=True)
        uuid.uuid1 = MagicMock(
            return_value=uuid.UUID("12345678123456781234567812345678"))
        codecs.open = MagicMock()

        generator.generate(interface=interface,
                           filename="Test.xml",
                           namespace="XXX::YYY::ZZZ",
                           destination_dir="/some/test/dir")

        os.path.exists.assert_has_calls([call('/some/test/dir')])

        open_result = codecs.open
        mock_calls = open_result.mock_calls

        self.assertEqual(mock_calls[0],
                         call('/some/test/dir/Test.h',
                              mode='w',
                              encoding='utf-8'),
                         "Invalid header file creation")

        self.assertEqual(mock_calls[4],
                         call('/some/test/dir/Test_schema.h',
                              mode='w',
                              encoding='utf-8'),
                         "Invalid source file creation")
        try:
            self.assertSequenceEqual(str(mock_calls[2])[27:-2].replace("\\n", "\n"),
                                     expected_h_file_content,
                                     "Invalid header file content")

            self.assertSequenceEqual(str(mock_calls[6])[27:-2].replace("\\n", "\n"),
                                     expected_cc_file_content,
                                     "Invalid source file content")
        except AssertionError as message:
            print(message)


if __name__ == '__main__':
    unittest.main()
