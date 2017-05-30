//
//  OrderBook.hpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/23.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#ifndef OrderBook_hpp
#define OrderBook_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

// Note: -1 => None price
struct Strike {
    double m_price; //   e.g. 9000
    double m_bid;   //   e.g. 100
    double m_ask;   //   e.g. 101
    Strike(double price=-1, double bid=-1, double ask=-1):
    m_price(price),m_bid(bid),m_ask(ask){}
};


struct OptionType{
    enum Type{
        BUY =0,
        SELL =1,
    };
};


typedef std::map<double,Strike*> ordermap;
typedef std::pair<double,Strike*> orderpair;
struct OrderBook{
    ordermap m_buy;
    ordermap m_sell;
    ~OrderBook();
    
    // addStrike // if strike exist then update it
    void addStrike(OptionType::Type type, Strike* strike,bool update);
    //void updateStrike(OptionType::Type type, Strike* strike);
    
    void dump();
};

#endif /* OrderBook_hpp */
