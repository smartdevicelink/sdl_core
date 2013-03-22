import collections
import sys
import os.path

projectRootDir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
sys.path.append(projectRootDir)

import unittest

from generator.generators.SmartSchema import SmartSchema
from generator import Model

expected_result_full_comment = """/**
 * @brief Enumeration Test Name.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 *
 * @note Issue1
 * @note Issue2
 * @note Issue3
 *
 * @todo Do1
 * @todo Do2
 */"""

expected_result_enum_element1 = """/**
 * @brief InternalName.
 */
InternalName = 10"""

expected_result_enum_element2 = """/**
 * @brief NO_VALUE_ELEMENT.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 */
NO_VALUE_ELEMENT"""

expected_result_enum_elements1 = """/**
 * @brief name1.
 *
 * Design Line1
 *
 * @todo Do1
 * @todo Do2
 */
name1 = 1,

/**
 * @brief internal_name2.
 *
 * Description Line1
 * Description Line2
 *
 * @note Issue1
 * @note Issue2
 * @note Issue3
 */
internal_name2"""

expected_result_enum1 = """namespace Enum1
{
    /**
     * @brief Enumeration Enum1.
     *
     * @todo Do1
     * @todo Do2
     */
    enum eType
    {
        /**
         * @brief INVALID_ENUM.
         */
        INVALID_ENUM = -1,

        /**
         * @brief name1.
         *
         * Design Line1
         *
         * @todo Do1
         * @todo Do2
         */
        name1 = 1,

        /**
         * @brief internal_name2.
         *
         * Description Line1
         * Description Line2
         *
         * @note Issue1
         * @note Issue2
         * @note Issue3
         */
        internal_name2
    };
}
"""

expected_result_enum2 = """namespace E2
{
    /**
     * @brief Enumeration E2.
     */
    enum eType
    {
        /**
         * @brief INVALID_ENUM.
         */
        INVALID_ENUM = -1,

        /**
         * @brief val_1.
         */
        val_1,

        /**
         * @brief val_2.
         */
        val_2 = 100,

        /**
         * @brief val_3.
         */
        val_3
    };
}
"""

description = ["Description Line1", "Description Line2"]
design_description = ["Design Line1"]
issues = ["Issue1", "Issue2", "Issue3"]
todos = ["Do1", "Do2"]

class TestSmartSchema(unittest.TestCase):

    def test_gen_comment(self):
        generator = SmartSchema()

        enum = Model.Enum(name="Test Name",
                          description=description,
                          design_description=design_description,
                          issues=issues,
                          todos=todos)
        self.assertEqual(generator._gen_comment(enum),
                         expected_result_full_comment,
                         "Full comment for enum is invalid")

    def test_gen_enum_element(self):
        generator = SmartSchema()

        enum_element1 = Model.EnumElement(name="Element1",
                                          internal_name="InternalName",
                                          value="10")
        self.assertEqual(generator._gen_enum_element(enum_element1),
                         expected_result_enum_element1,
                         "Short commented enum element with internal name is invalid")

        enum_element2 = Model.EnumElement(name="NO_VALUE_ELEMENT",
                                          description=description,
                                          design_description=design_description)
        self.assertEqual(generator._gen_enum_element(enum_element2),
                        expected_result_enum_element2,
                        "Enum element with no value is invalid")

    def test_gen_enum_elements(self):
        generator = SmartSchema()

        elements = [Model.EnumElement(name="name1",
                                      design_description=design_description,
                                      todos=todos,
                                      value="1"),
                    Model.EnumElement(name="name2",
                                      description=description,
                                      issues=issues,
                                      internal_name="internal_name2")]
        self.assertEqual(generator._gen_enum_elements(elements),
                         expected_result_enum_elements1,
                         "Simple enum elements are invalid")

    def test_gen_enum(self):
        generator = SmartSchema()

        elements1 = collections.OrderedDict()
        elements1["name1"] = Model.EnumElement(name="name1",
                                               design_description=design_description,
                                               todos=todos,
                                               value="1")
        elements1["name2"] = Model.EnumElement(name="name2",
                                               description=description,
                                               issues=issues,
                                               internal_name="internal_name2")

        enum1 = Model.Enum(name="Enum1",
                           todos=todos,
                           elements=elements1)
        self.assertEqual(generator._gen_enum(enum1),
                         expected_result_enum1,
                         "Simple enum is invalid")

        elements2 = collections.OrderedDict()
        elements2["xxx"] = Model.EnumElement(name="xxx",
                                             internal_name="val_1")
        elements2["yyy"] = Model.EnumElement(name="yyy",
                                             internal_name="val_2",
                                             value="100")
        elements2["zzz"] = Model.EnumElement(name="val_3")

        enum2 = Model.Enum(name="E2",
                           elements=elements2)
        self.assertEqual(generator._gen_enum(enum2),
                         expected_result_enum2,
                         "Long enum is invalid")

        self.assertEqual(generator._gen_enums([enum1, enum2]),
                         "{0}\n{1}".format(expected_result_enum1, expected_result_enum2)
                         , "Generated enums are invalid")

    def test_full_generation(self):
        generator = SmartSchema()

        elements1 = collections.OrderedDict()
        elements1["name1"] = Model.EnumElement(name="name1",
                                               design_description=design_description,
                                               todos=todos,
                                               value="1")
        elements1["name2"] = Model.EnumElement(name="name2",
                                                description=description,
                                                issues=issues,
                                                internal_name="internal_name2")

        enum1 = Model.Enum(name="Enum1",
                           todos=todos,
                           elements=elements1)

        elements2 = collections.OrderedDict()
        elements2["xxx"] = Model.EnumElement(name="xxx",
                                             internal_name="val_1")
        elements2["yyy"] = Model.EnumElement(name="yyy",
                                             internal_name="val_2",
                                             value="100")
        elements2["zzz"] = Model.EnumElement(name="val_3")

        enum2 = Model.Enum(name="E2",
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
        elements4["1"] = Model.EnumElement(name="xxx",
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

        functions = collections.OrderedDict()
        functions["Function1"] = Model.Function(name="Function1",
                                                function_id=elements1["name1"],
                                                message_type=elements1["name2"])
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

        # generator.generate(interface=interface,
        #                   filename="Test.xml",
        #                   namespace="XXX::YYY::ZZZ",
        #                   destination_dir="/home/eftin/gen_test")

if __name__ == '__main__':
    unittest.main()
