//
//  main.cpp
//  Ataxx
//
//  Original Author：zhouhy zys
//  Created by MarkYu on 16/11/4.
//  Version 0.5 - Random decision
//  http://www.botzone.org/games#Ataxx
//  Copyright © 2016年 MarkYu. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "jsoncpp/json.h"

using namespace std;

int currBotColor; // The color of current Bot: 1 for Black, -1 for White.
int gridInfo[7][7] = {0}; // Board Info

void Update(int x, int y, int color)
{
    if (x > 0) {
        if (gridInfo[x-1][y] == -color) gridInfo[x-1][y] = color;
        if (y > 0 && gridInfo[x-1][y-1] == -color) gridInfo[x-1][y-1] = color;
        if (y < 6 && gridInfo[x-1][y+1] == -color) gridInfo[x-1][y+1] = color;
    }
    if (y > 0 && gridInfo[x][y-1] == -color) gridInfo[x][y-1] = color;
    if (y < 6 && gridInfo[x][y+1] == -color) gridInfo[x][y+1] = color;
    if (x < 6) {
        if (gridInfo[x+1][y] == -color) gridInfo[x+1][y] = color;
        if (y > 0 && gridInfo[x+1][y-1] == -color) gridInfo[x+1][y-1] = color;
        if (y < 6 && gridInfo[x+1][y+1] == -color) gridInfo[x+1][y+1] = color;
    }
}

// Move (x0, y0) -> (x1, y1)
void ProcStep(int x0, int y0, int x1, int y1, int color)
{
    if (abs(x1-x0) <= 1 && abs(y1-y0) <= 1) {
        gridInfo[x1][y1] = color;
        Update(x1, y1, color);
    }
    else {
        gridInfo[x0][y0] = 0;
        gridInfo[x1][y1] = color;
        Update(x1, y1, color);
    }
}

int main()
{
    int x0, y0, x1, y1;
    
    // Initialization
    gridInfo[0][0] = gridInfo[6][6] = 1;
    gridInfo[6][0] = gridInfo[0][6] = -1;
    
    // JSON read
    string str;
    getline(cin, str);
    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);
    
    // Recover Board Status
    int turnID = input["responses"].size();
    currBotColor = input["requests"][(Json::Value::UInt) 0]["x0"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
    for (int i = 0; i < turnID; i++)
    {
        x0 = input["requests"][i]["x0"].asInt();
        y0 = input["requests"][i]["y0"].asInt();
        x1 = input["requests"][i]["x1"].asInt();
        y1 = input["requests"][i]["y1"].asInt();
        if (x0 >= 0)
            ProcStep(x0, y0, x1, y1, -currBotColor); // Opponent's move
        x0 = input["responses"][i]["x0"].asInt();
        y0 = input["responses"][i]["y0"].asInt();
        x1 = input["responses"][i]["x1"].asInt();
        y1 = input["responses"][i]["y1"].asInt();
        if (x0 >= 0)
            ProcStep(x0, y0, x1, y1, currBotColor); // Bot's move
    }
    
    // Current input
    x0 = input["requests"][turnID]["x0"].asInt();
    y0 = input["requests"][turnID]["y0"].asInt();
    x1 = input["requests"][turnID]["x1"].asInt();
    y1 = input["requests"][turnID]["y1"].asInt();
    if (x0 >= 0)
        ProcStep(x0, y0, x1, y1, -currBotColor); // Opponent's move
    
    // Making decisions among legal moves
    
    int startX, startY, resultX, resultY;
    int legalmoves = 0, moveInfo[1200][4] = {0};
    
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (gridInfo[i][j] == currBotColor) {
                if (i > 0) {
                    if (gridInfo[i-1][j] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-1; moveInfo[legalmoves][3] = j;
                        legalmoves++;
                    }
                    if (j > 0 && gridInfo[i-1][j-1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-1; moveInfo[legalmoves][3] = j-1;
                        legalmoves++;
                    }
                    if (j < 6 && gridInfo[i-1][j+1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-1; moveInfo[legalmoves][3] = j+1;
                        legalmoves++;
                    }
                }
                if (j > 0 && gridInfo[i][j-1] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i; moveInfo[legalmoves][3] = j-1;
                    legalmoves++;
                }
                if (j < 6 && gridInfo[i][j+1] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i; moveInfo[legalmoves][3] = j+1;
                    legalmoves++;
                }
                if (i < 6) {
                    if (gridInfo[i+1][j] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+1; moveInfo[legalmoves][3] = j;
                        legalmoves++;
                    }
                    if (j > 0 && gridInfo[i+1][j-1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+1; moveInfo[legalmoves][3] = j-1;
                        legalmoves++;
                    }
                    if (j < 6 && gridInfo[i+1][j+1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+1; moveInfo[legalmoves][3] = j+1;
                        legalmoves++;
                    }
                }
                if (i > 1) {
                    if (gridInfo[i-2][j] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-2; moveInfo[legalmoves][3] = j;
                        legalmoves++;
                    }
                    if (j > 0 && gridInfo[i-2][j-1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-2; moveInfo[legalmoves][3] = j-1;
                        legalmoves++;
                    }
                    if (j > 1 && gridInfo[i-2][j-2] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-2; moveInfo[legalmoves][3] = j-2;
                        legalmoves++;
                    }
                    if (j < 6 && gridInfo[i-2][j+1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-2; moveInfo[legalmoves][3] = j+1;
                        legalmoves++;
                    }
                    if (j < 5 && gridInfo[i-2][j+2] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-2; moveInfo[legalmoves][3] = j+2;
                        legalmoves++;
                    }
                }
                if (j > 1 && i > 0 && gridInfo[i-1][j-2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-1; moveInfo[legalmoves][3] = j-2;
                    legalmoves++;
                }
                if (j > 1 && i < 6 && gridInfo[i+1][j-2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+1; moveInfo[legalmoves][3] = j-2;
                    legalmoves++;
                }
                if (j > 1 && gridInfo[i][j-2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i; moveInfo[legalmoves][3] = j-2;
                    legalmoves++;
                }
                if (j < 5 && i > 0 && gridInfo[i-1][j+2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i-1; moveInfo[legalmoves][3] = j+2;
                    legalmoves++;
                }
                if (j < 5 && i < 6 && gridInfo[i+1][j+2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+1; moveInfo[legalmoves][3] = j+2;
                    legalmoves++;
                }
                if (j < 5 && gridInfo[i][j+2] == 0) {
                    moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i; moveInfo[legalmoves][3] = j+2;
                    legalmoves++;
                }
                if (i < 5) {
                    if (gridInfo[i+2][j] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+2; moveInfo[legalmoves][3] = j;
                        legalmoves++;
                    }
                    if (j > 0 && gridInfo[i+2][j-1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+2; moveInfo[legalmoves][3] = j-1;
                        legalmoves++;
                    }
                    if (j > 1 && gridInfo[i+2][j-2] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+2; moveInfo[legalmoves][3] = j-2;
                        legalmoves++;
                    }
                    if (j < 6 && gridInfo[i+2][j+1] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+2; moveInfo[legalmoves][3] = j+1;
                        legalmoves++;
                    }
                    if (j < 5 && gridInfo[i+2][j+2] == 0) {
                        moveInfo[legalmoves][0] = i; moveInfo[legalmoves][1] = j; moveInfo[legalmoves][2] = i+2; moveInfo[legalmoves][3] = j+2;
                        legalmoves++;
                    }
                }
            }
    srand((unsigned)time(NULL));
    int moveID = rand() % legalmoves;
    startX = moveInfo[moveID][0]; startY = moveInfo[moveID][1]; resultX = moveInfo[moveID][2]; resultY = moveInfo[moveID][3];
    // Output outcome
    
    Json::Value ret;
    ret["response"]["x0"] = startX;
    ret["response"]["y0"] = startY;
    ret["response"]["x1"] = resultX;
    ret["response"]["y1"] = resultY;
    Json::FastWriter writer;
    cout << writer.write(ret) << endl;
    return 0;
}
