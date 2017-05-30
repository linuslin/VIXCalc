//
//  VixCalc.hpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/23.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#ifndef VixCalc_hpp
#define VixCalc_hpp

#include <stdio.h>
//#include "OrderBook.hpp"

#include "OrderBook.hpp"

class VixCalc{
public:
    double caculateSigma(const OrderBook& orderbook, const double& expTime, const double& RFRate);
    double caculateVIX(const double& nearSigma,const double& nearExpTime, const double& nearSetTime, const double& nextSigma,const double& nextExpTime, const double& nextSetTime, const double& d30Time, const double& d365Time );
    //virtual void caculate();
};
#endif /* VixCalc_hpp */
