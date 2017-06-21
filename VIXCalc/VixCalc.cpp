//
//  VixCalc.cpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/23.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#include "VixCalc.hpp"

#include <cmath>



double VixCalc::caculateSigma(OrderBook& ob, const double& expTime, const double& rfRate){
    std::ostream restore(0);
    restore.copyfmt(std::cout);
    std::cout<< std::fixed;
    std::cout.precision(6);
    double sigma = 0;
    //VixCalcMap * calcMap = new VixCalcMap();
    VixCalcHelper * calcHelper = new VixCalcHelper();
    
    // step 1: calculate the mid price of buy and sell
    OrderMap::iterator it = ob.m_orderbook.begin();
    OrderMap::iterator minDiffIdx;
    double minValue = MAXFLOAT;
    double diff = 0;
    VixCalcInfo * info=NULL;
    for (; it!=ob.m_orderbook.end(); ++it){
        info = new VixCalcInfo();
        info->m_buy->m_midQuotePrice = (it->second->m_buy->m_bid + it->second->m_buy->m_ask)/2;
        info->m_sell->m_midQuotePrice = (it->second->m_sell->m_bid + it->second->m_sell->m_ask)/2;
        
        diff = std::fabs((it->second->m_buy->m_bid + it->second->m_buy->m_ask)-(it->second->m_sell->m_bid + it->second->m_sell->m_ask));
        std::cout << it->first << "\t" << it->second->m_buy->m_bid << '\t' << it->second->m_buy->m_ask<<'\t'<< (it->second->m_buy->m_bid + it->second->m_buy->m_ask)/2<<'\t';
        std::cout << "\t" << it->second->m_sell->m_bid << '\t' << it->second->m_sell->m_ask<<'\t'<< (it->second->m_sell->m_bid + it->second->m_sell->m_ask)/2;
        std::cout<<'\t'<<diff/2<<std::endl;
        if (minValue>diff) {
            minValue=diff;
            minDiffIdx=it;
        }else if (minValue==diff){
            std::cout<<"has same diff"<<std::endl;
            // discussed with fuching.
            if((minDiffIdx->second->m_buy->m_bid+ minDiffIdx->second->m_buy->m_ask
            + minDiffIdx->second->m_sell->m_bid + minDiffIdx->second->m_sell->m_ask) <
               (it->second->m_buy->m_bid + it->second->m_buy->m_ask
                + it->second->m_sell->m_bid + it->second->m_sell->m_ask)){
                   minDiffIdx=it;
               }
        }
        
        calcHelper->insertInfoMap(it->second,info);
        //calcHelper->m_calcMap.
        //calcHelper->addVixCalcNode();
        //calcHelper->addVixCalcNode(new VixCalcNode());
    }
    
    std::cout<<"min strike:\t" << minDiffIdx->first << std::endl;
    std::cout<<"expTime:\t" << expTime << std::endl;
    std::cout<<"rfRate:\t" << rfRate << std::endl;
    std::cout<<"exp(expTime * rfRate):\t"<< exp(expTime*rfRate) <<std::endl;
    std::cout<<"diff:\t" << ((minDiffIdx->second->m_buy->m_bid + minDiffIdx->second->m_buy->m_ask)-(minDiffIdx->second->m_sell->m_bid + minDiffIdx->second->m_sell->m_ask))/2<<std::endl;
    
    std::cout<<"========= Caculate Forward Price ========"<<std::endl;
    
    double forwardPrice = minDiffIdx->first + exp(expTime*rfRate) * ((minDiffIdx->second->m_buy->m_bid + minDiffIdx->second->m_buy->m_ask)-(minDiffIdx->second->m_sell->m_bid + minDiffIdx->second->m_sell->m_ask))/2;
    
    std::cout<<"forwardPrice:\t"<<forwardPrice<<std::endl;
    std::cout<<"========= Determinate K0 ========"<<std::endl;
    OrderMap::reverse_iterator k0Idx=ob.m_orderbook.rbegin();
    
    //TODO: need to improve
    for (; k0Idx!=ob.m_orderbook.rend(); ++k0Idx){
        if (k0Idx->first < forwardPrice) {
            break;
        }
    }
    if(k0Idx->first > forwardPrice){
        std::cerr<<"No k0 !!!"<<std::endl;
    }
    std::cout<<"k0:\t"<<k0Idx->first<<std::endl;
    
    
    
    
    
    // TODO: need to improve
    delete calcHelper;
    
    std::cout.copyfmt(restore);
    //std::cout<<std::default;
    return sigma;
    
}

//void VixCalcHelper::(OptionType::Type type, Strike *strike) {
    

VixCalcHelper::~VixCalcHelper(){
    VixCalcInfoMap::iterator it = m_calcMap.begin();
    for (; it!=m_calcMap.end(); ++it){
        delete it->second;
    }
}

void VixCalcHelper::insertInfoMap(const Strike* strike, VixCalcInfo * info){
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    try {
        if(strike!=NULL){
            if(strike->m_price>0){
                VixCalcInfoMap::iterator it = m_calcMap.find(strike->m_price);
                if (it != m_calcMap.end()){
                    // already has value ...
                    if (it->second) {
                        delete it->second;
                    }
                    it->second=info;
                }else{
                    // not found insert it
                    m_calcMap.insert(VixCalcInfoPair(strike->m_price,info));
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

VixCalcInfo::VixCalcInfo(){
    m_buy = new VixCalcInfoNode();
    m_sell = new VixCalcInfoNode();
}
//VixCalcInfo::VixCalcInfo();
VixCalcInfo::~VixCalcInfo(){
    if (m_buy) {
        delete m_buy;
    }
    if (m_sell) {
        delete m_sell;
    }
}
