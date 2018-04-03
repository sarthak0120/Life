//This is the Game of Life. It creates  a simulation of the the lifecycles of colonies of bacteria.
//The program includes functionality like ticking ad animating.
//EXTRA: Produces random worlds when filename is set to "random". (Function: random(), Line 92)

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

Grid<char> random();
Grid<char> getGrid(string filename);
void print(Grid<char> &grid);
void tick(Grid<char> &grid, char wrap);
void animate(Grid<char> &grid, char wrap);
int countNeighbours(Grid<char> &grid, int row, int col, char wrap);


int main() {

    cout<<"Welcome to the CS 106B Game of Life,\na simulation of the lifecycle of a bacteria colony.\n";
    cout<<"Cells (X) live and die by the following rules:\n";
    cout<<"- A cell with 1 or fewer neighbors dies.\n";
    cout<<"- Locations with 2 neighbors remain stable.\n";
    cout<<"- Locations with 3 neighbors will create life.\n";
    cout<<"- A cell with 4 or more neighbors dies.\n\n";

    string filename = getLine("Grid input file name? ");
    Grid<char> grid;
    if (filename=="random"){
        grid= random(); //Produces a random world!
    }

    else{

        if(!fileExists(filename)){

            do{ // Repeated prompt until user enters a valid file name.

                filename = getLine("Unable to open that file. Try again.\nGrid input file name? ");
                if  (filename=="random"){
                    grid=random();
                    break;
                }
            } while (!fileExists(filename));
        }
        if(filename!="random"){
            grid= getGrid(filename); //Gets a grid from file.
        }
     }

    char wrap;
    wrap = getLine("Should the simulation wrap around the grid (y/n)? ")[0];    //Takes the first letter if user enters a word.
    print(grid);

    string choice;    // User's entered value.
    char ch;          // Used to store relevant part of that value.
    do{
        choice= getLine("a)nimate, t)ick, q)uit? ");
        ch= choice[0];
        if (choice.length()!=1 || (ch!='a' && ch!='A' && ch!='t' && ch!='T' && ch!='q' && ch!='Q')){    //Handles invalid entries
            do{
                cout<<"Invalid choice; please try again.\na)nimate, t)ick, q)uit? ";
                choice=getLine();
                ch= choice[0];
            } while(choice.length()!=1 || (ch!='a' && ch!='A' && ch!='t' && ch!='T' && ch!='q' && ch!='Q'));
        }
        switch(ch){

        case 'a':
        case 'A': animate(grid, wrap); break;
        case 't':
        case 'T': tick(grid,wrap); break;
        case 'q':
        case 'Q': cout<<"Have a nice Life!"; break; //Exit statement
        }
    } while(ch!='q' && ch!='Q');

    return 0;
}

Grid<char> random(){

    int rows=randomInteger(1, 25);
    int cols=randomInteger(1, 25);
    Grid<char> grid(rows, cols);
    for (int i=0; i<rows; i++){     // Storing incoming lines of text letter by letter
        for (int j=0; j<cols; j++){
            int random= randomInteger(0,1);
            if (random==0){
                grid[i][j]='-';
            }
            else {
                grid[i][j]='X';
            }
        }
    }

    return grid;
}

Grid<char> getGrid(string filename){

    fstream stream;
    string line;
    stream.open(filename, ios::in);
    stream>>line;
    int rows= stringToInteger(line);
    stream>>line;
    int cols = stringToInteger(line);

    Grid<char> grid(rows,cols);
    for (int i=0; i<rows; i++){     // Storing incoming lines of text letter by letter
        string content;             // into the grid
        stream>>content;
        for (int j=0; j<cols; j++){
            grid[i][j]= content[j];
        }

    }
    stream.close();
    return(grid);
}

void print(Grid<char> &grid){

    for (int i=0; i<grid.numRows(); i++){
        for (int j=0; j<grid.numCols(); j++){
            cout<<grid[i][j];
        }
        cout<<endl;
    }
}

void tick(Grid<char> &grid, char wrap){

    Grid<char> temp(grid.numRows(),grid.numCols());     //A temporary grid copied later to actual grid.
    Grid<int> neighbours(grid.numRows(), grid.numCols());   //Stores no. of neighbors for each cell of actual grid.

    for (int i=0; i<neighbours.numRows(); i++){
        for (int j=0; j<neighbours.numCols(); j++){
            neighbours[i][j]= countNeighbours(grid, i, j, wrap);
        }
    }

    for (int i=0; i<grid.numRows(); i++){

        for (int j=0; j< grid.numCols(); j++){      //Filling the grid temp according to the rules of the game

            if(neighbours[i][j]==0 || neighbours[i][j]==1){
                temp[i][j]='-';
            }
            else if (neighbours[i][j]==2){

                temp[i][j]=grid[i][j];
            }
            else if (neighbours[i][j]==3){

                temp[i][j]='X';
            }
            else {

                temp[i][j]='-';
            }
        }
    }

    grid=temp;  //Copying the contents of temp to grid
    print(grid);
}

void animate(Grid<char> &grid, char wrap){

    string frames; //Number of frames. Made as string so as to check if every character is a digit.
    int i, flag;
    do{
        frames=getLine("How many frames? ");
        i=0;
        flag=1;
        while(i<frames.length()){
            if (frames[i]<48 || frames[i]>57){
                flag=0; // Sets flag =0 if any character in the string is not a digit.
                break;
            }
            i++;
        }
        if (flag==0){
            cout<<"Illegal integer format. Try again."<<endl;
        }
    } while(flag==0);

    int intframes= stringToInteger(frames);

    for (i=0; i<intframes; i++){    //Loops over the tick function intframes number of times
        clearConsole();             //thereby resulting in animation effect
        tick(grid, wrap);
        pause(50);      //50ms pause (Can be changed for faster animation)
    }


}

int countNeighbours(Grid<char> &grid, int row, int col, char wrap){

    int count=0;

    if (wrap=='y'){     //If user wants wrapping to be done.

        if (grid[row][col-1<0?col-1 + grid.numCols(): col-1]=='X'){
            count++;
        }
        if (grid[row][col+1>grid.numCols()-1?(col+1)%grid.numCols():col+1]=='X'){
            count++;
        }
        if (grid[row-1<0?row-1 + grid.numRows():row-1][col]=='X'){
            count++;
        }
        if (grid[row+1>grid.numRows()-1?(row+1)%grid.numRows():row+1][col]=='X'){
            count++;
        }
        if (grid[row-1<0?row-1 + grid.numRows():row-1][col-1<0?col-1 + grid.numCols(): col-1]=='X'){
            count++;
        }
        if (grid[row-1<0?row-1 + grid.numRows():row-1][col+1>grid.numCols()-1?(col+1)%grid.numCols():col+1]=='X'){
            count++;
        }
        if (grid[row+1>grid.numRows()-1?(row+1)%grid.numRows():row+1][col-1<0?col-1 + grid.numCols(): col-1]=='X'){
            count++;
        }
        if (grid[row+1>grid.numRows()-1?(row+1)%grid.numRows():row+1][col+1>grid.numCols()-1?(col+1)%grid.numCols():col+1]=='X'){
            count++;
        }
        return count;
    }

    else{    //if user does not want wrapping.
        if(row-1<0 && col-1<0){

            if(grid[row][col+1]=='X') count++;
            if(grid[row+1][col+1]=='X') count++;
            if(grid[row+1][col]=='X') count++;
        }
        else if(row-1<0 && col+1>grid.numCols()-1){

            if(grid[row][col-1]=='X') count++;
            if(grid[row+1][col-1]=='X') count++;
            if(grid[row+1][col]=='X') count++;
        }
        else if(row+1>grid.numRows()-1 && col+1>grid.numCols()-1){

            if(grid[row-1][col]=='X') count++;
            if(grid[row-1][col-1]=='X') count++;
            if(grid[row][col-1]=='X') count++;

        }
        else if(row+1>grid.numRows()-1 && col-1<0){

            if(grid[row][col+1]=='X') count++;
            if(grid[row-1][col+1]=='X') count++;
            if(grid[row-1][col]=='X') count++;

        }
        else if(row-1<0){
            if (grid[row][col+1]=='X') count++;
            if (grid[row][col-1]=='X') count++;
            if (grid[row+1][col+1]=='X') count++;
            if (grid[row+1][col]=='X') count++;
            if (grid[row+1][col-1]=='X') count++;
        }
        else if(col+1>grid.numCols()-1){
            if (grid[row-1][col-1]=='X') count++;
            if (grid[row-1][col]=='X') count++;
            if (grid[row][col-1]=='X') count++;
            if (grid[row+1][col]=='X') count++;
            if (grid[row+1][col-1]=='X') count++;
        }
        else if(row+1>grid.numRows()-1){
            if (grid[row][col+1]=='X') count++;
            if (grid[row][col-1]=='X') count++;
            if (grid[row-1][col+1]=='X') count++;
            if (grid[row-1][col]=='X') count++;
            if (grid[row-1][col-1]=='X') count++;
        }
        else if(col-1<0){
            if (grid[row-1][col]=='X') count++;
            if (grid[row-1][col+1]=='X') count++;
            if (grid[row][col+1]=='X') count++;
            if (grid[row+1][col]=='X') count++;
            if (grid[row+1][col+1]=='X') count++;
        }
        else{

            if(grid[row-1][col-1]=='X') count++;
            if(grid[row-1][col]=='X') count++;
            if(grid[row-1][col+1]=='X') count++;
            if(grid[row][col-1]=='X') count++;
            if(grid[row][col+1]=='X') count++;
            if(grid[row+1][col-1]=='X') count++;
            if(grid[row+1][col]=='X') count++;
            if(grid[row+1][col+1]=='X') count++;
        }
        return count;
    }

}

