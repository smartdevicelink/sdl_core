"""SDLRPCV1 XML parser unit test."""
import os
import unittest

import generator.Model
import generator.parsers.SDLRPCV1


class TestSDLRPCV1Parser(unittest.TestCase):

    """Test for SDLRPCV1 xml parser."""

    class _Issue:
        def __init__(self, creator, value):
            self.creator = creator
            self.value = value

        def __eq__(self, other):
            return self.creator == other.creator and self.value == other.value

    def setUp(self):
        """Test initialization."""
        self.valid_xml_name = os.path.dirname(os.path.realpath(__file__)) + \
            "/valid_SDLRPCV1.xml"
        self.parser = generator.parsers.SDLRPCV1.Parser()

    def test_valid_xml(self):
        """Test parsing of valid xml."""
        interface = self.parser.parse(self.valid_xml_name)

        self.assertEqual(2, len(interface.params))
        self.assertDictEqual({"attribute1": "value1", "attribute2": "value2"},
                             interface.params)

        # Enumerations

        self.assertEqual(3, len(interface.enums))

        # Enumeration "FunctionID"

        self.assertIn("FunctionID", interface.enums)
        enum = interface.enums["FunctionID"]
        self.verify_base_item(item=enum,
                              name="FunctionID")
        self.assertIsNone(enum.internal_scope)

        self.assertEqual(2, len(enum.elements))

        self.assertIn("Function1", enum.elements)
        element = enum.elements["Function1"]
        self.verify_base_item(
            item=element,
            name="Function1")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        self.assertIn("Function2", enum.elements)
        element = enum.elements["Function2"]
        self.verify_base_item(
            item=element,
            name="Function2")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        # Enumeration "messageType"

        self.assertIn("messageType", interface.enums)
        enum = interface.enums["messageType"]
        self.verify_base_item(
            item=enum,
            name="messageType")
        self.assertIsNone(enum.internal_scope)

        self.assertEqual(3, len(enum.elements))

        self.assertIn("request", enum.elements)
        element = enum.elements["request"]
        self.verify_base_item(item=element,
                              name="request")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        self.assertIn("response", enum.elements)
        element = enum.elements["response"]
        self.verify_base_item(item=element, name="response")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        self.assertIn("notification", enum.elements)
        element = enum.elements["notification"]
        self.verify_base_item(item=element, name="notification")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        # Enumeration "enum1"

        self.assertIn("enum1", interface.enums)
        enum = interface.enums["enum1"]
        self.verify_base_item(item=enum, name="enum1",
                              platform="enum1 platform")
        self.assertEqual("scope", enum.internal_scope)

        self.assertEqual(3, len(enum.elements))

        self.assertIn("element1", enum.elements)
        element = enum.elements["element1"]
        self.verify_base_item(item=element, name="element1")
        self.assertIsNone(element.internal_name)
        self.assertEqual(10, element.value)

        self.assertIn("element2", enum.elements)
        element = enum.elements["element2"]
        self.verify_base_item(item=element, name="element2")
        self.assertEqual("element2_internal", element.internal_name)
        self.assertEqual(11, element.value)

        self.assertIn("element3", enum.elements)
        element = enum.elements["element3"]
        self.verify_base_item(
            item=element,
            name="element3",
            design_description=["Element design description"],
            platform="element3 platform")
        self.assertIsNone(element.internal_name)
        self.assertIsNone(element.value)

        # Structures

        self.assertEqual(2, len(interface.structs))

        # Structure "struct1"

        self.assertIn("struct1", interface.structs)
        struct = interface.structs["struct1"]
        self.verify_base_item(
            item=struct,
            name="struct1",
            description=["Struct description"],
            issues=[TestSDLRPCV1Parser._Issue(creator="creator1",
                                              value="Issue1"),
                    TestSDLRPCV1Parser._Issue(creator="creator2",
                                              value="Issue2")])

        self.assertEqual(4, len(struct.members))

        self.assertIn("member1", struct.members)
        member = struct.members["member1"]
        self.verify_base_item(
            item=member,
            name="member1",
            description=["Param1 description"])
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Integer)
        self.assertIsNone(member.param_type.min_value)
        self.assertIsNone(member.param_type.max_value)

        self.assertIn("member2", struct.members)
        member = struct.members["member2"]
        self.verify_base_item(item=member, name="member2",
                              platform="member2 platform")
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Boolean)

        self.assertIn("member3", struct.members)
        member = struct.members["member3"]
        self.verify_base_item(item=member, name="member3")
        self.assertEqual(False, member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Double)
        self.assertIsNone(member.param_type.min_value)
        self.assertAlmostEqual(20.5, member.param_type.max_value)

        self.assertIn("member4", struct.members)
        member = struct.members["member4"]
        self.verify_base_item(item=member, name="member4")
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Array)
        self.assertIsNone(member.param_type.min_size)
        self.assertIsNone(member.param_type.max_size)
        self.assertIsInstance(member.param_type.element_type,
                              generator.Model.Integer)
        self.assertEqual(11, member.param_type.element_type.min_value)
        self.assertEqual(100, member.param_type.element_type.max_value)

        # Structure "struct2"

        self.assertIn("struct2", interface.structs)
        struct = interface.structs["struct2"]
        self.verify_base_item(item=struct,
                              name="struct2",
                              description=["Description of struct2"],
                              platform="struct2 platform")

        self.assertEqual(4, len(struct.members))

        self.assertIn("m1", struct.members)
        member = struct.members["m1"]
        self.verify_base_item(item=member, name="m1")
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.String)
        self.assertIsNone(member.param_type.max_length)

        self.assertIn("m2", struct.members)
        member = struct.members["m2"]
        self.verify_base_item(item=member, name="m2")
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Array)
        self.assertEqual(1, member.param_type.min_size)
        self.assertEqual(50, member.param_type.max_size)
        self.assertIsInstance(member.param_type.element_type,
                              generator.Model.String)
        self.assertEqual(100, member.param_type.element_type.max_length)

        self.assertIn("m3", struct.members)
        member = struct.members["m3"]
        self.verify_base_item(item=member, name="m3")
        self.assertTrue(member.is_mandatory)
        self.assertIs(member.param_type, interface.enums["enum1"])

        self.assertIn("m4", struct.members)
        member = struct.members["m4"]
        self.verify_base_item(item=member, name="m4")
        self.assertTrue(member.is_mandatory)
        self.assertIsInstance(member.param_type, generator.Model.Array)
        self.assertIsNone(member.param_type.min_size)
        self.assertEqual(10, member.param_type.max_size)
        self.assertIs(member.param_type.element_type,
                      interface.structs["struct1"])

        # Functions

        self.assertEqual(3, len(interface.functions))

        # Function request "Function1"

        self.assertIn(
            (interface.enums["FunctionID"].elements["Function1"],
             interface.enums["messageType"].elements["request"]),
            interface.functions)
        function = interface.functions[
            (interface.enums["FunctionID"].elements["Function1"],
             interface.enums["messageType"].elements["request"])]
        self.verify_base_item(
            item=function,
            name="Function1",
            description=["Description of request Function1"],
            todos=["Function1 request todo"])
        self.assertIs(function.function_id,
                      interface.enums["FunctionID"].elements["Function1"])
        self.assertIs(function.message_type,
                      interface.enums["messageType"].elements["request"])

        self.assertEqual(3, len(function.params))

        self.assertIn("param1", function.params)
        param = function.params["param1"]
        self.verify_base_item(
            item=param,
            name="param1",
            issues=[TestSDLRPCV1Parser._Issue(creator="", value="")])
        self.assertEqual(False, param.is_mandatory)
        self.assertIsInstance(param.param_type, generator.Model.String)
        self.assertIsNone(param.param_type.max_length)
        self.assertEqual("String default value", param.default_value)

        self.assertIn("param2", function.params)
        param = function.params["param2"]
        self.verify_base_item(
            item=param,
            name="param2",
            description=["Param2 description", ""],
            todos=["Param2 todo"],
            platform="param2 platform")
        self.assertTrue(param.is_mandatory)
        self.assertIsInstance(param.param_type, generator.Model.Integer)
        self.assertIsNone(param.param_type.min_value)
        self.assertIsNone(param.param_type.max_value)
        self.assertIsNone(param.default_value)

        self.assertIn("param3", function.params)
        param = function.params["param3"]
        self.verify_base_item(item=param, name="param3")
        self.assertEqual(False, param.is_mandatory)
        self.assertIs(param.param_type, interface.structs["struct1"])
        self.assertIsNone(param.default_value)

        # Function response "Function1"

        self.assertIn(
            (interface.enums["FunctionID"].elements["Function1"],
             interface.enums["messageType"].elements["response"]),
            interface.functions)
        function = interface.functions[
            (interface.enums["FunctionID"].elements["Function1"],
             interface.enums["messageType"].elements["response"])]
        self.verify_base_item(
            item=function,
            name="Function1",
            issues=[TestSDLRPCV1Parser._Issue(creator="c1", value=""),
                    TestSDLRPCV1Parser._Issue(creator="c2", value="")],
            platform="")
        self.assertIs(function.function_id,
                      interface.enums["FunctionID"].elements["Function1"])
        self.assertIs(function.message_type,
                      interface.enums["messageType"].elements["response"])

        self.assertEqual(3, len(function.params))

        self.assertIn("p1", function.params)
        param = function.params["p1"]
        self.verify_base_item(item=param, name="p1")
        self.assertTrue(param.is_mandatory)
        self.assertIs(param.param_type, interface.enums["enum1"])
        self.assertIsNone(param.default_value)

        self.assertIn("p2", function.params)
        param = function.params["p2"]
        self.verify_base_item(item=param, name="p2")
        self.assertTrue(param.is_mandatory)
        self.assertIs(param.param_type, interface.enums["enum1"])
        self.assertIs(param.default_value,
                      interface.enums["enum1"].elements["element2"])

        self.assertIn("p3", function.params)
        param = function.params["p3"]
        self.verify_base_item(item=param, name="p3", design_description=[""])
        self.assertTrue(param.is_mandatory)
        self.assertIsInstance(param.param_type, generator.Model.Boolean)
        self.assertEqual(False, param.default_value)

        # Function notification "Function2"

        self.assertIn(
            (interface.enums["FunctionID"].elements["Function2"],
             interface.enums["messageType"].elements["notification"]),
            interface.functions)
        function = interface.functions[
            (interface.enums["FunctionID"].elements["Function2"],
             interface.enums["messageType"].elements["notification"])]
        self.verify_base_item(item=function,
                              name="Function2",
                              description=["Function2 description"],
                              platform="function2 platform")
        self.assertIs(function.function_id,
                      interface.enums["FunctionID"].elements["Function2"])
        self.assertIs(function.message_type,
                      interface.enums["messageType"].elements["notification"])

        self.assertEqual(3, len(function.params))

        self.assertIn("n1", function.params)
        param = function.params["n1"]
        self.verify_base_item(item=param, name="n1", todos=["n1 todo"])
        self.assertTrue(param.is_mandatory)
        self.assertIsInstance(param.param_type, generator.Model.EnumSubset)
        self.assertIs(param.param_type.enum, interface.enums["enum1"])
        self.assertDictEqual(
            {"element2": interface.enums["enum1"].elements["element2"],
             "element3": interface.enums["enum1"].elements["element3"]},
            param.param_type.allowed_elements)
        self.assertIsNone(param.default_value)

        self.assertIn("n2", function.params)
        param = function.params["n2"]
        self.verify_base_item(item=param, name="n2", todos=["n2 todo"])
        self.assertTrue(param.is_mandatory)
        self.assertIsInstance(param.param_type, generator.Model.Array)
        self.assertEqual(1, param.param_type.min_size)
        self.assertEqual(100, param.param_type.max_size)
        self.assertIsInstance(param.param_type.element_type,
                              generator.Model.EnumSubset)
        self.assertIs(param.param_type.element_type.enum,
                      interface.enums["enum1"])
        self.assertDictEqual(
            {"element1": interface.enums["enum1"].elements["element1"],
             "element3": interface.enums["enum1"].elements["element3"]},
            param.param_type.element_type.allowed_elements)
        self.assertIsNone(param.default_value)

        self.assertIn("n3", function.params)
        param = function.params["n3"]
        self.verify_base_item(item=param, name="n3")
        self.assertEqual(False, param.is_mandatory)
        self.assertIs(param.param_type, interface.structs["struct2"])
        self.assertIsNone(param.default_value)

    def verify_base_item(self, item, name, description=None,
                         design_description=None, issues=None, todos=None,
                         platform=None):
        """Verify base interface item variables."""
        self.assertEqual(name, item.name)
        self.assertSequenceEqual(self.get_list(description), item.description)
        self.assertSequenceEqual(self.get_list(design_description),
                                 item.design_description)
        self.assertSequenceEqual(self.get_list(issues), item.issues)
        self.assertSequenceEqual(self.get_list(todos), item.todos)
        self.assertEqual(platform, item.platform)

    @staticmethod
    def get_list(list=None):
        """Return provided list or empty list if None is provided."""
        return list if list is not None else []

if __name__ == "__main__":
    unittest.main()
