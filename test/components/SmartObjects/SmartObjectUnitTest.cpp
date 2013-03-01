
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"

namespace test { namespace components { namespace SmartObjects { namespace SmartObjectUnitTest {

    using namespace NsAppLink::NsSmartObjects;


    class TestHelper : public ::testing::Test
    {
    protected:

        void makeMapObject(CSmartObject &obj, const int size) const
        {
            char i_key[8], j_key[8], k_key[8], value[8];

            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    for (int k = 0; k < size; k++)
                    {
                        sprintf(i_key, "i_%d", i);
                        sprintf(j_key, "j_%d", j);
                        sprintf(k_key, "k_%d", k);
                        sprintf(value, "%d", i + j + k);
                        obj[i_key][j_key][k_key] = value;
                    }
        }

        void checkMapObject(CSmartObject &obj, const int size) const
        {
            char i_key[8], j_key[8], k_key[8], value[8];

            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    for (int k = 0; k < size; k++)
                    {
                        sprintf(i_key, "i_%d", i);
                        sprintf(j_key, "j_%d", j);
                        sprintf(k_key, "k_%d", k);
                        sprintf(value, "%d", i + j + k);

                        ASSERT_EQ(std::string(value), static_cast<std::string>(obj[i_key][j_key][k_key])) <<
                                "Wrong value in the map at [" << i_key << "][" << j_key << "][" << k_key << "]";
                    }
        }

       void makeArrayObject(CSmartObject &obj, int size, int base=0)
       {
           for (int i = 0; i < size; i++)
               for (int j = 0; j < size; j++)
                   for (int k = 0; k < size; k++)
                   {
                       obj[i][j][k] = base + i + j + k;
                   }
       }

       void checkArrayObject(CSmartObject &obj, int size, int base=0)
       {
           for (int i = 0; i < size; i++)
               for (int j = 0; j < size; j++)
                   for (int k = 0; k < size; k++)
                   {
                       ASSERT_EQ(base + i + j + k, static_cast<int>(obj[i][j][k])) <<
                               "Wrong value in the array at index: " << i << ", " << j << ", " << k;
                   }
       }
    };

    /*
     * Tests different types sequentially
     */
    TEST(BasicMixtedTypes, test_SmartObjectUnitTest)
    {
        CSmartObject obj;

        // TODO: Use invalid values
        ASSERT_EQ(0, static_cast<int>(obj)) << "Wrong cast to int just after construction";

        obj = 10;
        ASSERT_EQ(10, static_cast<int>(obj)) << "Wrong cast to int";

        obj = "some string";
        ASSERT_EQ("some string", static_cast<std::string>(obj)) << "Wrong cast to std::string";

        obj = false;
        ASSERT_FALSE(static_cast<bool>(obj)) << "Wrong cast to bool";

        obj = 'A';
        ASSERT_EQ('A', static_cast<char>(obj)) << "Wrong cast to char";

        obj = 3.14;
        ASSERT_EQ(3.14, static_cast<double>(obj)) << "Wrong cast to double";

        // array test
        for (int i = 0; i < 100; i++)
        {
            obj[i] = i;
            ASSERT_EQ(i, static_cast<int>(obj[i]));
        }

        // map test
        for (int i = 0; i < 100; i++)
        {
            char key[8];
            sprintf(key, "%d", i);
            obj[key] = i;
            ASSERT_EQ(i, static_cast<int>(obj[key]));
        }
    }

    // TODO: Figure out why the test fails
    TEST_F(TestHelper, BasicArrayTest)
    {
        CSmartObject obj;

        ASSERT_EQ(0, static_cast<int>(obj[0])) << "Wrong value at accessing non existent index";
        ASSERT_EQ(0, static_cast<int>(obj["non_existent_key"])) << "Wrong value at accessing non existent key";

        obj[0] = 1;
        ASSERT_EQ(1, static_cast<int>(obj[0])) << "Wrong value at 0 index";
        obj[1] = 2;
        ASSERT_EQ(2, static_cast<int>(obj[1])) << "Wrong value at 1 index";

        obj[0][0] = 3;
        obj[1][0] = 1;
        ASSERT_EQ(3, static_cast<int>(obj[0][0])) << "Wrong value at index 0, 0";

        obj[0][0][0] = 4;
        obj[0][1][0] = 5;   // FIXME: Segmentation fault
        ASSERT_EQ(4, static_cast<int>(obj[0][0][0])) << "Wrong value at index 0, 0, 0";

        const int size = 32;
        makeArrayObject(obj, size);

        checkArrayObject(obj, size);
    }


    TEST_F(TestHelper, BasicMapTest)
    {
        CSmartObject obj;

        ASSERT_EQ(0, static_cast<int>(obj["non_existent_key"])) << "Wrong value for non existent key";

        obj["abc"]["def"]["ghi"] = 5;
        ASSERT_EQ(5, static_cast<int>(obj["abc"]["def"]["ghi"])) << "Wrong value for triple map";

        obj["123"]["456"]["789"] = "string test";
        // FIXME: Test crashes if string is assigned
        ASSERT_EQ("string test", static_cast<std::string>(obj["123"]["456"]["789"])) << "Wrong value for triple map";

        const int size = 32;

        makeMapObject(obj, size);

        checkMapObject(obj, size);
    }

    TEST(ConstructorsTest, test_SmartObjectUnitTest)
    {
        CSmartObject objInt(5678);
        ASSERT_EQ(5678, static_cast<int>(objInt)) << "Wrong constructor with int param";

        char c_str[] = "test c_string";
        CSmartObject obj_c_str(c_str);
        ASSERT_EQ("test c_string", static_cast<std::string>(obj_c_str)) << "Wrong constructor with c_str param";

        CSmartObject obj_std_str(std::string("test std_string"));
        ASSERT_EQ(std::string("test std_string"), static_cast<std::string>(obj_std_str));

        CSmartObject obj_char('R');
        ASSERT_EQ('R', static_cast<char>(obj_char)) << "Wrong constructor with char param";

        CSmartObject obj_double(-0.4321);
        ASSERT_EQ(-0.4321, static_cast<double>(obj_double)) << "Wrong constructor with double param";

        CSmartObject obj_bool(true);
        ASSERT_TRUE(static_cast<bool>(obj_bool)) << "Wrong constructor with bool param";

        CSmartObject src_obj;

        src_obj["key_1"] = "value_1";     // FIXME: String assignment crashes test
        src_obj["key_2"]["sub_key_1"] = "value_2";

        CSmartObject dst_obj(src_obj);
        ASSERT_EQ("value_1", static_cast<std::string>(dst_obj["key_1"])) << "Copy constructor is not correct";
        ASSERT_EQ("value_2", static_cast<std::string>(dst_obj["key_2"]["sub_key_1"])) << "Copy constructor is not correct";
    }

    // TODO: Fix test agains conversion table
//     TEST(ConversionTest, test_SmartObjectUnitTest)
//     {
//         CSmartObject obj;
// 
//         ASSERT_EQ(0, static_cast<int>(obj)) << "Initial value is not 0";
//         ASSERT_FALSE(static_cast<bool>(obj)) << "Initial value is not false";
//         ASSERT_EQ("null", static_cast<std::string>(obj)) << "Initial value is not an empty string";
//         ASSERT_EQ('\0', static_cast<char>(obj)) << "Initial value of a char is not \\0";
//         ASSERT_EQ(0, static_cast<double>(obj)) << "Initial value of a double is not 0";
// 
//         obj = 1;
//         ASSERT_TRUE(static_cast<bool>(obj)) << "1 is not true";
//         ASSERT_EQ("1", static_cast<std::string>(obj)) << "string representation is not correct";
//         ASSERT_EQ('\0', static_cast<char>(obj)) << "char representation is not correct";
//         ASSERT_EQ(1, static_cast<double>(obj)) << "double representation is not correct";
// 
//         // FIXME: Conversion from string to int doesn't work
//         obj = "54321";
//         ASSERT_EQ(54321, static_cast<int>(obj)) << "String to int is not correct";
// 
//         obj = "-1234";
//         ASSERT_EQ(-1234, static_cast<int>(obj)) << "String to int is not correct";
// 
//         obj = "true";
//         ASSERT_TRUE(static_cast<bool>(obj)) << "String to bool is not correct";
// 
//         obj = "false";
//         ASSERT_FALSE(static_cast<bool>(obj)) << "String to bool is not correct";
// 
//         obj = "TRUE";
//         ASSERT_TRUE(static_cast<bool>(obj)) << "Uppercase TRUE is not recognised as true";
// 
//         obj = "-3.1234";
//         ASSERT_EQ(-3.1234, static_cast<double>(obj)) << "String to double is not correct";
// 
//         obj = "-43.43.5something";
//         ASSERT_EQ(-1, static_cast<double>(obj)) << "Wrong conversion invalid string to double";
//     }

    TEST_F(TestHelper, AssignmentTest)
    {
        CSmartObject objSrc, objDst;

        objSrc = -6;
        objDst = 7;
        objDst = objSrc;
        ASSERT_EQ(-6, static_cast<int>(objDst)) << "Wrong assignment for int object";

        objSrc = "Some test string";
        objDst = "Other string";
        objDst = objSrc;
        ASSERT_EQ("Some test string", static_cast<std::string>(objDst)) << "Wrong assignment for std::string object";

        objSrc = 0.5;
        objDst = 4;
        objDst = objSrc;
        ASSERT_EQ(0.5, static_cast<double>(objDst)) << "Wrong assignment for double object";

        objSrc = true;
        objDst = false;
        objDst = objSrc;
        ASSERT_TRUE(static_cast<bool>(objDst)) << "Wrong assignment for bool object";

        const int size = 32;
        makeMapObject(objSrc, size);
        objDst["a"]["b"] = 4;
        objDst = objSrc;
        checkMapObject(objDst, size);

        makeArrayObject(objSrc, size, 5);
        makeArrayObject(objDst, 23, 6);
        objDst = objSrc;
        checkArrayObject(objDst, size, 5);
    }

    TEST_F(TestHelper, SizeTest)
    {
        CSmartObject obj;

        ASSERT_EQ(0, obj.size()) << "Wrong size for the uninitialized object";

        obj = 1234;
        ASSERT_EQ(1, obj.size()) << "Wrong size for the int object";

        std::string str("Some test very long string");
        obj = str;
        ASSERT_EQ(str.size(), obj.size()) << "The size of the object containing string is not correct";

        obj = true;
        ASSERT_EQ(1, obj.size()) << "Wrong size of the true";

        obj = 0.1234;
        ASSERT_EQ(1, obj.size()) << "Wrong size of the double";

        obj = 'A';
        ASSERT_EQ(1, obj.size()) << "Wrong size of the char";

        makeMapObject(obj, 12);
        ASSERT_EQ(12, obj.size()) << "Wrong size of the object containing map";

        makeArrayObject(obj, 21);
        ASSERT_EQ(21, obj.size()) << "Wrong size of the object containing array";
    }

    class StressTestHelper : public ::testing::Test
    {
    private:
        std::vector<std::string>split(const std::string &s, char delim) const
        {
            std::vector<std::string> elems;

            std::stringstream ss(s);
            std::string item;
            while(std::getline(ss, item, delim))
            {
                elems.push_back(item);
            }

            return elems;
        }

        char get_random_char() const
        {
            return static_cast<char>('A' + (rand()%52));
        }

        std::string to_string(const int value) const
        {
            char buff[32];
            sprintf(buff, "%d", value);
            return std::string(buff);
        }

        std::string to_string(const double value) const
        {
            char buff[32];
            sprintf(buff, "%g", value);
            return std::string(buff);
        }

        std::string to_string(const char ch) const
        {
            char buff[2];
            sprintf(buff, "%c", ch);
            return std::string(buff);
        }

        std::string to_string(const bool b) const
        {
            return std::string( (b) ? "true" : "false");
        }

    protected:
        typedef std::map<std::string, std::string> VerificationMap;
        VerificationMap mVerifyMap;

        std::string generate_key(const char *pPrefix, const int index) const
        {
            char buff[32];
            sprintf(buff, "%s%d ", pPrefix, index);
            return std::string(buff);
        }

        void makeRandomObject(CSmartObject &obj, const int size, std::string key_path)
        {
            int type_id = rand() % 7;

            switch (type_id)
            {
            case 0:     // int
                {
                    int iVal = rand();
                    std::cout << iVal << std::endl;
                    obj = iVal;
                    mVerifyMap[key_path] = to_string(iVal);
                    break;
                }
            case 1:     // bool
                {
                    bool bVal = static_cast<bool>(rand()%2);
                    std::cout << bVal << std::endl;
                    obj = bVal;
                    mVerifyMap[key_path] = to_string(bVal);
                    break;
                }
            case 2:     // double
                {
                    double dVal = 100.0 / (rand()%200);
                    std::cout << dVal << std::endl;
                    obj = dVal;
                    mVerifyMap[key_path] = to_string(dVal);
                    break;
                }
            case 3:     // char
                {
                    char cVal = get_random_char();
                    std::cout << cVal << std::endl;
                    obj = cVal;
                    mVerifyMap[key_path] = to_string(cVal);
                    break;
                }
            case 4:     // string
                {
                    std::string strVal(rand()%200, get_random_char());
                    std::cout << strVal;
                    obj = strVal;   // string with random char filled random size
                    mVerifyMap[key_path] = strVal;
                    break;
                }
            case 5:     // map
                if (size <= 0)
                    break;

                std::cout << "map" << std::endl;

                mVerifyMap[key_path] = "map";
                for (int i = 0; i < 1; i++)
                {
                    std::string key = generate_key("M", i);
                    obj[key] = key;
                    //obj[key] = CSmartObject();
                    //makeRandomObject(obj[key], size - 1, key_path + key);     // recursion
                    std::cout << "created object: " << static_cast<std::string>(obj[key]) << " key: " << key_path + key << std::endl;
                }
                break;
            case 6:     // array
                if (size <= 0)
                    break;

                std::cout << "array" << std::endl;

                mVerifyMap[key_path] = "array";
                for (int i = 0; i < size; i++)
                {
                    obj[i] = CSmartObject();      // just init it as an array
                    makeRandomObject(obj[i], size - 1, key_path + generate_key("A", i));     // recursion
                }


                break;
            }
        }

        CSmartObject get_object(CSmartObject &rootObj, const std::string &path) const
        {
            std::vector<std::string> obj_tokens;
            CSmartObject lastObj = rootObj;

            obj_tokens = split(path, ' ');

            for (int i = 0; i < obj_tokens.size(); i++)
            {
                if (obj_tokens[i][0] == 'A')            // array
                {
                    int index = atoi(&(obj_tokens[i].c_str()[1]));      // get integer skipping first char
                    std::cout << " Array index=" << index;
                    //lastObj = lastObj[index];       // go to the child object
                    lastObj = CSmartObject(lastObj[index]);        // FIXME:  Workaround!!!
                }
                else if (obj_tokens[i][0] == 'M')       // map
                {
                    std::cout << " Map key=" << obj_tokens[i];
                    //lastObj = lastObj[obj_tokens[i]];       // go to the child object
                    lastObj = CSmartObject(lastObj[obj_tokens[i]]);     // FIXME: Workaround!!!
                }
                else
                {
                    //FAIL();
                    EXPECT_TRUE(false);
                }
            }
            return lastObj;
        }
    };

    /*
     * The test creates the initial CSmartObject and use it as an array for the next CSmartObjects.
     * Each next CSmartObject is randomly assigned to some type.
     * If one of the object happens to be a container it fills it with CSmartObject of random type. The amount of these
     * objects is the size of the parent container -1.
     * The iteration continues until all nodes are simple CSmartObjects (not arrays or maps)
     */
    TEST_F(StressTestHelper, StressTest)
    {
        CSmartObject objects;

        const int size = 16;

        for (int i = 0; i < size; i++)
        {
            CSmartObject obj;

            makeRandomObject(obj, size - 1, generate_key("A", i));

            objects[i] = obj;
        }


        for (VerificationMap::const_iterator it = mVerifyMap.begin(); it != mVerifyMap.end(); it++)
        {
            std::string value(it->second);

            if (value.compare("map") && value.compare("array"))
            {
                std::cout << "Key: " << it->first << " Val: " << value << std::endl;
                CSmartObject obj = get_object(objects, it->first);
                std::cout << " value: " << static_cast<std::string>(obj) << std::endl;

                ASSERT_EQ(value, static_cast<std::string>(obj)) << "Object value is not correct. Object path: " << it->first;
            }
        }
    }

   TEST(CopyObjectsTest, SmartObjectTest)
   {
       CSmartObject obj;

       obj[0] = "test string";

       obj = obj[0];

       ASSERT_EQ("test string", static_cast<std::string>(obj));
   }
   
    TEST(CopyConstructorTest, SmartObjectTest)
    {
        CSmartObject srcObj;
        
        srcObj[0] = "test string";
        
        CSmartObject dstObj = srcObj[0];
        
        ASSERT_EQ("test string", static_cast<std::string>(dstObj));
    }

    TEST_F(StressTestHelper, ExtraManualDebugTest)
    {
        CSmartObject obj;

        obj[0] = false;
        obj[1] = 0.869495;
        obj[2] = true;
        obj[3] = 'Q';
        obj[4] = true;
        obj[5] = 3.704;
        obj[6] = CSmartObject();
        obj[6][0] = std::string("ttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt");
        obj[6][1] = 'K';
        obj[6][2] = 0.735294;
        obj[6][3] = 'I';
        obj[6][4] = CSmartObject();
        obj[6][4]["M0"] = 0.59432;
        CSmartObject & refObj = obj[6][4];
        refObj["M1"]["M0"]["M0"][0] = true;

        ASSERT_EQ("0.59432", static_cast<std::string>(get_object(obj, "A6 A4 M0")));
        ASSERT_EQ("true", static_cast<std::string>(get_object(obj, "A6 A4 M1 M0 M0 A0")));
    }

    int main(int argc, char **argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

}}}}

