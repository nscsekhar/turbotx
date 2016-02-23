//
//  test.cpp
//  TurboTx
//
//  Created by Surya Nimmagadda on 2/18/16.
//  Copyright © 2016 Surya Nimmagadda. All rights reserved.
//

#include "test.hpp"
#include "gtest/gtest.h"
#include "PacketIO.hpp"

class TurboTxTest : public testing::Test {
public:

    int x;
    int y;
    void SetUp ()
    {
        x = 10;
        y = 11;
    }
    
    
};

TEST(Basic, simple) {
    int x = 1;
    EXPECT_TRUE(x == 1);
}

TEST_F(TurboTxTest, simple_1) {
    EXPECT_TRUE(x == 10);
}

TEST_F(TurboTxTest, simple_2) {
    PacketIO *ptr;
    
    ptr = new PacketIO("vboxnet0", "vboxnet1");
    EXPECT_TRUE(ptr != nullptr);
}

