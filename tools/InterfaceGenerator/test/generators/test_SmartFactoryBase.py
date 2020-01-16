"""Test for SmartFactory generator base.

Verifies common helper functions and produced source code.

"""
import collections
import sys
import unittest
from pathlib import Path

sys.path.append(Path(__file__).absolute().parents[2].as_posix())
sys.path.append(Path(__file__).absolute().parents[3].joinpath('rpc_spec/InterfaceParser').as_posix())
try:
    from generators.SmartFactoryBase import SmartFactoryBase
    from model.enum import Enum
    from model.enum_element import EnumElement
    from model.issue import Issue
except ModuleNotFoundError as error:
    print('{}.\nProbably you did not initialize submodule'.format(error))
    sys.exit(1)

EXPECTED_RESULT_FULL_COMMENT = """/**
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

EXPECTED_RESULT_ENUM_ELEMENT1 = """/**
 * @brief InternalName.
 */
InternalName = 10"""

EXPECTED_RESULT_ENUM_ELEMENT2 = """/**
 * @brief NO_VALUE_ELEMENT.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 */
NO_VALUE_ELEMENT"""

EXPECTED_RESULT_ENUM_ELEMENTS1 = """/**
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

EXPECTED_RESULT_ENUM1 = """namespace Enum1 {
/**
 * @brief Enumeration Enum1.
 *
 * @todo Do1
 * @todo Do2
 */
enum eType {
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
} // Enum1
"""

EXPECTED_RESULT_ENUM2 = """namespace E2 {
/**
 * @brief Enumeration E2.
 */
enum eType {
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
} // E2
"""

DESCRIPTION = ["Description Line1", "Description Line2"]

DESIGN_DESCRIPTION = ["Design Line1"]

ISSUES = [Issue(value="Issue1"),
          Issue(value="Issue2"),
          Issue(value="Issue3")]

TODOS = ["Do1", "Do2"]


class Test(unittest.TestCase):
    """Test for SmartFactory base generator.

    This class holds set of test cases for the SmartFactory base generator.

    """

    def test_gen_comment(self):
        """Test generation of the source code comments.

        Verifies correct generation of the code comments.

        """
        generator = SmartFactoryBase()

        enum = Enum(name="Test Name",
                    description=DESCRIPTION,
                    design_description=DESIGN_DESCRIPTION,
                    issues=ISSUES,
                    todos=TODOS)
        self.assertEqual(generator._gen_comment(enum),
                         EXPECTED_RESULT_FULL_COMMENT,
                         "Full comment for enum is invalid")

    def test_gen_enum_element(self):
        """Test generation of the single enum element.

        Verifies correct generation of the single enum element.

        """
        generator = SmartFactoryBase()

        enum_element1 = EnumElement(name="Element1",
                                    internal_name="InternalName",
                                    value="10")

        self.assertEqual(
            generator._gen_enum_element(enum_element1),
            EXPECTED_RESULT_ENUM_ELEMENT1,
            "Short commented enum element with internal name is invalid")

        enum_element2 = EnumElement(
            name="NO_VALUE_ELEMENT",
            description=DESCRIPTION,
            design_description=DESIGN_DESCRIPTION)
        self.assertEqual(generator._gen_enum_element(enum_element2),
                         EXPECTED_RESULT_ENUM_ELEMENT2,
                         "Enum element with no value is invalid")

    def test_gen_enum_elements(self):
        """Test generation of the enum elements.

        Verifies correct generation of the enum elements.

        """
        generator = SmartFactoryBase()

        elements = [EnumElement(name="name1",
                                design_description=DESIGN_DESCRIPTION,
                                todos=TODOS,
                                value="1"),
                    EnumElement(name="name2",
                                description=DESCRIPTION,
                                issues=ISSUES,
                                internal_name="internal_name2")]
        self.assertEqual(generator._gen_enum_elements(elements),
                         EXPECTED_RESULT_ENUM_ELEMENTS1,
                         "Simple enum elements are invalid")

    def test_gen_enum(self):
        """Test generation of the enum.

        Verifies correct generation of the enum.

        """
        generator = SmartFactoryBase()

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

        self.assertEqual(generator._gen_enum(enum1),
                         EXPECTED_RESULT_ENUM1,
                         "Simple enum is invalid")

        elements2 = collections.OrderedDict()
        elements2["xxx"] = EnumElement(name="xxx",
                                       internal_name="val_1")
        elements2["yyy"] = EnumElement(name="yyy",
                                       internal_name="val_2",
                                       value="100")
        elements2["zzz"] = EnumElement(name="val_3")

        enum2 = Enum(name="E2",
                     elements=elements2)
        self.assertEqual(generator._gen_enum(enum2),
                         EXPECTED_RESULT_ENUM2,
                         "Long enum is invalid")

        self.assertEqual(generator._gen_enums([enum1, enum2],
                                              collections.OrderedDict()),
                         "{0}\n{1}".format(EXPECTED_RESULT_ENUM1,
                                           EXPECTED_RESULT_ENUM2),
                         "Generated enums are invalid")

    def test_normalize_multiline_comments(self):
        """Test normalization of the multiline comments.

        Verifies correct normalization of the multiline comments.

        """
        generator = SmartFactoryBase()

        self.assertEqual(generator._normalize_multiline_comments([]), [])

        self.assertEqual(generator._normalize_multiline_comments(["aaa",
                                                                  "bbb",
                                                                  "ccc ccc",
                                                                  "ddd\tddd"]),
                         ["aaa", "bbb", "ccc ccc", "ddd\tddd"])
        self.assertEqual(generator._normalize_multiline_comments(["aaa\n1",
                                                                  "bbb\r\n2",
                                                                  "ccc\r3",
                                                                  "aaa aaa"]),
                         ["aaa", "1", "bbb", "2", "ccc", "3", "aaa aaa"])


if __name__ == '__main__':
    unittest.main()
