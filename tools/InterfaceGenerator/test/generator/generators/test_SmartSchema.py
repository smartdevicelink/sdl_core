import sys
import os.path

projectRootDir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
print projectRootDir
sys.path.append(projectRootDir)

import unittest

from generator.generators.SmartSchema import SmartSchema
from generator import Model

expected_result_enum1 = """/**
 * @brief Enumeration Test Name.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 * Design Line2
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

class TestSmartSchema(unittest.TestCase):
    
    def test_generate_comment(self):
        description = ["Description Line1", "Description Line2"]
        design_description = ["Design Line1", "Design Line2"]
        issues = ["Issue1", "Issue2", "Issue3"]
        todos = ["Do1", "Do2"]
        
        smart_schema_generator = SmartSchema()
        
        enum = Model.Enum("Test Name", description, design_description, issues, todos)
        self.assertEqual(smart_schema_generator._generate_comment(enum),
                         expected_result_enum1, 
                         "Full comment for enum is invalid")

        enum_element1 = Model.EnumElement("Element with internal name",
                                          None, None, None, None, "InternalName", "10")
        print smart_schema_generator._generate_enum_element(enum_element1)
        print expected_result_enum_element1
        self.assertEqual(smart_schema_generator._generate_enum_element(enum_element1),
                         expected_result_enum_element1,
                         "Short commented enum element with internal name is invalid")

if __name__ == '__main__':
    unittest.main()
