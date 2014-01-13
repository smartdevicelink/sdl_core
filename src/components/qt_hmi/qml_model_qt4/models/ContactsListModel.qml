/**
 * @file ContactsListModel.qml
 * @brief List of contacts.
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
import QtQuick 1.1

ListModel
{
    // TODO {ALeshin}: This list model should be sorted in ABC and Upper/lower case order,
    // when it will be filled dynamically.
    ListElement { name: "Aaron" ; phone:"(614)555-2444";type:"Mobile"}
    ListElement { name: "Adrian" ; phone:"(740)245-3434";type:"Mobile" }
    ListElement { name: "Albert" ; phone:"(614)245-5584";type:"Mobile" }
    ListElement { name: "Alice" ; phone:"(313)878-3566";type:"Office"}
    ListElement { name: "Alec" ; phone:"(917)608-2418";type:"Mobile"}
    ListElement { name: "Alexander" ; phone:"(614)608-2400";type:"Home"}
    ListElement { name: "Alexander Vasil'evich Michail Leontiy" ; phone:"(614)608-2400";type:"Mobile"}
    ListElement { name: "Amber" ; phone:"(614)608-2111";type:"Mobile"}
    ListElement { name: "Amy" ; phone:"(440)245-3434";type:"Mobile" }
    ListElement { name: "Andy" ; phone:"(313)245-3434";type:"Mobile" }
    ListElement { name: "Antje" ; phone:"(567)245-3434";type:"Home" }
    ListElement { name: "buddy" ; phone:"(614)813-6610";type:"Mobile" }
    ListElement { name: "Barry" ; phone:"(313)555-2211";type:"Mobile" }
    ListElement { name: "Bea" ; phone:"(614)455-3434";type:"Mobile" }
    ListElement { name: "Ben" ; phone:"(614)882-1843";type:"Office" }
    ListElement { name: "Blake" ; phone:"(330)149-7778";type:"Mobile" }
    ListElement { name: "Bob Mould" ; phone:"(313)245-3434";type:"Mobile" }
    ListElement { name: "Brian" ; phone:"(614)332-7499";type:"Mobile" }
    ListElement { name: "Calvin" ; phone:"(614)442-9974";type:"Mobile" }
    ListElement { name: "Carl" ; phone:"(567)773-3434";type:"Home" }
    ListElement { name: "Cat Power" ; phone:"(313)608-3434";type:"Home"}
    ListElement { name: "Cathy" ; phone:"(313)444-456";type:"Home"}
    ListElement { name: "Chris" ; phone:"(734)997-6514";type:"Home" }
    ListElement { name: "Cloud Nothings"; phone:"(444)608-543";type:"Mobile"}
    ListElement { name: "Cody" ; phone:"(567)365-8413";type:"Home" }
    ListElement { name: "Curtis" ; phone:"(614)245-1547";type:"Office" }
    ListElement { name: "Dakota" ; phone:"(567)555-4444";type:"Home" }
    ListElement { name: "Dale" ; phone:"(567)723-1974";type:"Mobile" }
    ListElement { name: "Dallas"; phone:"(614)556-879";type:"Home"}
    ListElement { name: "Damian" ; phone:"(614)245-3434";type:"Office" }
    ListElement { name: "Daniel"; phone:"(614)556-879";type:"Home"}
    ListElement { name: "Darina"; phone:"(313)895-888";type:"Mobile"}
    ListElement { name: "Dee "; phone:"(313)432-3434";type:"Home"}
//    ListElement { name: "Edward" ; phone:"(917)608-975";type:"Office"}
    ListElement { name: "Ferdinand" ; phone:"(917)879-795";type:"Mobile"}
    ListElement { name: "gomez" ; phone:"(313)455-543";type:"Mobile"}
    ListElement { name: "Harry Potter"; phone:"(313)888-432";type:"Home"}
    ListElement { name: "Ivan"; phone:"(313)996-225";type:"Home"}
//    ListElement { name: "James" ; phone:"(917)608-3434";type:"Mobile"}
    ListElement { name: "Karen"; phone:"(313)998-332";type:"Home"}
    ListElement { name: "Luis Torres"; phone:"(313)779-111";type:"Home"}
    ListElement { name: "Mary"; phone:"(313)234-098";type:"Mobile"}
    ListElement { name: "Neil"; phone:"(313)444-112";type:"Mobile"}
    ListElement { name: "Oswald"; phone:"(313)456-921";type:"Mobile"}
    ListElement { name: "Oscar"; phone:"(313)332-778";type:"Home"}
    ListElement { name: "~1123"; phone:"(313)1332";type:"Home"}
    ListElement { name: "Patrice"; phone:"(313)789-111";type:"Mobile"}
    ListElement { name: "Queen Elizabeth"; phone:"(313)897-666";type:"Mobile"}
    ListElement { name: "Rose"; phone:"(313)111-211";type:"Mobile"}
    ListElement { name: "Susell"; phone:"(313)345-122";type:"Home"}
    ListElement { name: "Thomas"; phone:"(917)608-342";type:"Mobile"}
    ListElement { name: "Titus Andronicus" ; phone:"(917)608-321";type:"Office"}
    ListElement { name: "Toretto" ; phone:"(917)608-3434";type:"Home"}
    ListElement { name: "Uhl" ; phone:"(313)444-654";type:"Office"}
    ListElement { name: "willi" ; phone:"(313)224-5712";type:"Office"}
    ListElement { name: "Wanda" ; phone:"(313)666-6969";type:"Office"}
    ListElement { name: "wanda" ; phone:"(313)666-6969";type:"Office"}
    ListElement { name: "Zara" ; phone:"(313)231-009";type:"Mobile"}
}
