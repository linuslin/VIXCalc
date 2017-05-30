//
//  OrderBook.cpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/23.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#include "OrderBook.hpp"


void OrderBook::addStrike(OptionType::Type type, Strike *strike, bool update = false){
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    try {
        if(strike!=NULL){
            if(strike->m_price>0){
                ordermap* option;
                std::cout<<"OptionType="<<type<<std::endl;
                if(type==OptionType::BUY){
                    option=&(this->m_buy);
                }else if(type==OptionType::SELL){
                    option=&(this->m_sell);
                }else{
                    std::cerr<<"OptionType neither BUY nor SELL"<<std::endl;
                    return;
                }
                ordermap::iterator it = option->find(strike->m_price);
                if (it != option->end()){
                    // already has value ...
                    if (update) {
                        // replace it
                        delete it->second;
                        it->second=strike;
                    }else{
                        std::cerr<<"Ordermap already has vaule"<<std::endl;
                    }
                }else{
                    // not found insert it
                    option->insert(orderpair(strike->m_price,strike));
                }
            }else{
                std::cerr<<"Param strike->m_price is negative!"<<std::endl;
            }
        }else{
            std::cerr<<"Param strike is NULL"<<std::endl;
        }
    } catch (std::exception &e) {
        std::cerr<<e.what()<<std::endl;
    }
}

void OrderBook::dump(){
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    std::cout<<"--------    BUY     --------"<<std::endl;
    
    ordermap::iterator it = this->m_buy.begin();
    for (; it!=this->m_buy.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_bid << '\t' << it->second->m_ask<<std::endl;
    }
    
    std::cout<<"--------    SELL    --------"<<std::endl;
    it = this->m_sell.begin();
    for (; it!=this->m_sell.end(); ++it){
        std::cout << it->first <<"\t"<< it->second->m_bid << '\t' << it->second->m_ask<<std::endl;
    }
}
