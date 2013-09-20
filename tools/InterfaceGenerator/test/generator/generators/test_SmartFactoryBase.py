"""Test for SmartFactory generator base.

Verifies common helper functions and produced source code.

"""
import collections
import unittest

from generator.generators import SmartFactoryBase
from generator import Model


EXPECTED_RESULT_FULL_COMMENT = u"""/**
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

EXPECTED_RESULT_ENUM_ELEMENT1 = u"""/**
 * @brief InternalName.
 */
InternalName = 10"""

EXPECTED_RESULT_ENUM_ELEMENT2 = u"""/**
 * @brief NO_VALUE_ELEMENT.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 */
NO_VALUE_ELEMENT"""

EXPECTED_RESULT_ENUM_ELEMENTS1 = u"""/**
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

EXPECTED_RESULT_ENUM1 = u"""namespace Enum1 {
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

EXPECTED_RESULT_ENUM2 = u"""namespace E2 {
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

DESCRIPTION = [u"Description Line1", u"Description Line2"]

DESIGN_DESCRIPTION = [u"Design Line1"]

ISSUES = [Model.Issue(value=u"Issue1"),
          Model.Issue(value=u"Issue2"),
          Model.Issue(value=u"Issue3")]

TODOS = [u"Do1", u"Do2"]


class Test(unittest.TestCase):

    """Test for SmartFactory base generator.

    This class holds set of test cases for the SmartFactory base generator.

    """

    def test_gen_comment(self):
        """Test generation of the source code comments.

        Verifies correct generation of the code comments.

        """
        generator = SmartFactoryBase.CodeGenerator()

        enum = Model.Enum(name=u"Test Name",
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
        generator = SmartFactoryBase.CodeGenerator()

        enum_element1 = Model.EnumElement(name=u"Element1",
                                          internal_name=u"InternalName",
                                          value=u"10")

        self.assertEqual(
            generator._gen_enum_element(enum_element1),
            EXPECTED_RESULT_ENUM_ELEMENT1,
            "Short commented enum element with internal name is invalid")

        enum_element2 = Model.EnumElement(
            name=u"NO_VALUE_ELEMENT",
            description=DESCRIPTION,
            design_description=DESIGN_DESCRIPTION)
        self.assertEqual(generator._gen_enum_element(enum_element2),
                         EXPECTED_RESULT_ENUM_ELEMENT2,
                         "Enum element with no value is invalid")

    def test_gen_enum_elements(self):
        """Test generation of the enum elements.

        Verifies correct generation of the enum elements.

        """
        generator = SmartFactoryBase.CodeGenerator()

        elements = [Model.EnumElement(name=u"name1",
                                      design_description=DESIGN_DESCRIPTION,
                                      todos=TODOS,
                                      value=u"1"),
                    Model.EnumElement(name=u"name2",
                                      description=DESCRIPTION,
                                      issues=ISSUES,
                                      internal_name=u"internal_name2")]
        self.assertEqual(generator._gen_enum_elements(elements),
                         EXPECTED_RESULT_ENUM_ELEMENTS1,
                         "Simple enum elements are invalid")

    def test_gen_enum(self):
        """Test generation of the enum.

        Verifies correct generation of the enum.

        """
        generator = SmartFactoryBase.CodeGenerator()

        elements1 = collections.OrderedDict()
        elements1[u"name1"] = Model.EnumElement(
            name=u"name1",
            design_description=DESIGN_DESCRIPTION,
            todos=TODOS,
            value=u"1")
        elements1[u"name2"] = Model.EnumElement(
            name=u"name2",
            description=DESCRIPTION,
            issues=ISSUES,
            internal_name=u"internal_name2")

        enum1 = Model.Enum(name=u"Enum1",
                           todos=TODOS,
                           elements=elements1)

        self.assertEqual(generator._gen_enum(enum1),
                         EXPECTED_RESULT_ENUM1,
                         "Simple enum is invalid")

        elements2 = collections.OrderedDict()
        elements2[u"xxx"] = Model.EnumElement(name=u"xxx",
                                              internal_name=u"val_1")
        elements2[u"yyy"] = Model.EnumElement(name=u"yyy",
                                              internal_name=u"val_2",
                                              value=u"100")
        elements2[u"zzz"] = Model.EnumElement(name=u"val_3")

        enum2 = Model.Enum(name=u"E2",
                           elements=elements2)
        self.assertEqual(generator._gen_enum(enum2),
                         EXPECTED_RESULT_ENUM2,
                         "Long enum is invalid")

        self.assertEqual(generator._gen_enums([enum1, enum2],
                                              collections.OrderedDict()),
                         u"{0}\n{1}".format(EXPECTED_RESULT_ENUM1,
                                            EXPECTED_RESULT_ENUM2),
                         "Generated enums are invalid")

    def test_normalize_multiline_comments(self):
        """Test normalization of the multiline comments.

        Verifies correct normalization of the multiline comments.

        """
        generator = SmartFactoryBase.CodeGenerator()

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
