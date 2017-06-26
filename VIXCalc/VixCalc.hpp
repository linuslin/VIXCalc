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
#include <vector>
//#include "OrderBook.hpp"


#include "OrderBook.hpp"


class VixCalcInfoNode{
public:
    typedef enum e{
        CALL = 0,
        PUT = 1,
        K0 = 2,
        NONE = 3,
    }InfoType;
    //double m_price;
    //OptionType::Type m_type;
    InfoType m_type;
    OrderMap::reverse_iterator m_itor;
    double m_deltaK;
    double m_midQuotePrice;
    double m_contribution;
    VixCalcInfoNode():m_deltaK(-1),m_midQuotePrice(-1),m_contribution(-1){};
    VixCalcInfoNode(InfoType type, OrderMap::reverse_iterator itor):m_type(type),m_itor(itor), m_deltaK(-1),m_midQuotePrice(-1),m_contribution(-1){};
    
};

typedef std::map<double,VixCalcInfoNode*> VixCalcInfoMap;
typedef std::pair<double,VixCalcInfoNode*> VixCalcInfoPair;


class VixCalcHelper{
public:
    typedef enum e{
        ERROR = -1,
        NORMAL = 0,
        BID_ZERO = 1,
        ASK_ZERO = 2,
        BIDASK_ZERO =3, //should never happened.
    }BIDASK_TYPE;
    static int isBidAskWithZero(OrderMap::reverse_iterator & itor,OptionType::Type type);
    //VixCalcInfoMap m_infoMap;
    //VixCalcInfoMap m_outlier;
    //~VixCalcHelper();
    static int insertInfoMap(VixCalcInfoMap &infomap, double strike,VixCalcInfoNode * info);
    static int filterOrderbook(OrderBook& orderbook, OrderMap::reverse_iterator& k0Itor,VixCalcInfoMap& result, VixCalcInfoMap& filterOut);
    static int calculateDeltaK(VixCalcInfoMap &infomap);
    static int calculateMidQuote(VixCalcInfoMap &infomap);
    static int calculateContribution(VixCalcInfoMap &infomap);
    static void dumpInfoMap(VixCalcInfoMap infomap);
};

class VixCalc{
    
public:
    // expTime time before
    static double caculateSigma(OrderBook& ob, const double& expTime, const double& rfRate);
    double caculateVIX(const double& nearSigma,const double& nearExpTime, const double& nearSetTime, const double& nextSigma,const double& nextExpTime, const double& nextSetTime, const double& d30Time, const double& d365Time );
    //virtual void caculate();
};


#endif /* VixCalc_hpp */
