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
    double sigma = 0;
    
    // step 1: calculate the mid price of buy and sell
    double buy_mid;
    ordermap::iterator it = ob.m_buy.begin();
    for (; it!=ob.m_buy.end(); ++it){
        std::cout << it->first << "\t" << it->second->m_bid << '\t' << it->second->m_ask<< (it->second->m_bid + it->second->m_ask)/2 <<std::endl;
        
    }
    
    return sigma;
    
}
