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


#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_UNARY_PREDICAT_FORD_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_UNARY_PREDICAT_FORD_H_

#include "rpc/ICheckClass.h"


/**
 * @brief Need onli AddTest
 */
template <typename UnFunc, typename FirstArgument, typename ReturnValues>
class unaryFuncFord: public ICheckClass
{
public:
	//methods
	/**
	 * @brief Constructor
	 *
	 * @param first - test case name, second - test name, third - Unfunctor, fourth - first argument Unfunctor,
	 * fifth - expected return value Unfunctor
	 *
	 */
	unaryFuncFord(const std::string &testCaseName, const std::string &testName, 
						UnFunc inFunc, FirstArgument first, ReturnValues ret);
	
	/**
	 * @brief Destructor
	 */					
	~unaryFuncFord();
	
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
	 * @param first - const ref unaryFuncFord
	 */
	unaryFuncFord(const unaryFuncFord &);
	
	/**
	 * @brief Operator assignment
	 *
	 * @param first - const ref unaryFuncFord
	 *
	 * @return point to unaryFuncFord
	 */
	unaryFuncFord &operator=(const unaryFuncFord &);
	
	//members
	
	/**
	 * @brief UnFunctor
	 */
	UnFunc func_;
	
	/**
	 * @brief FirstArgument
	 */
	FirstArgument first_;
	
	/**
	 * @brief ReturnValues
	 */
	ReturnValues ret_;
};



template <typename UnFunc, typename FirstArgument, typename ReturnValues>
unaryFuncFord<UnFunc,FirstArgument,ReturnValues>::unaryFuncFord(const std::string &testCaseName, const std::string &testName, 
														UnFunc inFunc, FirstArgument first, ReturnValues ret)
	:ICheckClass(testCaseName, testName)
	,func_(inFunc)
	,first_(first)
	,ret_(ret)					
{
}


template <typename UnFunc, typename FirstArgument, typename ReturnValues>
unaryFuncFord<UnFunc,FirstArgument,ReturnValues>::~unaryFuncFord()
{
}


template <typename UnFunc, typename FirstArgument, typename ReturnValues>
bool unaryFuncFord<UnFunc,FirstArgument,ReturnValues>::check()
{
	return (ret_ == func_(first_));
}


template <typename UnFunc, typename FirstArgument, typename ReturnValues>
unaryFuncFord<UnFunc,FirstArgument,ReturnValues>::unaryFuncFord(const unaryFuncFord &in)
	:ICheckClass(in.unitTestName, in.testName)
{
}


template <typename UnFunc, typename FirstArgument, typename ReturnValues>
unaryFuncFord<UnFunc,FirstArgument,ReturnValues> &unaryFuncFord<UnFunc,FirstArgument,ReturnValues>::operator=(const unaryFuncFord &)
{
	return *this;
}



#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_UNARY_PREDICAT_FORD_H_





