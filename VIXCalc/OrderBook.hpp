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
struct StrikeNode {
    double m_price; //   e.g. 9000
    double m_bid;   //   e.g. 100
    double m_ask;   //   e.g. 101
    StrikeNode(double price=-1, double bid=-1, double ask=-1):
    m_price(price),m_bid(bid),m_ask(ask){}
};
//typedef std::map<double,Strike*> StrikeMap;
//typedef std::pair<double,Strike*> StrikePair;
struct OptionType{
    enum Type{
        CALL =0,
        PUT =1,
    };
};

struct Strike{
    double m_price; //   e.g. 9000
    StrikeNode * m_call;
    StrikeNode * m_put;
    Strike(double price);
    Strike(OptionType::Type type, StrikeNode * strikeNode);
    ~Strike();
};
typedef std::map<double,Strike*> OrderMap;
typedef std::pair<double,Strike*> OrderPair;

struct OrderBook{
    OrderMap m_orderbook;
    OrderBook();
    ~OrderBook();
    
    // addStrike // if strike exist then update it
    void updateStrike(OptionType::Type type, StrikeNode* strikeNode);
    //void updateStrike(OptionType::Type type, Strike* strike);
    
    void dump();
};

#endif /* OrderBook_hpp */
