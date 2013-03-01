
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"


#define NO_INCLUSIVE_MAPS
#define COPY_SUB_OBJECTS_WORKAROUND


namespace test { namespace components { namespace SmartObjects { namespace SmartObjectStressTest {

    using namespace NsAppLink::NsSmartObjects;

    class StressTestHelper : public ::testing::Test
    {
    private:
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
//            char buff[32];
//            sprintf(buff, "%g", value);
//            return std::string(buff);
            return std::to_string(value);   // TODO: decide which format to use, because the result is different
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
                    obj = iVal;
                    mVerifyMap[key_path] = to_string(iVal);
                    std::cout << "Created int, value: " << iVal << std::endl;
                    break;
                }
            case 1:     // bool
                {
                    bool bVal = static_cast<bool>(rand()%2);
                    obj = bVal;
                    mVerifyMap[key_path] = to_string(bVal);
                    std::cout << "Created bool, value: " << to_string(bVal) << std::endl;
                    break;
                }
            case 2:     // double
                {
                    double dVal = 100.0 / (rand()%200);
                    obj = dVal;
                    mVerifyMap[key_path] = to_string(dVal);
                    std::cout << "Created double, value: " << dVal << std::endl;
                    break;
                }
            case 3:     // char
                {
                    char cVal = get_random_char();
                    obj = cVal;
                    mVerifyMap[key_path] = to_string(cVal);
                    std::cout << "Created char, value: " << cVal << std::endl;
                    break;
                }
            case 4:     // string
                {
                    std::string strVal(rand()%200, get_random_char());
                    obj = strVal;   // string with random char filled random size
                    mVerifyMap[key_path] = strVal;
                    std::cout << "Created string, value: " << strVal << std::endl;
                    break;
                }
            case 5:     // map
                if (size <= 0)
                    break;

                std::cout << "Creating a map with size: " << size << std::endl;
                mVerifyMap[key_path] = "map";
                for (int i = 0; i < size; i++)
                {
                    std::string key = generate_key("M", i);
#ifdef NO_INCLUSIVE_MAPS
                    obj[key] = key;
#else
                    obj[key] = CSmartObject();
                    makeRandomObject(obj[key], size - 1, key_path + key);     // recursion
#endif // MAP_WORKAROUND
                }
                break;
            case 6:     // array
                if (size <= 0)
                    break;

                std::cout << "Creating an array with size: " << size << std::endl;
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
#ifdef COPY_SUB_OBJECTS_WORKAROUND
                    lastObj = CSmartObject(lastObj[index]);
#else
                    lastObj = lastObj[index];       // go to the child object
#endif
                }
                else if (obj_tokens[i][0] == 'M')       // map
                {
#ifdef COPY_SUB_OBJECTS_WORKAROUND
                    lastObj = CSmartObject(lastObj[obj_tokens[i]]);
#else
                    lastObj = lastObj[obj_tokens[i]];       // go to the child object
#endif
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

        const int size = 10;

        for (int i = 0; i < size; i++)
        {
            CSmartObject obj;

            makeRandomObject(obj, size - 1, generate_key("A", i));

            objects[i] = obj;
        }


        for (VerificationMap::const_iterator it = mVerifyMap.begin(); it != mVerifyMap.end(); it++)
        {
            std::string value(it->second);
            CSmartObject obj = get_object(objects, it->first);

#ifdef NO_INCLUSIVE_MAPS
            if (!value.compare("map"))
            {
                std::vector<std::string> path = split(it->first, ' ');

                std::string map_value = path[path.size()-1];
                ASSERT_EQ(map_value, static_cast<std::string>(obj));
            }
#endif
            if (value.compare("map") && value.compare("array"))
            {
                std::cout << "Verification key: " << it->first << " Value: " << value << std::endl;
                std::cout << "Object Value: " << static_cast<std::string>(obj) << std::endl;

                if (!value.compare("true"))
                {
                    ASSERT_TRUE(static_cast<bool>(obj));
                }
                else if (!value.compare("false"))
                {
                    ASSERT_FALSE(static_cast<bool>(obj));
                }
                else
                {
                    ASSERT_EQ(value, static_cast<std::string>(obj)) << "Object value is not correct. Object path: " << it->first;
                }
            }
        }
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

        // FIXME: Figure out why there's a trailing zero while converting from double to string
        ASSERT_EQ("0.594320", static_cast<std::string>(get_object(obj, "A6 A4 M0")));
        ASSERT_TRUE(static_cast<bool>(get_object(obj, "A6 A4 M1 M0 M0 A0")));
    }

    int main(int argc, char **argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

}}}}
