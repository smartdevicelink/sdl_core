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


#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_PREDICAT_FORD_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_PREDICAT_FORD_H_

#include "rpc/ICheckClass.h"
/**
 * @brief Need onli AddTest
 */
template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
class binaryFuncFord: public ICheckClass
{
public:
	//methods
	/**
	* @brief Constructor
	*
	* @param first - test case name, second - test name, third - Binfunctor, fourth - first argument Binfanc,
	* fifth - second argument Binfanc, the 6th - expected return value Binfanc
	*
	*/
	binaryFuncFord(const std::string &testCaseName, const std::string &testName, 
						BinFunc func, FirstArgument first, SecondArgument second, ReturnValues ret);
	
	/**
	 * @brief Destructor
	 */
	~binaryFuncFord();
	
	/**
	 * @brief Calls Gtest func 
	 *
	 * @param
	 * 
	 * @return bool
	 */
	virtual bool check();
	
protected:

private:
	//methods
	/**
	 * @brief copy constructor
	 *
	 * @param first - const ref binaryFuncFord
	 */
	binaryFuncFord(const binaryFuncFord &);
	
	/**
	 * @brief Operator assignment
	 *
	 * @param first - const ref binaryFuncFord
	 *
	 * @return point to binaryFuncFord
	 */
	binaryFuncFord &operator=(const binaryFuncFord &);
	
	//members
	/**
	 * @brief BinFunc
	 */
	BinFunc func_;
	
	/**
	 * @brief FirstArgument
	 */
	FirstArgument first_;
	
	/**
	 * @brief SecondArgument
	 */
	SecondArgument second_;
	
	/**
	 * @brief ReturnValues
	 */
	ReturnValues ret_;
};


template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>::binaryFuncFord(const std::string &testCaseName, const std::string &testName, BinFunc func, FirstArgument first, SecondArgument second, ReturnValues ret)
	:ICheckClass(testCaseName, testName)
	,func_(func)
	,first_(first)
	,second_(second)
	,ret_(ret)
					
{
}


template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>::~binaryFuncFord()
{
}


template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
bool binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>::check()
{
	return (ret_ == func_(first_,second_));
}


template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>::binaryFuncFord(const binaryFuncFord &in)
					:ICheckClass(in.unitTestName, in.testName)
{
}


template <typename BinFunc, typename FirstArgument, typename SecondArgument, typename ReturnValues>
binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues> &
				binaryFuncFord<BinFunc,FirstArgument,SecondArgument,ReturnValues>::operator=(const binaryFuncFord &)
{
	return *this;
}




#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_BINARY_PREDICAT_FORD_H_





