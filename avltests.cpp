#include "catch.hpp"
#include "dstree.h"
#include <iostream>
#include <string>
using namespace std;
/**
 * @brief TEST_CASE : these are the catch tests for the AVLtree
 * they test all the methods the same way as the hash table
 */
TEST_CASE("AVLtree ","[dstree]") {
    dstree<string,int> test1;
    dstree<string,vector<int>> test2;
    test2.insert("test");
    test2.insert("test1");
    SECTION("[] Operator") {
        for(int i =0; i < 50; i ++) {
            test2["test1"].push_back(i);
        }
        auto iter = test2["test1"].begin();
        REQUIRE(*iter == 0);
        iter +=2;
        REQUIRE(*iter == 2);
        test1.insert("he");
        test1["he"] = 50;
        test1.insert("het");
        test1["het"] = 100000;
        REQUIRE(test1["he"] == 50);
        REQUIRE(test1["het"] == 100000);
    }
    dstree<int,string> tst;
    for(int i =0; i < 50; i ++) {
        tst.insert(i);
        tst[i] = "test";
    }
    SECTION("COPY CONSTRUCTOR") {
        dstree<int,string> tok(tst);
        REQUIRE(tok[0] == "test");
        REQUIRE(tok[49] == "test");
        dstree<string,vector<int>> tst2(test2);
        REQUIRE(tst2.contains("test"));
        tst2["test"].push_back(0);
        tst2["test"].push_back(1);
        auto iter = tst2["test"].begin();
        REQUIRE(*iter == 0);
        iter++;
        REQUIRE(*iter == 1);
    }
    SECTION("= OPERATOR") {
        dstree<int,string> tok;
        tok = tst;
        REQUIRE(tok[0] == "test");
        REQUIRE(tok[0] == "test");
        dstree<string,vector<int>> ref = test2;
        ref["test1"].push_back(45);
        ref["test1"].push_back(33);
        auto iter = ref["test1"].begin();
        REQUIRE(*iter == 45);
        iter++;
        REQUIRE(* iter == 33);
    }
    SECTION("CONTAINS") {
        REQUIRE(test2.contains("test"));
        REQUIRE(!test1.contains("test"));
        tst.remove(2);
        REQUIRE(!tst.contains(2));
        REQUIRE(tst.contains(0));
    }
    SECTION("IS_EMPTY") {
        REQUIRE(test1.isEmpty());
        REQUIRE(!test2.isEmpty());
        dstree<int,int> temp;
        REQUIRE(temp.isEmpty());
        dstree<int,dstree<int,int>> top;
        top.insert(2);
        REQUIRE(top[2].isEmpty());
        top[2].insert(0);
        REQUIRE(!top[2].isEmpty());
    }
    SECTION("MAKE_EMPTY"){
        test2.makeEmpty();
        REQUIRE(test2.isEmpty());
        dstree<int,dstree<int,int>> top;
        top.insert(5);
        top[5].insert(40);
        top[5].makeEmpty();
        REQUIRE(top[5].isEmpty());
        top.makeEmpty();
        REQUIRE(top.isEmpty());
        top.insert(4);
        REQUIRE(!top.isEmpty());
    }
    SECTION("INSERT") {
        dstree<string,int> po;
        po.insert("test");
        REQUIRE(po.contains("test"));
        po.insert("hwllo");
        REQUIRE(po.contains("hwllo"));
        dstree<double,int> po2;
        po2.insert(24.6);
        REQUIRE(po2.contains(24.6));
        po2.insert(-13.55555);
        REQUIRE(po2.contains(-13.55555));
    }
    SECTION("REMOVE") {
        tst.remove(0);
        REQUIRE(!tst.contains(0));
        tst.remove(44);
        REQUIRE(tst.contains(49));
        test2.remove("test1");
        REQUIRE(!test2.contains("test1"));
    }
}
