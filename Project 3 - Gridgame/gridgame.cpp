//
//  main.cpp
//  gridgame
//
//  Created by Kelly Yu on 10/21/21.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

bool hasCorrectForm(string plan);
int determineSafeDistance(int r, int c, char dir, int maxSteps);
int obeyPlan(int sr, int sc, int er, int ec, char dir, string plan, int& nsteps);

#include "grid.h"

int main()
{
    setSize(3,4);
    setWall(1,4);
    setWall(2,2);
    setWall(3,2);
    draw(1,3, 3,1);    // draw the grid, showing an S at (3,1) start
                               //    position, and an E at (3,4) end position
}

//function 1 checks if plan is formatted correctly
bool hasCorrectForm(string plan){
    for (int i=0; i != plan.size(); i++){
        //check if plan only has letters and digits
        if (!(isalpha(plan.at(i))) && !(isdigit(plan.at(i))))
            return false;
        
        //check if letters are only R and L
        if (isalpha(plan.at(i))){
            plan.at(i) = toupper(plan.at(i));
            if (plan.at(i)!='R' && plan.at(i) != 'L')
                return false;
        }
        
        //check if last character in plan is R or L
        if (!(isalpha(plan.at(plan.size()-1))))
            return false;
                    
        //if steps is three digits or more, value is invalid
        if (i < plan.size()-3){
            if (isdigit(plan.at(i)) && isdigit(plan.at(i+1)) && isdigit(plan.at(i+2)))
                return false;
        }
    }
    
    return true;
}


//function 2 checks if horizontal or vertical movement is valid
int determineSafeDistance(int r, int c, char dir, int maxSteps){
    //check if we're at empty space with positive number of steps to take
    if (r < 1|| r > getRows() || c < 1 || c > getCols() || isWall(r,c) || maxSteps < 0)
        return -1;
       
    //create successful steps counter
    int maxValidSteps=0;
    
    //check if we can move maxSteps in indicated direction
    switch(toupper(dir)){
        case 'S':
            for(int k=1; k < maxSteps + 1; k++){    //check each step until we moved maxSteps
                r+=1;
                if(r > getRows() || isWall(r,c))    // check if we're not in empty space
                    return maxValidSteps;       // prints num of possible steps South
                maxValidSteps+=1;               // counts every valid step
            }
            return maxSteps;
            break;
            
        case 'N':                               //repeat for heading North
            for(int k=1; k < maxSteps + 1; k++){
                r-=1;
                if(r < 1 || isWall(r,c))
                    return maxValidSteps;
                maxValidSteps+=1;
            }
            return maxSteps;
            break;
            
        case 'E':                                //repeat for heading East
            for(int k=1; k < maxSteps + 1; k++){
                c+=1;
                if(c > getCols() || isWall(r,c))
                    return maxValidSteps;
                maxValidSteps+=1;
            }
            return maxSteps;
            break;
            
        case 'W':                               //repeat for heading West
            for(int k=1; k < maxSteps + 1; k++){
                c-=1;
                if(c < 1 || isWall(r,c))
                    return maxValidSteps;
                maxValidSteps+=1;
            }
            return maxSteps;
            break;
            
        default:
            return -1;   // return -1 for all invalid direction inputs
    }
}

//function 3 checks if we can to reach endpoint through plan
int obeyPlan(int sr, int sc, int er, int ec, char dir, string plan, int& nsteps){
    
    // check if (sr,sc) and (er,ec) are valid grid points
    if (sr < 1 || sr > getRows() || er < 1 || er > getRows())
        return 2;
    if (sc < 1 || sc > getCols() || ec < 1 || ec > getCols())
        return 2;
    if (isWall(sr,sc) || isWall(er, ec))
        return 2;
    
    //set direction to uppercase
    dir = toupper(dir);
    
    //check if direction is valid
    if (dir != 'N' && dir != 'W' && dir != 'E' && dir != 'S' )
        return 2;
    
    //check if plan is in a valid form
    if (!hasCorrectForm(plan))
        return 2;
    
    char newDir = dir;                // store the car's new direction after turning
    int totalStepsTaken = 0;          // counter for total successful steps we take
    
    for (int j=0; j != plan.size(); j++){
        //turns the car left or right according to the plan
        if (isalpha(plan.at(j))){
            if (plan.at(j) == 'L' || plan.at(j) == 'l' ){
                switch(newDir){
                    case 'N':
                        newDir = 'W';
                        break;
                    case 'W':
                        newDir = 'S';
                        break;
                    case 'S':
                        newDir = 'E';
                        break;
                    case 'E':
                        newDir = 'N';
                    break;}
                }
            else if (plan.at(j)== 'R' || plan.at(j) == 'r'){
                switch(newDir){
                    case 'N':
                        newDir = 'E';
                        break;
                    case 'E':
                        newDir = 'S';
                        break;
                    case 'S':
                        newDir = 'W';
                        break;
                    case 'W':
                        newDir = 'N';
                        break;}
                }
        }

        //if element is not a letter, it must be a digit
        
        //checks the number of steps to take
        if (isdigit(plan.at(j))){
            //for double digit movements
            if (isdigit(plan.at(j+1))){         //won't go out of bounds; digit is not last char
                char tensChar = plan.at(j);
                char onesChar = plan.at(j+1);
                int planSteps = (tensChar - '0')*10 + (onesChar - '0');
                j++;       //for loop will not check the next element twice

            //can travel the indicated number of steps
                if ((determineSafeDistance(sr, sc, newDir, planSteps)==planSteps)){
                    switch(newDir){
                        case 'N':
                            sr -= planSteps;
                            break;
                        case 'W':
                            sc -= planSteps;
                            break;
                        case 'E':
                            sc += planSteps;
                            break;
                        case 'S':
                            sr += planSteps;
                            break;}
                    totalStepsTaken += planSteps;}
                
            //cannot travel ALL indicated number of steps
                else{
                    totalStepsTaken += determineSafeDistance(sr, sc, newDir, planSteps);
                    nsteps = totalStepsTaken;        //number of successful steps before crash
                    return 3;}
            }
        //for single digit movement
            else{
                int planSteps = plan.at(j) - '0';

            //can travel the indicated number of steps
                if ((determineSafeDistance(sr, sc, newDir, planSteps)==planSteps)){
                    switch(newDir){
                        case 'N':
                            sr -= planSteps;
                            break;
                        case 'W':
                            sc -= planSteps;
                            break;
                        case 'E':
                            sc += planSteps;
                            break;
                        case 'S':
                            sr += planSteps;
                            break;}
                    totalStepsTaken += planSteps;}
                
            //cannot travel ALL indicated number of steps
                else{
                    totalStepsTaken += determineSafeDistance(sr, sc, newDir, planSteps);
                    nsteps = totalStepsTaken;        //number of successful steps before crash
                    return 3;}
                }
            }
        }
    
    nsteps = totalStepsTaken;           //total steps taken after completing the whole plan
    
    if ((sr == er) && (sc == ec))       //check if we reached endpoint or not
        return 0;
    else
        return 1;

}


