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


class VixCalcInfoNode{
public:
    //double m_price;
    //OptionType::Type m_type;
    double m_deltaK;
    double m_midQuotePrice;
    double m_contribution;
    VixCalcInfoNode():m_deltaK(-1),m_midQuotePrice(-1),m_contribution(-1){};
};

class VixCalcInfo{
public:
    //double m_price;
    VixCalcInfoNode * m_buy;
    VixCalcInfoNode * m_sell;
    VixCalcInfo();
    ~VixCalcInfo();
};


typedef std::map<double,VixCalcInfo*> VixCalcInfoMap;
typedef std::pair<double,VixCalcInfo*> VixCalcInfoPair;

class VixCalcHelper{
public:
    VixCalcInfoMap m_calcMap;
    ~VixCalcHelper();
    void insertInfoMap(const Strike* strike,VixCalcInfo * info);
};

class VixCalc{
    
public:
    // expTime time before
    static double caculateSigma(OrderBook& ob, const double& expTime, const double& rfRate);
    double caculateVIX(const double& nearSigma,const double& nearExpTime, const double& nearSetTime, const double& nextSigma,const double& nextExpTime, const double& nextSetTime, const double& d30Time, const double& d365Time );
    //virtual void caculate();
};


#endif /* VixCalc_hpp */
