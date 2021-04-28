#include <iostream>
#include <map>
#include <string>
using namespace std;

namespace ariel {
    class NumberWithUnits{

    private:
        double value;
        string unit;

    public:
        NumberWithUnits(const double val,const string& un);
        // ~NumberWithUnits(){} // not needed.
        static void read_units(ifstream& file);
        static map<string, map<string, double> > convertor; 

        friend ostream& operator<<(ostream& out, const NumberWithUnits& n);
        friend istream& operator>>(istream& in, NumberWithUnits& n);


        NumberWithUnits operator+(const NumberWithUnits& n) const;
        NumberWithUnits operator-(const NumberWithUnits& n) const;
        NumberWithUnits operator-() const;
        NumberWithUnits operator+() const;

        NumberWithUnits& operator+=(const NumberWithUnits& n);
        NumberWithUnits& operator-=(const NumberWithUnits& n);


        NumberWithUnits& operator++();
        NumberWithUnits& operator--();
        NumberWithUnits operator++(int);
        NumberWithUnits operator--(int);

        friend NumberWithUnits operator*(const NumberWithUnits& n, double num);
        friend NumberWithUnits operator*(double num, const NumberWithUnits& n);
        
        // NumberWithUnits operator*(const int num);
        // friend NumberWithUnits operator*(NumberWithUnits& n,int num);
        // friend NumberWithUnits operator*(int num, NumberWithUnits& n);
        bool operator>(const NumberWithUnits& n1) const;
        bool operator>=(const NumberWithUnits& n1) const;
        bool operator<(const NumberWithUnits& n1) const;
        bool operator<=(const NumberWithUnits& n1) const;
        bool operator==(const NumberWithUnits& n1) const;
        bool operator!=(const NumberWithUnits& n1) const;
       

        double convert(const string &uFrom, const string &uTo,const double value)const;
    };      

}