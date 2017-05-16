//
//  main.cpp
//  Ataxx
//
//  Original Author：zhouhy zys
//  Created by MarkYu on 16/12/30.
//  Version 2.2 - MinMax Search 3
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
int tInfo[5][7][7] = {0};
const int MaxDepth = 3, INF = 0xfffffff;

struct Moves {
    int sx, sy, ex, ey;
}Decision;

int LegalMove(int x0, int y0, int x1, int y1, int gInfo[][7])
{
    if (x1 < 0 || y1 < 0 || x1 > 6 || y1 > 6) return 0;
    if (x1 == x0 && y1 == y0) return 0;
    if (gInfo[x1][y1] != 0) return 0;
    if (abs(x0-x1) <= 1 && abs(y0-y1) <= 1) return 1;
    return 2;
}

void Update(int x, int y, int color, int gInfo[][7])
{
    if (x > 0) {
        if (gInfo[x-1][y] == -color) gInfo[x-1][y] = color;
        if (y > 0 && gInfo[x-1][y-1] == -color) gInfo[x-1][y-1] = color;
        if (y < 6 && gInfo[x-1][y+1] == -color) gInfo[x-1][y+1] = color;
    }
    if (y > 0 && gInfo[x][y-1] == -color) gInfo[x][y-1] = color;
    if (y < 6 && gInfo[x][y+1] == -color) gInfo[x][y+1] = color;
    if (x < 6) {
        if (gInfo[x+1][y] == -color) gInfo[x+1][y] = color;
        if (y > 0 && gInfo[x+1][y-1] == -color) gInfo[x+1][y-1] = color;
        if (y < 6 && gInfo[x+1][y+1] == -color) gInfo[x+1][y+1] = color;
    }
}

// Move (x0, y0) -> (x1, y1)
void ProcStep(int x0, int y0, int x1, int y1, int color, int gInfo[][7])
{
    if (abs(x1-x0) <= 1 && abs(y1-y0) <= 1) {
        gInfo[x1][y1] = color;
        Update(x1, y1, color, gInfo);
    }
    else {
        gInfo[x0][y0] = 0;
        gInfo[x1][y1] = color;
        Update(x1, y1, color, gInfo);
    }
}
int checknear(int g[][7], int x, int y, int color)
{
    int v1, v2, v3;
    v1 = 10; v2 = 40; v3 = 100;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            if (x+i < 0 || x+i > 6 || y+j < 0 || y+j > 6) continue;
            if (g[x+i][y+j] == 0) {
                for (int k = -1; k <= 1; k++)
                    for (int m = -1; m <= 1; m++) {
                        if (x+i+k < 0 || x+i+k > 6 || y+j+m < 0 || y+j+m > 6) continue;
                        if (g[x+i+k][y+j+m] == -color) return v1;
                    }
                for (int k = -2; k <= 2; k++)
                    for (int m = -2; m <= 2; m++) {
                        if (abs(k) != 2 && abs(m) != 2) continue;
                        if (x+i+k < 0 || x+i+k > 6 || y+j+m < 0 || y+j+m > 6) continue;
                        if (g[x+i+k][y+j+m] == -color) return v2;
                    }
            }
        }
    return v3;
}

int Value(int g[][7], int color)
{
    int num = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (g[i][j] == color)
                num += checknear(g, i, j, color);
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (g[i][j] == -color)
                num -= checknear(g, i, j, -color);
    return num;
}

void Copy(int g1[][7], int g2[][7])
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            g2[i][j] = g1[i][j];
}

bool Moveable(int g[][7], int color)
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (g[i][j] == color)
            {
                for (int ki = 0; ki < 7; ki++)
                    for (int kj = 0; kj < 7; kj++)
                        if (LegalMove(i, j, ki, kj, g))
                            return true;
            }
    return false;
}

int GetNum(int g[][7], int color)
{
    int s = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (g[i][j] == color)
                s++;
    return s;
}

int isWin(int g[][7], int color)
{
    if (!Moveable(g, 1) || !Moveable(g, -1))
    {
        if (Moveable(g, 1) && GetNum(g, 1) > 0 && GetNum(g, -1) > 0)
        {
            int tmp = 0;
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 7; j++)
                    if (g[i][j] == 0) tmp++;
            if (GetNum(g, 1)+tmp > GetNum(g, -1)) return color == 1 ? 1 : -1;
            else if (GetNum(g, 1)+tmp < GetNum(g, -1)) return color == -1 ? 1 : -1;
        }
        else if (Moveable(g, -1) && GetNum(g, 1) > 0 && GetNum(g, -1) > 0)
        {
            int tmp = 0;
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 7; j++)
                    if (g[i][j] == 0) tmp++;
            if (GetNum(g, 1) > GetNum(g, -1)+tmp) return color == 1 ? 1 : -1;
            else if (GetNum(g, 1) < GetNum(g, -1)+tmp) return color == -1 ? 1 : -1;
        }
        else
        {
            if (GetNum(g, 1) > GetNum(g, -1)) return color == 1 ? 1 : -1;
            else if (GetNum(g, 1) < GetNum(g, -1)) return color == -1 ? 1 : -1;
        }
    }
    return 0;
}

int MinMax(int depth, int color)
{
    int best = -INF, val;
    bool checker[7][7] = {{0}};
    if (isWin(tInfo[depth], color) == 1) return INF-1;
    else if (isWin(tInfo[depth], color) == -1) return -INF+1;
    if (depth >= MaxDepth) return Value(tInfo[depth], color);
    vector<Moves> mlist;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (tInfo[depth][i][j] == color) {
                for (int k1 = i-2; k1 <= i+2; k1++)
                    for (int k2 = j-2; k2 <= j+2; k2++) {
                        if (LegalMove(i, j, k1, k2, tInfo[depth]) == 2) {
                            Moves m;
                            m.sx = i; m.sy = j; m.ex = k1; m.ey = k2;
                            mlist.push_back(m);
                        }
                        else if (LegalMove(i, j, k1, k2, tInfo[depth]) == 1) {
                            if (checker[k1][k2]) continue;
                            Moves m;
                            m.sx = i; m.sy = j; m.ex = k1; m.ey = k2;
                            mlist.push_back(m);
                            checker[k1][k2] = true;
                        }
                    }
            }
        }
    }
    for (vector<Moves>::iterator it = mlist.begin(); it != mlist.end(); it++) {
        Copy(tInfo[depth], tInfo[depth+1]);
        ProcStep((*it).sx, (*it).sy, (*it).ex, (*it).ey, color, tInfo[depth+1]);
        val = -MinMax(depth+1, -color);
        if (val > best) {
            best = val;
            if (depth == 0) Decision = (*it);
        }
    }
    return best;
}

void MakeDecision(int &startX, int &startY, int &resultX, int &resultY, int color, int gInfo[][7])
{
    Copy(gInfo, tInfo[0]);
    int best;
    best = MinMax(0, color);
    startX = Decision.sx; startY = Decision.sy;
    resultX = Decision.ex; resultY = Decision.ey;
    return;
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
            ProcStep(x0, y0, x1, y1, -currBotColor, gridInfo); // Opponent's move
        x0 = input["responses"][i]["x0"].asInt();
        y0 = input["responses"][i]["y0"].asInt();
        x1 = input["responses"][i]["x1"].asInt();
        y1 = input["responses"][i]["y1"].asInt();
        if (x0 >= 0)
            ProcStep(x0, y0, x1, y1, currBotColor, gridInfo); // Bot's move
    }
    
    // Current input
    x0 = input["requests"][turnID]["x0"].asInt();
    y0 = input["requests"][turnID]["y0"].asInt();
    x1 = input["requests"][turnID]["x1"].asInt();
    y1 = input["requests"][turnID]["y1"].asInt();
    if (x0 >= 0)
        ProcStep(x0, y0, x1, y1, -currBotColor, gridInfo); // Opponent's move
    
    // Making decisions among legal moves
    
    int startX, startY, resultX, resultY;
    MakeDecision(startX, startY, resultX, resultY, currBotColor, gridInfo);
    
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
