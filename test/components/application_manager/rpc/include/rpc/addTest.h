/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADDTEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADDTEST_H_

#include "rpc/ICheckClass.h"
#include "rpc/GMockClassContainer.h"
#include "rpc/noParamsFord.h"
#include "rpc/unaryFord.h"
#include "rpc/binaryFord.h"


/**
* @brief Add test to Gtest
*
* @param first - test case name, second - test name, third - functor, fourth - expected return value functor
* 
* @return void
*/
template<typename NoParamsFunc, typename ReturnValues>
void addTest(const std::string &testCaseName, const std::string &testName, NoParamsFunc func, ReturnValues ret)
{
	ICheckClass *test = new noParamsFuncFord<NoParamsFunc, ReturnValues>(testCaseName, testName, func, ret);
	GMockClassContainer *container  = GMockClassContainer::instance();
	
	container->addTest(test);
}

/**
* @brief Add test to Gtest
*
* @param first - test case name, second - test name, third - Unfunctor, fourth - first argument Unfanc,
* fifth - expected return value Unfunctor
* 
* @return void
*/
template <typename UnFunc, typename FirstArgument, typename ReturnValues>
void addTest(const std::string &testCaseName, const std::string testName,
			 UnFunc func, FirstArgument first, ReturnValues ret)
{
	ICheckClass *test = new unaryFuncFord<UnFunc,FirstArgument,ReturnValues>(testCaseName, testName, func, first, ret);
	GMockClassContainer *container  = GMockClassContainer::instance();
	
	container->addTest(test);
}


/**
* @brief Add test to Gtest
*
* @param first - test case name, second - test name, third - Binfunctor, fourth - first argument Binfanc,
* fifth - second argument Binfanc, the 6th - expected return value Binfanc
*
* @return void
*/
template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
void addTest(const std::string &testCaseName, const std::string testName,
			 BinFunc func, FirstArgument first, SecondArgument second, ReturnValues ret)
{
	ICheckClass *test = new binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>(testCaseName, testName, func, first, second, ret);
	GMockClassContainer *container  = GMockClassContainer::instance();
	
	container->addTest(test);
}



#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADDTEST_H_





