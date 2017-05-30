//
//  OrderBookTest.cpp
//  VIXCalc
//
//  Created by Shin Cheng Lin on 2017/5/29.
//  Copyright © 2017年 LINK-US CO., LTD. All rights reserved.
//

#include "OrderBookTest.hpp"


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "OrderBook.hpp"

class CSVRow
{
public:
    std::string const& operator[](std::size_t index) const
    {
        return m_data[index];
    }
    std::size_t size() const
    {
        return m_data.size();
    }
    void readNextRow(std::istream& str)
    {
        std::string         line;
        std::getline(str, line);
        
        std::stringstream   lineStream(line);
        std::string         cell;
        
        m_data.clear();
        while(std::getline(lineStream, cell, ','))
        {
            m_data.push_back(cell);
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add an empty element.
            m_data.push_back("");
        }
    }
private:
    std::vector<std::string>    m_data;
};


std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

void OrderBookTest::test()
{
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    std::ifstream file("20170526.csv");
    std::string line;
    if (!file.is_open()){
        std::cout << "Unable to open file"<<std::endl;
    }
    
    CSVRow row;
    OrderBook * ob = new OrderBook();
    while(file >> row)
    {
        ob->addStrike(OptionType::BUY, new Strike(atof(row[2].c_str()),atof(row[0].c_str()),atof(row[1].c_str())),false);
        ob->addStrike(OptionType::SELL, new Strike(atof(row[2].c_str()),atof(row[3].c_str()),atof(row[4].c_str())),false);
    }
    ob->dump();
    file.close();
}
