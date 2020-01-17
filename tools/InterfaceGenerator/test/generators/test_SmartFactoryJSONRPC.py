"""Test for JSONRPC SmartFactory generator.

Verifies format specific functions and produced source code.

"""
import codecs
import collections
import os
import sys
import unittest
import uuid
from pathlib import Path

from mock import MagicMock
from mock import call

sys.path.append(Path(__file__).absolute().parents[2].as_posix())
sys.path.append(Path(__file__).absolute().parents[3].joinpath('rpc_spec/InterfaceParser').as_posix())

try:
    from generators import SmartFactoryJSONRPC
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

EXPECTED_RESULT_REQUEST = (
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_FUNCTION_ID] = SMember("""
    """TEnumSchemaItem<FunctionID::eType>::create("""
    """function_id_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_MESSAGE_TYPE] = SMember("""
    """TEnumSchemaItem<messageType::eType>::create("""
    """message_type_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_VERSION] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_TYPE] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_CORRELATION_ID] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
)

EXPECTED_RESULT_RESPONSE = (
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_FUNCTION_ID] = SMember("""
    """TEnumSchemaItem<FunctionID::eType>::create("""
    """function_id_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_MESSAGE_TYPE] = SMember("""
    """TEnumSchemaItem<messageType::eType>::create("""
    """message_type_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_VERSION] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_TYPE] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_CORRELATION_ID] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::kCode] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
)

EXPECTED_RESULT_NOTIFICATION = (
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_FUNCTION_ID] = SMember("""
    """TEnumSchemaItem<FunctionID::eType>::create("""
    """function_id_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_MESSAGE_TYPE] = SMember("""
    """TEnumSchemaItem<messageType::eType>::create("""
    """message_type_items), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_VERSION] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_TYPE] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
)

EXPECTED_PRE_FUNCTION_CODE = (
    """  std::map<std::string, SMember> """
    """params_members;\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_FUNCTION_ID] = SMember("""
    """TEnumSchemaItem<FunctionID::eType>::create("""
    """function_id_items), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_MESSAGE_TYPE] = SMember("""
    """TEnumSchemaItem<messageType::eType>::create("""
    """message_type_items), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_VERSION] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PROTOCOL_TYPE] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::S_CORRELATION_ID] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler"""
    """::strings::kCode] = SMember("""
    """TNumberSchemaItem<int>::create(), true);\n"""
    """  params_members[ns_smart_device_link::ns_json_handler::"""
    """strings::kMessage] = SMember("""
    """CStringSchemaItem::create(), true);\n"""
    """\n"""
    """  std::map<std::string, SMember> """
    """root_members_map;\n"""
    """  root_members_map[ns_smart_device_link::ns_json_handler::"""
    """strings::S_PARAMS] = SMember("""
    """CObjectSchemaItem::create(params_members), true);\n"""
    """\n"""
    """  CSmartSchema error_response_schema(CObjectSchemaItem::create("""
    """root_members_map));\n"""
    """\n"""
    """  functions_schemes_.insert(std::make_pair("""
    """ns_smart_device_link::ns_json_handler::SmartSchemaKey<"""
    """FunctionID::eType, messageType::eType>("""
    """FunctionID::request, messageType::error_response)"""
    """, error_response_schema));\n"""
    """\n"""
)

DESCRIPTION = ["Description Line1", "Description Line2"]

DESIGN_DESCRIPTION = ["Design Line1"]

ISSUES = [Issue(value="Issue1"),
          Issue(value="Issue2"),
          Issue(value="Issue3")]

TODOS = ["Do1", "Do2"]


class Test(unittest.TestCase):
    """Test for JSONRPC SmartFactory generator.

    This class holds set of test cases for the JSONRPC SmartFactory generator.

    """

    def test_gen_schema_params_fill(self):
        """Test feature that allows to create format specific PARAMS.

        Verifies JSONRPC implementation of the _gen_schema_params_fill
        method.

        """
        generator = SmartFactoryJSONRPC.CodeGenerator()

        self.assertEqual(generator._gen_schema_params_fill("request"),
                         EXPECTED_RESULT_REQUEST,
                         "Invalid code generation for request")

        self.assertEqual(generator._gen_schema_params_fill("request"),
                         EXPECTED_RESULT_REQUEST,
                         "Invalid code generation for request")

        self.assertEqual(generator._gen_schema_params_fill("response"),
                         EXPECTED_RESULT_RESPONSE,
                         "Invalid code generation for response")

        self.assertEqual(generator._gen_schema_params_fill("response"),
                         EXPECTED_RESULT_RESPONSE,
                         "Invalid code generation for response")

        self.assertEqual(generator._gen_schema_params_fill("notification"),
                         EXPECTED_RESULT_NOTIFICATION,
                         "Invalid code generation for notification")

        self.assertEqual(generator._gen_schema_params_fill("notification"),
                         EXPECTED_RESULT_NOTIFICATION,
                         "Invalid code generation for notification")

    def test_preprocess_message_type(self):
        """Test preprocessing of the message_type enum.

        Verifies JSONPRC implementation of the _preprocess_message_type
        function.

        """

        generator = SmartFactoryJSONRPC.CodeGenerator()

        message_type_elements = collections.OrderedDict()
        message_type_elements["request"] = EnumElement(name="request")
        message_type_elements["response"] = EnumElement(
            name="response")
        message_type_elements["notification"] = EnumElement(
            name="notification")

        message_type = Enum(name="messageType",
                            elements=message_type_elements)

        result_enum = generator._preprocess_message_type(message_type)

        self.assertIn("error_response", result_enum.elements)
        self.assertEqual("error_response",
                         result_enum.elements["error_response"].primary_name)

        message_type_elements = collections.OrderedDict()
        message_type_elements["request"] = EnumElement(name="request")
        message_type_elements["notification"] = EnumElement(
            name="notification")

        message_type = Enum(name="messageType",
                            elements=message_type_elements)

        result_enum = generator._preprocess_message_type(message_type)

        self.assertNotIn("error_response", result_enum.elements)

    def test_gen_pre_function_schemas(self):
        """Test code that goes before schema initialization.

        Verifies JSONPRC implementation of the _gen_pre_function_schemas
        function.

        """

        generator = SmartFactoryJSONRPC.CodeGenerator()

        self.assertEqual("",
                         generator._gen_pre_function_schemas([]),
                         "Invalid code for empty functions list")

        message_type_elements = collections.OrderedDict()
        message_type_elements["request"] = EnumElement(name="request")
        message_type_elements["response"] = EnumElement(
            name="response")
        message_type_elements["notification"] = EnumElement(
            name="notification")

        message_type = Enum(name="messageType",
                            elements=message_type_elements)

        function1 = Function(
            "func1", function_id=message_type.elements["request"],
            message_type=message_type.elements["request"])

        self.assertEqual("",
                         generator._gen_pre_function_schemas([function1]),
                         "Invalid code for empty functions list")

        function2 = Function(
            "func2", function_id=message_type.elements["request"],
            message_type=message_type.elements["response"])
        try:
            self.assertEqual(EXPECTED_PRE_FUNCTION_CODE,
                             generator._gen_pre_function_schemas([function2]),
                             "Invalid code for single response function")
        except AssertionError as message:
            message = message.args[0]
            print(message)
            self.assertTrue(message.endswith("Invalid code for single response function"))

        try:
            self.assertEqual(EXPECTED_PRE_FUNCTION_CODE,
                             generator._gen_pre_function_schemas([function1,
                                                                  function2]),
                             "Invalid code for mixed function list")
        except AssertionError as message:
            message = message.args[0]
            print(message)
            self.assertTrue(message.endswith("Invalid code for mixed function list"))

    def test_full_generation(self):
        """Test full generation using JSONRPC SmartSchema generator.

        Creates output files which is captured by the mock and compare them
        with sample files with correct code. This test requires valid
        test_expected_jsonrpc.h and test_expected_jsonrpc.cc in the same
        directory as this module.

        """
        with open('test_expected_jsonrpc.h', 'r') as f:
            expected_h_file_content = f.read()
        with open('test_expected_jsonrpc.cc', 'r')as f:
            expected_cc_file_content = f.read()

        generator = SmartFactoryJSONRPC.CodeGenerator()

        message_type_elements = collections.OrderedDict()
        message_type_elements["request"] = EnumElement(name="request")
        message_type_elements["response"] = EnumElement(
            name="response")
        message_type_elements["notification"] = EnumElement(
            name="notification")

        message_type = Enum(name="messageType",
                            elements=message_type_elements)

        elements1 = collections.OrderedDict()
        elements1["name1"] = EnumElement(
            name="name1",
            design_description=DESIGN_DESCRIPTION,
            todos=TODOS,
            value="1")
        elements1["name2"] = EnumElement(
            name="name2",
            description=DESCRIPTION,
            issues=ISSUES,
            internal_name="internal_name2")

        enum1 = Enum(name="Enum1",
                     todos=TODOS,
                     elements=elements1)

        elements2 = collections.OrderedDict()
        elements2["xxx"] = EnumElement(name="xxx",
                                       internal_name="val_1")
        elements2["yyy"] = EnumElement(name="yyy",
                                       internal_name="val_2",
                                       value="100")
        elements2["zzz"] = EnumElement(name="val_3")

        enum2 = Enum(name="E2",
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
        try:
            self.assertEqual(mock_calls[4],
                             call('/some/test/dir/Test.cc',
                                  mode='w',
                                  encoding='utf-8'),
                             "Invalid source file creation")
        except AssertionError as message:
            message = message.args[0]
            print(message)
            self.assertTrue(message.endswith("Invalid source file creation"))

        try:
            self.assertEqual(str(mock_calls[2])[27:-2].replace("\\n", "\n"),
                             expected_h_file_content,
                             "Invalid header file content")
        except AssertionError as message:
            message = message.args[0]
            print(message)
            self.assertTrue(message.endswith("Invalid header file content"))

        try:
            self.assertEqual(str(mock_calls[6])[27:-2].replace("\\n", "\n"),
                             expected_cc_file_content,
                             "Invalid source file content")
        except AssertionError as message:
            message = message.args[0]
            print(message)
            self.assertTrue(message.endswith("Invalid source file content"))


if __name__ == '__main__':
    unittest.main()
