#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include "eosio.system_tester.hpp"

#include "Runtime/Runtime.h"

#include <fc/variant_object.hpp>

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;
using namespace std;

using mvo = fc::mutable_variant_object;

class bos_pegtoken_tester : public tester
{
 public:
   bos_pegtoken_tester()
   {
      produce_blocks(2);

      create_accounts({N(alice), N(bob), N(eosio.token), N(btc.bos), N(issuer.bank), N(auditor.bank), N(gather.bank), N(teller.bank), N(manager.bank)});
      produce_blocks(2);

      set_code(N(btc.bos), contracts::pegtoken_wasm());
      set_abi(N(btc.bos), contracts::pegtoken_abi().data());

      produce_blocks();

      const auto &accnt = control->db().get<account_object, by_name>(N(btc.bos));
      abi_def abi;
      BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
      abi_ser.set_abi(abi, abi_serializer_max_time);
   }

   action_result push_action(const account_name &signer, const action_name &name, const variant_object &data)
   {
      string action_type_name = abi_ser.get_action_type(name);

      action act;
      act.account = N(btc.bos);
      act.name = name;
      act.data = abi_ser.variant_to_binary(action_type_name, data, abi_serializer_max_time);

      return base_tester::push_action(std::move(act), uint64_t(signer));
   }

   fc::variant get_auditor(const name aud, const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      //
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(auditors), aud.value);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("auditor_ts", data, abi_serializer_max_time);
   }

   fc::variant get_gatherer(const name gat, const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      //
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(gatherers), gat.value);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("gatherer_ts", data, abi_serializer_max_time);
   }

   fc::variant get_teller(const name tel, const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      //
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(tellers), tel.value);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("teller_ts", data, abi_serializer_max_time);
   }

   fc::variant get_manager(const name mgr, const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      //
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(managers), mgr.value);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("manager_ts", data, abi_serializer_max_time);
   }

   fc::variant get_symbols(const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      //
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(symbols), symbol_code);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("symbol_ts", data, abi_serializer_max_time);
   }

   fc::variant get_infos(const string &symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(infos), symbol_code);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("info_ts", data, abi_serializer_max_time);
   }

   name get_issuer(const string &symbolname)
   {
      return get_infos(symbolname)["issuer"].as<name>();
      // return data.empty() ? fc::variant() : abi_ser.binary_to_variant("info_ts", data, abi_serializer_max_time);
   }

   fc::variant get_issuer_mvo(const string &symbolname)
   {
      // return get_infos(symbolname)["issuer"];
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account(N(btc.bos), symbol_code, N(infos), symbol_code);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("info_ts", data, abi_serializer_max_time)["issuer"];
   }

   fc::variant get_addrs(const name act, symbol_code sym)
   {
      vector<char> data = get_row_by_account(N(btc.bos), act, N(addrs), act.value);
      return abi_ser.binary_to_variant("personal", data, abi_serializer_max_time);
   }

   fc::variant get_statistics(const name act, symbol_code sym)
   {
      vector<char> data = get_row_by_account(N(btc.bos), act, N(statistics), act.value);
      return abi_ser.binary_to_variant("personal", data, abi_serializer_max_time);
   }

   action_result create(symbol sym, name issuer, name address_style)
   {
      return push_action(N(btc.bos), N(create), mvo()("sym", sym)("issuer", issuer)("address_style", address_style));
   }

   action_result setissuer(symbol_code sym_code, name issuer)
   {
      return push_action(N(btc.bos), N(setissuer), mvo()("sym_code", sym_code)("issuer", issuer));
   }

   abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(bos_pegtoken_tests)

BOOST_FIXTURE_TEST_CASE(create_tests, bos_pegtoken_tester)
try
{
   // wrong style
   BOOST_REQUIRE_EQUAL(wasm_assert_msg("address_style must be one of bitcoin, ethereum, tether or other"),
                       create(symbol(SY(8, BTC)), N(btc.bos), "fdsf"));
   produce_blocks(1);
   // right test
   auto token = create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   auto syms = get_symbols("8,BTC");
   REQUIRE_MATCHING_OBJECT(syms, mvo()("sym", symbol(SY(8, BTC))));
   produce_blocks(1);
   // existing token
   BOOST_REQUIRE_EQUAL(wasm_assert_msg("token with symbol already exists in symbols table"),
                       create(symbol(SY(7, BTC)), N(btc.bos), "bitcoin"));
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(set_issuer_test, bos_pegtoken_tester)
try
{
   // set correct create_test
   auto token = create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   auto syms = get_symbols("8,BTC");
   REQUIRE_MATCHING_OBJECT(syms, mvo()("sym", symbol(SY(8, BTC))));
   produce_blocks(1);

   BOOST_REQUIRE_EQUAL(wasm_assert_msg("invalid account issuer"),
                       setissuer(symbol(SY(8, BTC)).to_symbol_code(), N(jacksonsmith)));

   // set correct issuer
   // setissuer(symbol(SY(8, BTC)).to_symbol_code(), N(issuer.bank));
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setissuer), mvo()("sym_code", "BTC")("issuer", "issuer.bank")));
   // 如何对比table中的一个元素
   // name issr = get_issuer("8,BTC");
   // REQUIRE_MATCHING_OBJECT(issr, N(issuer.bank));

   auto issr_mvo = get_issuer_mvo("8,BTC");
   // print("...");
   // REQUIRE_MATCHING_OBJECT(issr_mvo, "issuer.bank");
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(setauditor_test, bos_pegtoken_tester)
try
{
   // set correct create_test
   auto token = create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   auto syms = get_symbols("8,BTC");
   REQUIRE_MATCHING_OBJECT(syms, mvo()("sym", symbol(SY(8, BTC))));
   produce_blocks(2);
   //only owner can call 2
   BOOST_REQUIRE_EQUAL(error("missing authority of btc.bos"),
                       push_action(N(bob), N(setauditor), mvo()("sym_code", "BTC")("actn", "add")("auditor", "auditor.bank")));
   //setpersonal should success,and check result
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setauditor), mvo()("sym_code", "BTC")("actn", "add")("auditor", "auditor.bank")));
   auto aud = get_auditor("auditor.bank", "8,BTC");
   REQUIRE_MATCHING_OBJECT(aud, mvo()("auditor", "auditor.bank"));
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(setgatherer_test, bos_pegtoken_tester)
try
{
   create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   BOOST_REQUIRE_EQUAL(error("missing authority of btc.bos"),
                       push_action(N(bob), N(setgatherer), mvo()("sym_code", "BTC")("gatherer", "gather.bank")));
   produce_blocks(2);
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setgatherer), mvo()("sym_code", "BTC")("gatherer", "gather.bank")));
   auto gat = get_gatherer(N("gather.bank"), "8,BTC");
   //FIXME
   REQUIRE_MATCHING_OBJECT(gat, mvo()("gatherer", "gather.bank"));
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(setteller_test, bos_pegtoken_tester)
try
{
   create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   BOOST_REQUIRE_EQUAL(error("missing authority of btc.bos"),
                       push_action(N(bob), N(setteller), mvo()("sym_code", "BTC")("teller", "teller.bank")));
   produce_blocks(2);
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setteller), mvo()("sym_code", "BTC")("teller", "teller.bank")));
   auto tel = get_teller("teller.bank", "8,BTC");
   REQUIRE_MATCHING_OBJECT(tel, mvo()("teller", "teller.bank"));
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(setmanager_test, bos_pegtoken_tester)
try
{
   create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   BOOST_REQUIRE_EQUAL(error("missing authority of btc.bos"),
                       push_action(N(bob), N(setmanager), mvo()("sym_code", "BTC")("manager", "manager.bank")));

   BOOST_REQUIRE_EQUAL(wasm_assert_msg("invalid account manager"),
                       push_action(N(btc.bos), N(setmanager), mvo()("sym_code", "BTC")("manager", "ff.bank")));
   produce_blocks(2);
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setmanager), mvo()("sym_code", "BTC")("manager", "manager.bank")));
   auto mgr = get_manager("manager.bank", "8,BTC");
   REQUIRE_MATCHING_OBJECT(mgr, mvo()("manager", "manager.bank"));
}
FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(init_test, bos_pegtoken_tester)
try
{
   create(symbol(SY(8, BTC)), N(btc.bos), "bitcoin");
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(btc.bos), N(setissuer), mvo()("sym_code", "BTC")("issuer", "issuer.bank")));
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(issuer.bank), N(setauditor), mvo()("sym_code", "BTC")("actn", "add")("auditor", "auditor.bank")));
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(issuer.bank), N(setgatherer), mvo()("sym_code", "BTC")("gatherer", "gather.bank")));
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(issuer.bank), N(setteller), mvo()("sym_code", "BTC")("teller", "teller.bank")));
   BOOST_REQUIRE_EQUAL(success(),
                       push_action(N(issuer.bank), N(setmanager), mvo()("sym_code", "BTC")("manager", "manager.bank")));
}
FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()