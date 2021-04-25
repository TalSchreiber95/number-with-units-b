#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "NumberWithUnits.hpp"
using namespace std;

namespace ariel {

    static map<string, map<string, double> > convertor; // convertor[unitFrom][unitTo]=value

    double convert(const string uFrom, const string uTo,double value){
        if(uFrom == uTo) {return value;}
        try {
            return convertor.at(uFrom).at(uTo)*value;
        }
        catch(const exception& e) {
            throw invalid_argument{"Convertor does Not conatain match uFrom- ["+uFrom+"] uTo ["+uTo+"]"};
        }
    }

    // this function is made to convert units that have transitive of more than 2 connection.
    void updateUnits(string unit1,string unit2){
          for(auto &pair : convertor[unit2]) {
                double w = convertor[unit1][unit2] * pair.second;
                convertor[unit1][pair.first] = w;
                convertor[pair.first][unit1] = 1/w;
            }
    }
    
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    void NumberWithUnits::read_units(ifstream& units_file){
        double n1, n2; 
        string unit1, unit2,none ,line;
        while(getline(units_file,line)){
            istringstream isr(line);
            if(!(isr>> n1 >> unit1 >> none >> n2 >> unit2)){
                break;
            }
            convertor[unit1][unit2] = n2;
            convertor[unit2][unit1] = 1/n2;
            updateUnits(unit1,unit2); // transitive of more than 2 connection.
            updateUnits(unit2,unit1);
        }
    }

    ostream& operator<<(ostream& out, const NumberWithUnits& t){
        return out<< t.value << "(" << t.unit << ")";
    }
    istream& operator>>(istream& in, NumberWithUnits& t){
        string s;
        return in >> t.value >> s >> t.unit;
    }

    NumberWithUnits operator+(const NumberWithUnits& n1, const NumberWithUnits& n2) {
        double converted = convert(n2.unit, n1.unit,n2.value);
        return NumberWithUnits(n1.value+converted, n1.unit);
    }
    NumberWithUnits operator+(const NumberWithUnits& n, double a) {
        return NumberWithUnits(n.value+a, n.unit);
    }
    NumberWithUnits operator+=(NumberWithUnits& n1, const NumberWithUnits& n2) {
        n1.value += convert(n2.unit, n1.unit,n2.value);
        return n1;
    }
    NumberWithUnits operator-(const NumberWithUnits& n1, const NumberWithUnits& n2) {
        double converted = convert(n2.unit, n1.unit,n2.value);
        return NumberWithUnits(n1.value-converted, n1.unit);
    }
    NumberWithUnits operator-(const NumberWithUnits& n) {
        return NumberWithUnits(-n.value, n.unit);
    }
    NumberWithUnits operator+(const NumberWithUnits& n){
         return NumberWithUnits(n.value, n.unit);
    }

    NumberWithUnits operator-=(NumberWithUnits& n1, const NumberWithUnits& n2) {
        double converted = convert(n2.unit, n1.unit,n2.value);
        return NumberWithUnits(n1.value-=converted, n1.unit);
    }

    NumberWithUnits operator++(NumberWithUnits& n) {
        // ++n.value;
        // return n;
        return NumberWithUnits(++n.value, n.unit);

    }
    NumberWithUnits operator++(NumberWithUnits& n, int) {
        // n.value++;
        // return n;
        return NumberWithUnits(n.value++, n.unit);

    }
    NumberWithUnits operator--(NumberWithUnits& n) {
        // --n.value;
        // return n;
        return NumberWithUnits(--n.value, n.unit);

    }
    NumberWithUnits operator--(NumberWithUnits& n, int) {
        // n.value--;
        // return n;
        return NumberWithUnits(n.value--, n.unit);
    }
    NumberWithUnits operator*(NumberWithUnits& n, double num) {
        // n.value*=num;
        // return n;
        return NumberWithUnits(n.value*num, n.unit);
    }
    NumberWithUnits operator*(double num, NumberWithUnits& n) {
        // n.value*=num;
        // return n;
        return NumberWithUnits(n.value*num, n.unit);
    }
    NumberWithUnits operator*=(NumberWithUnits& n, double num) {
        //n.value*=num;
        // return n;
        return NumberWithUnits(n.value*=num, n.unit);
    }
    NumberWithUnits operator*=(double num, NumberWithUnits& n) {
        // n.value*=num;
        // return n;
        return NumberWithUnits(n.value*=num, n.unit);
    }
        NumberWithUnits operator*(NumberWithUnits& n, int num) {
        return NumberWithUnits(n.value*num, n.unit);
    }
    NumberWithUnits operator*(int num, NumberWithUnits& n) {
        // n.value*=num;
        // return n;
        return NumberWithUnits(n.value*num, n.unit);
    }
    NumberWithUnits operator*=(NumberWithUnits& n, int num) {
        //n.value*=num;
        // return n;
        return NumberWithUnits(n.value*=num, n.unit);
    }
    NumberWithUnits operator*=(int num, NumberWithUnits& n) {
        // n.value*=num;
        // return n;
        return NumberWithUnits(n.value*=num, n.unit);
    }

    int compare(const NumberWithUnits& n1, const NumberWithUnits& n2){
        double x = n1.value - convert(n2.unit, n1.unit,n2.value);
        double y = convert(n2.unit, n1.unit,n2.value) - n1.value;
        double epsilon = 0.00001;
        if(x > epsilon){
            return 1;
        }
        else if(y > epsilon){
            return -1;
        }
        else{
            return 0;
        }
    }

    bool operator>(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) > 0;
    }
    bool operator>=(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) >= 0;
    }
    bool operator<(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) < 0;
    }
    bool operator<=(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) <= 0;
    }
    bool operator==(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) == 0;
    }
    bool operator!=(const NumberWithUnits& n1, const NumberWithUnits& n2){
        return compare(n1, n2) != 0;
    }
}