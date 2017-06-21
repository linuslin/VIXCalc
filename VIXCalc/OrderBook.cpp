//
//  OrderBook.cpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/23.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#include "OrderBook.hpp"


void OrderBook::updateStrike(OptionType::Type type, StrikeNode *strikeNode){
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    try {
        if(strikeNode!=NULL){
            if(strikeNode->m_price>0){
                //OptionType* option;
                //std::cout<<"OptionType="<<type<<std::endl;
                //if(type==OptionType::BUY){
                //    option=&(this->m_buy);
                //}else if(type==OptionType::SELL){
                //    option=&(this->m_sell);
                //}else{
                //    std::cerr<<"OptionType neither BUY nor SELL"<<std::endl;
                //    return;
                //}
                OrderMap::iterator it = this->m_orderbook.find(strikeNode->m_price);
                if (it != m_orderbook.end()){
                    // already has value ...
                        if(type==OptionType::BUY){
                            if(it->second->m_buy){
                                delete it->second->m_buy;
                            }
                            it->second->m_buy=strikeNode;
                        }else if(type==OptionType::SELL){
                            if(it->second->m_sell){
                                delete it->second->m_sell;
                            }
                            it->second->m_sell=strikeNode;
                        }
                }else{
                    // not found insert it
                    Strike * tmpOpt = new Strike(type,strikeNode);
                    m_orderbook.insert(OrderPair(strikeNode->m_price,tmpOpt));
                }
            }else{
                std::cerr<<"Param strikeNode->m_price is less than 0!"<<std::endl;
            }
        }else{
            std::cerr<<"Param strikeNode is NULL"<<std::endl;
        }
    } catch (std::exception &e) {
        std::cerr<<e.what()<<std::endl;
    }
}


void OrderBook::dump(){
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    std::cout<<"--------    BUY     --------"<<std::endl;
    
    OrderMap::iterator it = this->m_orderbook.begin();
    for (; it!=this->m_orderbook.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_buy->m_bid << '\t' << it->second->m_buy->m_ask<<std::endl;
        
    }
    
    std::cout<<"--------    SELL    --------"<<std::endl;
    it = this->m_orderbook.begin();
    for (; it!=this->m_orderbook.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_sell->m_bid << '\t' << it->second->m_sell->m_ask<<std::endl;
    }
}


OrderBook::OrderBook(){
}

OrderBook::~OrderBook(){
    OrderMap::iterator it = m_orderbook.begin();
    for (; it!=this->m_orderbook.end(); ++it){
        //if (it->second?)
        delete it->second;
    }
}


Strike::Strike(double price):m_price(price){
    m_buy = new StrikeNode(price,0,0);
    m_sell = new StrikeNode(price,0,0);
}

Strike::Strike(OptionType::Type type, StrikeNode * strikeNode){
    if (type == OptionType::BUY){
        m_buy = strikeNode;
        m_sell = new StrikeNode(strikeNode->m_price);
    }else if(type == OptionType::SELL){
        m_sell = strikeNode;
        m_buy = new StrikeNode(strikeNode->m_price);
    }else{
        std::cerr<< "this should not be happened"<<std::endl;
    }
    m_price = strikeNode->m_price;
}

Strike::~Strike(){
    if (m_buy) {
        delete m_buy;
    }
    if (m_sell) {
        delete m_sell;
    }
}
