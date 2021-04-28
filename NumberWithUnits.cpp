#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

namespace ariel {

    map<string, map<string, double>> NumberWithUnits::convertor; // NumberWithUnits::convertor[unitFrom][unitTo]=value
    const double EPSILON = 0.0003;

    NumberWithUnits::NumberWithUnits(const double val,const string& un): value(val) , unit(un)
    {
        if(convertor.find(un) == convertor.end()){
            throw invalid_argument("Convertor does Not conatain match- ["+un+"]");
        }
    }

    double NumberWithUnits::convert(const string &uFrom, const string &uTo,const double value) const{
        // cout << "CONVERT" << endl;
        if(uFrom == uTo) {return value;}
        if(NumberWithUnits::convertor.at(uFrom).find(uTo) != NumberWithUnits::convertor.at(uFrom).end()){
            // cout << uFrom << endl;
            return NumberWithUnits::convertor[uFrom][uTo]*value;
        }else{
            throw invalid_argument{"Convertor does Not conatain match uFrom- ["+uFrom+"] uTo ["+uTo+"]"};
        }
    }

    // this function is made to convert units that have transitive of more than 2 connection.
    void updateUnits(string unit1, string unit2){
          for(auto &grandSon : NumberWithUnits::convertor[unit2]) {
                double w = NumberWithUnits::convertor[unit1][unit2] * grandSon.second;
                NumberWithUnits::convertor[unit1][grandSon.first] = w;
                NumberWithUnits::convertor[grandSon.first][unit1] = 1/w;
            }
    }
    
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    void NumberWithUnits::read_units(ifstream& units_file){
        double n1=0, n2=0; 
        string unit1, unit2,none ,line;
        while(getline(units_file,line)){
            istringstream isr(line);
            if(!(isr>> n1 >> unit1 >> none >> n2 >> unit2)){
                break;
            }
            NumberWithUnits::convertor[unit1][unit2] = n2;
            NumberWithUnits::convertor[unit2][unit1] = 1/n2;
            updateUnits(unit1,unit2); // transitive of more than 2 connection.
            updateUnits(unit2,unit1);
        }
    }


    ostream& operator<<(ostream& out, const NumberWithUnits& n){
        return out<< n.value << "[" << n.unit << "]";
    }
    istream& operator>>(istream& in, NumberWithUnits& n){
        char bracketLEFT=' ',bracketRIGHT=' ';
        string temp=" ";
        double tempVal = 0;
        in >> skipws >> tempVal >> bracketLEFT >> temp;

        uint length = temp.length();
        char dummy = temp.at(length - 1);
        if (dummy != ']'){

            in >> bracketRIGHT;
        }
        else{
            temp = temp.substr(0, length - 1);
        }
        if (NumberWithUnits::convertor.count(temp) == 0)
        {
            throw invalid_argument("UNKNOWN UNIT");
        }
        n.value = tempVal;
        n.unit = temp;
        return in;
    }

    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& n) const{
        double converted = this->value + convert(n.unit, this->unit,n.value);
        return NumberWithUnits(converted,this->unit);
    }
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& n) const{
        double converted = this->value - convert(n.unit, this->unit,n.value);
        return NumberWithUnits(converted,this->unit);
    }
    NumberWithUnits NumberWithUnits::operator-() const{
        return NumberWithUnits(-this->value,this->unit);
    }
    NumberWithUnits NumberWithUnits::operator+() const{
        return NumberWithUnits(+this->value,this->unit);
        
    }
    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& n){
        double converted = convert(n.unit, this->unit,n.value);
        this->value+=converted;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& n){
        double converted = convert(n.unit, this->unit,n.value);
        this->value-=converted;
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator++(){
        (this->value)++;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator--(){
        (this->value)--;
        return *this;
    }
    NumberWithUnits NumberWithUnits::operator++(int){
        NumberWithUnits tmp= *this;
        ++(this->value);
        return tmp;
    }
    NumberWithUnits NumberWithUnits::operator--(int){
        NumberWithUnits tmp= *this;
        --(this->value);
        return tmp;
    }
    NumberWithUnits operator*(const NumberWithUnits& n, double num){
        return NumberWithUnits(n.value*num,n.unit);
    }
    NumberWithUnits operator*(double num,const NumberWithUnits& n){
        return NumberWithUnits(n.value*num,n.unit);
    }

    bool NumberWithUnits::operator>(const NumberWithUnits& n1) const{

        return ((this->value) - convert(n1.unit,this->unit,n1.value)) > EPSILON;
    }
    bool NumberWithUnits::operator>=(const NumberWithUnits& n1) const{

        return !(*this < n1);
    }
    bool NumberWithUnits::operator<(const NumberWithUnits& n1) const{

        return (n1 > *this);
    }
    bool NumberWithUnits::operator<=(const NumberWithUnits& n1) const{

        return !(*this > n1);
    }
    bool NumberWithUnits::operator==(const NumberWithUnits& n1) const{
        return abs( (this->value) - convert(n1.unit,this->unit,n1.value) ) <= EPSILON;
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits& n1) const{

        return !(*this == n1);
    }
}