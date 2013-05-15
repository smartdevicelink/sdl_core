//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef APPPOLICY_H_
#define APPPOLICY_H_

#include <string>
#include "utils/logger.h"


namespace NsAppManager
{

    /**
     * \brief AppPolicy acts as application behaviour modifier according to some rule
     */
    class AppPolicy
    {
    public:

        /**
         * \brief Class constructor
         * \param policy text representation
         */
        AppPolicy( const std::string& policy );

        /**
         * \brief Default class destructor
         */
        ~AppPolicy( );

        /**
         * \brief comparison operator
         * \return comparison result
         */
        bool operator<(const AppPolicy& item2) const;

        /**
         * \brief gets application policy text representation
         * \return application policy test representation
         */
        const std::string& getPolicyHash() const;

    private:

        /**
         * \brief Default class copy constructor
         */
        AppPolicy( const AppPolicy& );

        const std::string mPolicy;
        static log4cxx::LoggerPtr logger_;
    };

} // namespace NsAppManager

#endif /* APPPOLICY_H_ */
