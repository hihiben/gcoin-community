// Copyright (c) 2014-2016 The Gcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <stdint.h>

#include <map>
#include <string>
#include <iostream>

#include "test_util.h"
#include "policy/licenseinfo.h"

using namespace std;

BOOST_FIXTURE_TEST_SUITE(GlobalSetup, GlobalSetupFixture)

BOOST_FIXTURE_TEST_SUITE(CacheSetup, CacheSetupFixture)

struct NormalHandlerFixture : public GlobalSetupFixture, public CacheSetupFixture
{
    NormalHandlerFixture()
    {
        handler = type_transaction_handler::GetHandler(NORMAL);
    }
    ~NormalHandlerFixture()
    {
        handler = NULL;
    }
};

struct NormalHandlerCheckValidFixture : public NormalHandlerFixture
{
    NormalHandlerCheckValidFixture()
    {
        in_hash = ArithToUint256(arith_uint256(1));
        member_hash = ArithToUint256(arith_uint256(3));
        out_hash = ArithToUint256(arith_uint256(4));
        color = 0xFFFFFFFE;
        pinfo = new CLicenseInfo();
        pinfo->fMemberControl = true;
        plicense->SetOwner(color, CreateAddress(), pinfo);

        member = CreateAddress();
        receiver = CreateAddress();
        pactivate->Activate(color, receiver);
        CreateTransaction(in_hash, MINT);
        CreateTransaction(member_hash, NORMAL);
        CreateTransaction(out_hash, NORMAL);
        ConnectTransactions(in_hash, member_hash, COIN, member, color);
        ConnectTransactions(member_hash, out_hash, COIN, receiver, color);
    }

    ~NormalHandlerCheckValidFixture()
    {
        delete pinfo;
    }

    void CheckFalse(int ndos, const std::string &msg)
    {
        CValidationState state;
        bool ret;
        int v;

        ret = handler->CheckValid(
                CTransaction(transactions[member_hash]), state, NULL);
        ret &= handler->CheckFormat(
                CTransaction(transactions[member_hash]), state, NULL);

        BOOST_CHECK_MESSAGE(
                ret == false && state.IsInvalid(v) && v == ndos, msg);
    }

    uint256 in_hash, member_hash, out_hash;
    std::string member, receiver;
    type_Color color;
    CValidationState state;
    CLicenseInfo *pinfo;
};


BOOST_FIXTURE_TEST_CASE(NormalHandlerCheckValidPass, NormalHandlerCheckValidFixture)
{
    BOOST_CHECK(handler->CheckValid(
                CTransaction(transactions[member_hash]), state, NULL) == true);
}

BOOST_AUTO_TEST_SUITE_END(); // CacheSetup

BOOST_AUTO_TEST_SUITE_END(); // GlobalSetup
