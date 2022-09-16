//
//  main.cpp
//  proj2
//
//  Created by Kelly Yu on 10/9/21.
//

#include <iostream>
using namespace std;
#include <string>

int main(){

// variables that require user inputs
    int odoStart=0, odoEnd=0, rentalDays=0, month=0;
    string customerName="", luxuryCar="";

// gathering inputs and storing them in variables
    cout << "Odometer at start: ";
    cin >> odoStart;
    cout << "Odometer at end: ";
    cin >> odoEnd;
    cout << "Rental days: ";
    cin >> rentalDays;
    cin.ignore(10000, '\n');
    
    cout << "Customer name: ";
    getline(cin, customerName);
    cout << "Luxury car? (y/n): ";
    getline(cin, luxuryCar);
    cout << "Month (1=Jan, 2=Feb, etc.): ";
    cin >> month;
    
    cout << "---" << endl;

// base charge depends on whether or not car is luxury
    int baseCharge=0;
    if (luxuryCar == "y"){
        baseCharge=71;}
    else if (luxuryCar == "n"){
        baseCharge=43;}

// check if it's winter season; rates are dependent on answer
    string winter = "";
    
    if (month == 12 || (month >= 1 && month <= 3)){
        winter = "y";}
    else{
        winter = "n";}

// the different rates
    double rateFirst100=0.27;
    double rateNext400Winter=0.27;
    double rateNext400NotWinter=0.21;
    double rateBeyond500=0.17;

// set precision for dollar and cents value
    cout.setf(ios::fixed);
    cout.precision(2);

// calculating mileage charge; rates differ based on miles driven & winter season
    double milesDriven = odoEnd - odoStart;
    double mileageCharge=0;

    if (milesDriven <= 100){
        mileageCharge = milesDriven * rateFirst100;}
    else if (winter == "y"){
        if (milesDriven > 100 && milesDriven <=500){
            mileageCharge = 100*rateFirst100 + (milesDriven-100)* rateNext400Winter;}
        else if (milesDriven > 500){
            mileageCharge = 100*rateFirst100 + 400*rateNext400Winter + (milesDriven-500)*rateBeyond500;}
            }
    else if (winter == "n"){
        if (milesDriven > 100 && milesDriven <=500){
            mileageCharge = 100*rateFirst100 + (milesDriven-100)* rateNext400NotWinter;}
        else if (milesDriven > 500){
            mileageCharge = 100*rateFirst100 + 400*rateNext400NotWinter + (milesDriven-500)*rateBeyond500;}
        }

// calculating and presenting the total charge
    double totalCharge = mileageCharge + baseCharge * rentalDays;

// outputting results
    if (odoStart < 0){
        cout << "The starting odometer reading must not be negative." << endl;}
    else if (odoEnd < odoStart){
        cout << "The ending odometer reading must be at least as large as the starting reading." << endl;}
    else if (rentalDays <= 0){
        cout << "The number of rental days must be positive." << endl;}
    else if (customerName == ""){
        cout << "You must enter a customer name." << endl;}
    else if (luxuryCar != "y" && luxuryCar != "n"){
        cout << "You must enter y or n." << endl;}
    else if (month < 1 || month > 12){
        cout << "The month number must be in the range 1 through 12." << endl;}
    else{
        cout << "The rental charge for " << customerName << " is $" << totalCharge << endl ;
    }
    
}
