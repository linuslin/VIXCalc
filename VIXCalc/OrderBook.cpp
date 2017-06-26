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
                //if(type==OptionType::CALL){
                //    option=&(this->m_call);
                //}else if(type==OptionType::PUT){
                //    option=&(this->m_put);
                //}else{
                //    std::cerr<<"OptionType neither CALL nor PUT"<<std::endl;
                //    return;
                //}
                OrderMap::iterator it = this->m_orderbook.find(strikeNode->m_price);
                if (it != m_orderbook.end()){
                    // already has value ...
                        if(type==OptionType::CALL){
                            if(it->second->m_call){
                                delete it->second->m_call;
                            }
                            it->second->m_call=strikeNode;
                        }else if(type==OptionType::PUT){
                            if(it->second->m_put){
                                delete it->second->m_put;
                            }
                            it->second->m_put=strikeNode;
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
    std::cout<<"--------    CALL     --------"<<std::endl;
    
    OrderMap::iterator it = this->m_orderbook.begin();
    for (; it!=this->m_orderbook.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_call->m_bid << '\t' << it->second->m_call->m_ask<<std::endl;
        
    }
    
    std::cout<<"--------    PUT    --------"<<std::endl;
    it = this->m_orderbook.begin();
    for (; it!=this->m_orderbook.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_put->m_bid << '\t' << it->second->m_put->m_ask<<std::endl;
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
    m_call = new StrikeNode(price,0,0);
    m_put = new StrikeNode(price,0,0);
}

Strike::Strike(OptionType::Type type, StrikeNode * strikeNode){
    if (type == OptionType::CALL){
        m_call = strikeNode;
        m_put = new StrikeNode(strikeNode->m_price);
    }else if(type == OptionType::PUT){
        m_put = strikeNode;
        m_call = new StrikeNode(strikeNode->m_price);
    }else{
        std::cerr<< "this should not be happened"<<std::endl;
    }
    m_price = strikeNode->m_price;
}

Strike::~Strike(){
    if (m_call) {
        delete m_call;
    }
    if (m_put) {
        delete m_put;
    }
}
