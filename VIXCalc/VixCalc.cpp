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
    
    
    // step 1: calculate the mid price of buy and sell
    OrderMap::const_iterator it = ob.m_orderbook.begin();
    OrderMap::const_iterator minDiffIdx;
    double minValue = MAXFLOAT;
    double diff = 0;
    for (; it!=ob.m_orderbook.end(); ++it){
        diff = std::fabs((it->second->m_call->m_bid + it->second->m_call->m_ask)-(it->second->m_put->m_bid + it->second->m_put->m_ask));
        std::cout << it->first << "\t" << it->second->m_call->m_bid << '\t' << it->second->m_call->m_ask<<'\t'<< (it->second->m_call->m_bid + it->second->m_call->m_ask)/2<<'\t';
        std::cout << "\t" << it->second->m_put->m_bid << '\t' << it->second->m_put->m_ask<<'\t'<< (it->second->m_put->m_bid + it->second->m_put->m_ask)/2;
        std::cout<<'\t'<<diff/2<<std::endl;
        if (minValue>diff) {
            minValue=diff;
            minDiffIdx=it;
        }else if (minValue==diff){
            std::cout<<"has same diff"<<std::endl;
            // discussed with fuching.
            if((minDiffIdx->second->m_call->m_bid+ minDiffIdx->second->m_call->m_ask
            + minDiffIdx->second->m_put->m_bid + minDiffIdx->second->m_put->m_ask) <
               (it->second->m_call->m_bid + it->second->m_call->m_ask
                + it->second->m_put->m_bid + it->second->m_put->m_ask)){
                   minDiffIdx=it;
               }
        }
        
    }
    
    std::cout<<"min strike:\t" << minDiffIdx->first << std::endl;
    std::cout<<"expTime:\t" << expTime << std::endl;
    std::cout<<"rfRate:\t" << rfRate << std::endl;
    std::cout<<"exp(expTime * rfRate):\t"<< exp(expTime*rfRate) <<std::endl;
    std::cout<<"diff:\t" << ((minDiffIdx->second->m_call->m_bid + minDiffIdx->second->m_call->m_ask)-(minDiffIdx->second->m_put->m_bid + minDiffIdx->second->m_put->m_ask))/2<<std::endl;
    
    std::cout<<"========= Caculate Forward Price ========"<<std::endl;
    
    double forwardPrice = minDiffIdx->first + exp(expTime*rfRate) * ((minDiffIdx->second->m_call->m_bid + minDiffIdx->second->m_call->m_ask)-(minDiffIdx->second->m_put->m_bid + minDiffIdx->second->m_put->m_ask))/2;
    
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
    
    
    
    
    
    //VixCalcHelper * calcHelper = new VixCalcHelper();
    
    
    VixCalcInfoMap infomap;
    VixCalcInfoMap filterout;
    
    VixCalcHelper::filterOrderbook(ob, k0Idx, infomap, filterout);
    VixCalcHelper::dumpInfoMap(infomap);
    
    // TODO: need to improve
    //delete calcHelper;
    
    std::cout.copyfmt(restore);
    //std::cout<<std::default;
    return sigma;
    
}


//VixCalcHelper::BIDASK_TYPE VixCalcHelper::isBidAskZero(OrderMap::reverse_iterator &itor){
    //return VixCalcHelper::ERROR;
//}

void VixCalcHelper::dumpInfoMap(VixCalcInfoMap infomap){
    std::cout<<"======= dump infomap ========" <<std::endl;
    VixCalcInfoMap::iterator itor = infomap.begin();
    for (; itor!=infomap.end(); ++itor) {
        std::cout<<itor->first<<"\t"<<itor->second->m_type <<"\t"
                <<itor->second->m_deltaK<<"\t"<<itor->second->m_midQuotePrice
                <<"\t"<<itor->second->m_contribution<<std::endl;
    }
}

int VixCalcHelper::isBidAskWithZero(OrderMap::reverse_iterator & itor,OptionType::Type type){
    if (type==OptionType::CALL) {
        return itor->second->m_call->m_ask == 0 || itor->second->m_call->m_bid ==0;
    }else if (type==OptionType::PUT){
        return itor->second->m_put->m_ask == 0 || itor->second->m_put->m_bid ==0;
    }
    return 0;
}

int VixCalcHelper::filterOrderbook(OrderBook& orderbook, OrderMap::reverse_iterator& k0Itor,VixCalcInfoMap& result, VixCalcInfoMap& filterOut){
    if (orderbook.m_orderbook.size() > 1) {
        //if (k0Itor == orderbook.m_orderbook.rbegin() || k0Itor == orderbook.m_orderbook.rend()) {
        //    cerr << __PRETTY_FUNCTION__ <<" "<<"k0is"
        //    return -1;
        //}
        
        int times=0;
        //PUT
        if(k0Itor!= orderbook.m_orderbook.rend()){
            OrderMap::reverse_iterator tmpPutItor=k0Itor;
            ++tmpPutItor;
            
            for (; tmpPutItor!=orderbook.m_orderbook.rend(); ++tmpPutItor) {
                if (times>=2) {
                    VixCalcHelper::insertInfoMap(filterOut, tmpPutItor->first, new VixCalcInfoNode(VixCalcInfoNode::NONE,tmpPutItor));
                }else if (VixCalcHelper::isBidAskWithZero(tmpPutItor, OptionType::PUT)) {
                    VixCalcHelper::insertInfoMap(filterOut, tmpPutItor->first, new VixCalcInfoNode(VixCalcInfoNode::NONE,tmpPutItor));
                    ++times;
                }else{
                    VixCalcHelper::insertInfoMap(result, tmpPutItor->first, new VixCalcInfoNode(VixCalcInfoNode::PUT,tmpPutItor));
                    times=0;
                }
            }
        }
        // K0
        VixCalcHelper::insertInfoMap(result, k0Itor->first, new VixCalcInfoNode(VixCalcInfoNode::K0,k0Itor));
        
        // CALL
        if (k0Itor!= orderbook.m_orderbook.rbegin()) {
            OrderMap::reverse_iterator tmpCallItor=k0Itor;
            --tmpCallItor;
            times=0;
            for (; tmpCallItor!=orderbook.m_orderbook.rbegin(); --tmpCallItor) {
                if (times>=2) {
                    VixCalcHelper::insertInfoMap(filterOut, tmpCallItor->first, new VixCalcInfoNode(VixCalcInfoNode::NONE,tmpCallItor));
                    
                }else if (VixCalcHelper::isBidAskWithZero(tmpCallItor, OptionType::CALL)) {
                    VixCalcHelper::insertInfoMap(filterOut, tmpCallItor->first, new VixCalcInfoNode(VixCalcInfoNode::NONE,tmpCallItor));
                    ++times;
                }else{
                    VixCalcHelper::insertInfoMap(result, tmpCallItor->first, new VixCalcInfoNode(VixCalcInfoNode::CALL,tmpCallItor));
                    times=0;
                }
            }
        }
        
        return 0;
    }
    std::cerr<<__PRETTY_FUNCTION__<<" "<<"orderbook size is less than 1 !!!"<<std::endl;
    return -1;
}
//int calculateDeltaK(VixCalcInfoMap &infomap);
//int calculateMidQuote(VixCalcInfoMap &infomap);
//int calculateContribution(VixCalcInfoMap &infomap);

//void VixCalcHelper::(OptionType::Type type, Strike *strike) {
    
/*
VixCalcHelper::~VixCalcHelper(){
    VixCalcInfoMap::iterator it = m_calcMap.begin();
    for (; it!=m_calcMap.end(); ++it){
        delete it->second;
    }
}

*/


int VixCalcHelper::insertInfoMap(VixCalcInfoMap &infomap, double strike, VixCalcInfoNode *info){
    try {
        if(info!=NULL){
            if(strike>0){
                VixCalcInfoMap::iterator it = infomap.find(strike);
                if (it != infomap.end()){
                    // already has value ...
                    if (it->second) {
                        delete it->second;
                    }
                    it->second=info;
                }else{
                    // not found insert it
                    infomap.insert(VixCalcInfoPair(strike,info));
                }
                return 0;
            }else{
                std::cerr<<__PRETTY_FUNCTION__<<" "<<"Param strikeNode->m_price is less than 0!"<<std::endl;
            }
        }else{
            std::cerr<<__PRETTY_FUNCTION__<<" "<<"Param info is NULL"<<std::endl;
        }
    } catch (std::exception &e) {
        std::cerr<<e.what()<<std::endl;
    }
    return -1;
}
/*
void VixCalcHelper::insertInfoMap(const Strike* strike, VixCalcInfoNode * info){
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
 */

