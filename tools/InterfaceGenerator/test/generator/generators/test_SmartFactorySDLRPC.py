import collections
import unittest

from generator.generators import SmartFactorySDLRPC
from generator import Model

expected_result_code = (
u"""params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);\n""" \
u"""params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);\n""" \
u"""params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);\n""" \
u"""params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);\n""" \
u"""params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);\n"""
)

description = [u"Description Line1", u"Description Line2"]

design_description = [u"Design Line1"]

issues = [Model.Issue(value=u"Issue1"),
          Model.Issue(value=u"Issue2"),
          Model.Issue(value=u"Issue3")]

todos = [u"Do1", u"Do2"]

class TestSmartSchema(unittest.TestCase):

    def test_gen_schema_params_fill(self):
        generator = SmartFactorySDLRPC.CodeGenerator()

        self.assertEqual(generator._gen_schema_params_fill("request"),
                         expected_result_code,
                         "Invalid code generation")

        self.assertEqual(generator._gen_schema_params_fill("response"),
                         expected_result_code,
                         "Invalid code generation")

        self.assertEqual(generator._gen_schema_params_fill("notification"),
                         expected_result_code,
                         "Invalid code generation")

        self.assertEqual(generator._gen_schema_params_fill("any text as input"),
                         expected_result_code,
                         "Invalid code generation")

    def test_full_generation(self):
        generator = SmartFactorySDLRPC.CodeGenerator()

        elements1 = collections.OrderedDict()
        elements1[u"name1"] = Model.EnumElement(name=u"name1",
                                               design_description=design_description,
                                               todos=todos,
                                               value=u"1")
        elements1[u"name2"] = Model.EnumElement(name="name2",
                                                description=description,
                                                issues=issues,
                                                internal_name=u"internal_name2")

        enum1 = Model.Enum(name=u"Enum1",
                           todos=todos,
                           elements=elements1)

        elements2 = collections.OrderedDict()
        elements2[u"xxx"] = Model.EnumElement(name=u"xxx",
                                             internal_name=u"val_1")
        elements2[u"yyy"] = Model.EnumElement(name=u"yyy",
                                             internal_name=u"val_2",
                                             value=u"100")
        elements2[u"zzz"] = Model.EnumElement(name=u"val_3")

        enum2 = Model.Enum(name=u"E2",
                           elements=elements2)

        elements3 = collections.OrderedDict()
        elements3["1"] = Model.EnumElement(name="xxx",
                                           internal_name="_1")
        elements3["2"] = Model.EnumElement(name="xxx",
                                           internal_name="_2")
        elements3["3"] = Model.EnumElement(name="xxx",
                                           internal_name="_3")
        enum3 = Model.Enum(name="Enum_new2",
                           elements=elements3)

        elements4 = collections.OrderedDict()
        elements4["name1"] = Model.EnumElement(name="xxx",
                                           internal_name="_11")
        elements4["name2"] = Model.EnumElement(name="xxx",
                                               internal_name="_22")
        enum4 = Model.Enum(name="Enum_new4",
                           elements=elements4)

        enums = collections.OrderedDict()
        enums["Enum1"] = enum1
        enums["Enum2"] = enum2
        enums["Enum3"] = enum3
        enums["Enum4"] = enum4

        params1 = collections.OrderedDict()
        params1["1"] = Model.FunctionParam(name="param1",
                                           design_description=design_description,
                                           description=description,
                                           issues=issues,
                                           todos=todos,
                                           param_type=enum4,
                                           default_value=elements4["name1"])
        params1["2"] = Model.FunctionParam(name="param2",
                                           param_type=Model.EnumSubset(name="sub1",
                                                                       enum=enum1,
                                                                       allowed_elements={"e1" : elements1["name1"]}),
                                           default_value=elements1["name1"])

        functions = collections.OrderedDict()
        functions["Function1"] = Model.Function(name="Function1",
                                                function_id=elements1["name1"],
                                                message_type=elements1["name2"],
                                                params=params1)
        functions["Function2"] = Model.Function(name="Function2",
                                                function_id=elements2["xxx"],
                                                message_type=elements2["yyy"])

        members1 = collections.OrderedDict()
        members1["m1"] = Model.Param(name="intParam",
                                     param_type=Model.Integer(max_value=2))
        members1["m11"] = Model.Param(name="doubleParam",
                                      param_type=Model.Double(min_value=0.333),
                                      is_mandatory=False)
        members1["m222"] = Model.Param(name="boolParam",
                                       param_type=Model.Boolean())
        members1["m2"] = Model.Param(name="structParam",
                                     param_type=Model.Struct(name="Struct2"))
        members1["aaa"] = Model.Param(name="enumParam",
                                      param_type=enum1)
        members1["bbb"] = Model.Param(name="enumParam1",
                                      param_type=enum1)
        members1["xxx"] = Model.Param(name="enumSubset1",
                                      param_type=Model.EnumSubset(name="sub",
                                                                  enum=enum1,
                                                                  allowed_elements={"e1" : elements1["name1"]}),
                                      is_mandatory=False)
        members1["1"] = Model.Param(name="arrayOfInt",
                                    param_type=Model.Array(min_size=0,
                                                           max_size=20,
                                                           element_type=Model.Boolean()),
                                    is_mandatory=False)
        members1["2"] = Model.Param(name="arrayOfEnum1",
                                    param_type=Model.Array(min_size=0,
                                                           max_size=20,
                                                           element_type=enum1),
                                    is_mandatory=False)
        members1["3"] = Model.Param(name="arrayOfEnum3",
                                    param_type=Model.Array(min_size=10,
                                                           max_size=40,
                                                           element_type=enum3),
                                    is_mandatory=True)
        members1["4"] = Model.Param(name="arrayOfEnum4",
                                    param_type=Model.Array(min_size=10,
                                                           max_size=41,
                                                           element_type=Model.EnumSubset(name="sub1",
                                                                                         enum=enum1,
                                                                                         allowed_elements={"e1" : elements1["name1"]})))
        members1["5"] = Model.Param(name="arrayOfEnum5",
                                    param_type=Model.Array(min_size=10,
                                                           max_size=42,
                                                           element_type=Model.EnumSubset(name="sub2",
                                                                                         enum=enum1,
                                                                                         allowed_elements={"e1" : elements1["name2"]})))
        members1["6"] = Model.Param(name="arrayOfEnum6",
                                    param_type=Model.Array(min_size=10,
                                                           max_size=43,
                                                           element_type=Model.EnumSubset(name="sub3",
                                                                                         enum=enum4,
                                                                                         allowed_elements={"e1" : elements4["name2"]})))

        structs = collections.OrderedDict()
        structs["Struct1"] = Model.Struct(name="Struct1",
                                          design_description=design_description,
                                          issues=issues,
                                          members=members1)
        structs["Struct2"] = Model.Struct(name="Struct2",
                                          issues=issues)

        interface = Model.Interface(enums=enums,
                                    structs=structs,
                                    functions=functions,
                                    params={"param1" : "value1",
                                            "param2" : "value2"})
        # Use this code to create test source code until this test is
        # not fully functional.
        # generator.generate(interface=interface,
        #                    filename="Test.xml",
        #                    namespace="XXX::YYY::ZZZ",
        #                    destination_dir="/home/eftin/gen_test")
