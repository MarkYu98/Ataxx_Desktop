#include "AI.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <QDebug>

const int INF = 0xfffffff;

int abs(int x)
{
    return x < 0 ? -x : x;
}

int checknear(int g[][7], int x, int y, int color, int mode)
{
    int v1, v2, v3;
    if (mode == 3) {v1 = 10; v2 = 40; v3 = 100;}
    if (mode == 4) {v1 = 10; v2 = 40; v3 = 100;}
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

int AI::MinMax(int depth, int color, int mode)
{
    int best = -INF, val;
    bool checker[7][7] = {{0}};
    if (isWin(gInfo[depth], color) == 1) return INF-1;
    else if (isWin(gInfo[depth], color) == -1) return -INF+1;
    if (depth >= MaxDepth) return Value(gInfo[depth], color, mode);
    std::vector<Moves> mlist;
    for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (gInfo[depth][i][j] == color) {
                    for (int k1 = i-2; k1 <= i+2; k1++)
                        for (int k2 = j-2; k2 <= j+2; k2++) {
                            if (LegalMove(i, j, k1, k2, gInfo[depth]) == 2) {
                                Moves m;
                                m.sx = i; m.sy = j; m.ex = k1; m.ey = k2;
                                mlist.push_back(m);
                            }
                            else if (LegalMove(i, j, k1, k2, gInfo[depth]) == 1) {
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
    for (std::vector<Moves>::iterator it = mlist.begin(); it != mlist.end(); it++) {
        Copy(gInfo[depth], gInfo[depth+1]);
        ProcStep((*it).sx, (*it).sy, (*it).ex, (*it).ey, color, gInfo[depth+1]);
        val = -MinMax(depth+1, -color, mode);
        if (val > best) {
            best = val;
            if (depth == 0) Decision = (*it);
        }
    }
    return best;
}

int AI::AlphaBeta(int depth, int alpha, int beta, int color, int mode)
{
    int v4 = 50;
    bool checker[7][7] = {{0}};
    if (isWin(gInfo[depth], color) == 1) return INF-1;
    else if (isWin(gInfo[depth], color) == -1) return -INF+1;
    if (depth >= MaxDepth) return Value(gInfo[depth], color, mode) + rand()%v4;
    std::vector<Moves> mlist;
    for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (gInfo[depth][i][j] == color) {
                    for (int k1 = i-2; k1 <= i+2; k1++)
                        for (int k2 = j-2; k2 <= j+2; k2++) {
                            if (LegalMove(i, j, k1, k2, gInfo[depth]) == 2) {
                                Moves m;
                                m.sx = i; m.sy = j; m.ex = k1; m.ey = k2;
                                mlist.push_back(m);
                            }
                            else if (LegalMove(i, j, k1, k2, gInfo[depth]) == 1) {
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
    int val;
    if (color == AIcolor) {
        for (std::vector<Moves>::iterator it = mlist.begin(); it != mlist.end(); it++) {
            Copy(gInfo[depth], gInfo[depth+1]);
            ProcStep((*it).sx, (*it).sy, (*it).ex, (*it).ey, color, gInfo[depth+1]);
            val = AlphaBeta(depth+1, alpha, beta, -color, mode);
            if (val > alpha) {
                alpha = val;
                if (depth == 0) Decision = (*it);
            }
            if (beta <= alpha) break;
        }
        return alpha;
    }
    else {
        for (std::vector<Moves>::iterator it = mlist.begin(); it != mlist.end(); it++) {
            Copy(gInfo[depth], gInfo[depth+1]);
            ProcStep((*it).sx, (*it).sy, (*it).ex, (*it).ey, color, gInfo[depth+1]);
            val = AlphaBeta(depth+1, alpha, beta, -color, mode);
            if (val < beta)
                beta = val;
            if (beta <= alpha) break;
        }
        return beta;
    }
}

int AI::LegalMove(int x0, int y0, int x1, int y1, int g[][7])
{
    if (x1 < 0 || y1 < 0 || x1 > 6 || y1 > 6) return 0;
    if (x1 == x0 && y1 == y0) return 0;
    if (g[x1][y1] != 0) return 0;
    if (abs(x0-x1) <= 1 && abs(y0-y1) <= 1) return 1;
    return 2;
}

void AI::Update(int x, int y, int color, int g[][7])
{
    if (x > 0) {
        if (g[x-1][y] == -color) g[x-1][y] = color;
        if (y > 0 && g[x-1][y-1] == -color) g[x-1][y-1] = color;
        if (y < 6 && g[x-1][y+1] == -color) g[x-1][y+1] = color;
    }
    if (y > 0 && g[x][y-1] == -color) g[x][y-1] = color;
    if (y < 6 && g[x][y+1] == -color) g[x][y+1] = color;
    if (x < 6) {
        if (g[x+1][y] == -color) g[x+1][y] = color;
        if (y > 0 && g[x+1][y-1] == -color) g[x+1][y-1] = color;
        if (y < 6 && g[x+1][y+1] == -color) g[x+1][y+1] = color;
    }
}

// Move (x0, y0) -> (x1, y1)
void AI::ProcStep(int x0, int y0, int x1, int y1, int color, int g[][7])
{
    if (abs(x1-x0) <= 1 && abs(y1-y0) <= 1) {
        g[x1][y1] = color;
        Update(x1, y1, color, g);
    }
    else {
        g[x0][y0] = 0;
        g[x1][y1] = color;
        Update(x1, y1, color, g);
    }
}

void AI::Copy(int g1[][7], int g2[][7])
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            g2[i][j] = g1[i][j];
}

bool AI::Moveable(int g[][7], int color)
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

int AI::GetNum(int g[][7], int color)
{
    int s = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (g[i][j] == color)
                s++;
    return s;
}

int AI::isWin(int g[][7], int color)
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

int AI::Value(int g[][7], int color, int mode)
{
    if (mode == 1) return 0;
    if (mode == 2)
    {
        int num = 0;
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if (g[i][j] == color)
                    num++;
        return num;
    }
    if (mode == 3)
    {
        int num = 0;
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if (g[i][j] == color)
                    num += checknear(g, i, j, color, mode);
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if (g[i][j] == -color)
                    num -= checknear(g, i, j, -color, mode);
        return num;
    }
    if (mode == 4)
    {
        int num = 0;
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if (g[i][j] == color)
                    num += checknear(g, i, j, color, mode);
        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if (g[i][j] == -color)
                    num -= checknear(g, i, j, -color, mode);
        return num;
    }
    return 0;
}

void AI::MakeDecision(int &startX, int &startY, int &resultX, int &resultY, int color, int g[][7], int mode)
{
    if (mode == 1) //Retard
    {
        int movenum = 0;
        for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 7; j++) {
                    if (g[i][j] == color) {
                        for (int k1 = i-2; k1 <= i+2; k1++)
                            for (int k2 = j-2; k2 <= j+2; k2++)
                                if (LegalMove(i, j, k1, k2, g)) {
                                    movenum++;
                                }
                    }
                }
        }
        srand(time(NULL));
        int moveid = rand()%movenum+1, tmp = 0;
        for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 7; j++) {
                    if (g[i][j] == color) {
                        for (int k1 = i-2; k1 <= i+2; k1++)
                            for (int k2 = j-2; k2 <= j+2; k2++) {
                                if (LegalMove(i, j, k1, k2, g)) {
                                    tmp++;
                                    if (tmp == moveid) {
                                        startX = i; startY = j; resultX = k1; resultY = k2;
                                        return;
                                    }
                                }
                            }
                    }
                }
        }
        return;
    }
    if (mode == 2) //Simple
    {
        int mmax = 0;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (g[i][j] == color) {
                    for (int k1 = i-2; k1 <= i+2; k1++)
                        for (int k2 = j-2; k2 <= j+2; k2++)
                            if (LegalMove(i, j, k1, k2, g)) {
                                Copy(g, gInfo[1]);
                                ProcStep(i, j, k1, k2, color, gInfo[1]);
                                if (mmax < Value(gInfo[1], color, mode)) {
                                    mmax = Value(gInfo[1], color, mode);
                                    startX = i; startY = j; resultX = k1; resultY = k2;
                                }
                            }
                }
            }
        }
        return;
    }
    if (mode == 3)
    {
        Copy(g, gInfo[0]);
        MaxDepth = 3;
        int best;
        best = MinMax(0, color, mode);
        startX = Decision.sx; startY = Decision.sy;
        resultX = Decision.ex; resultY = Decision.ey;
        return;
    }
    if (mode == 4)
    {
        srand(time(NULL));
        Copy(g, gInfo[0]);
        MaxDepth = 4;
        int best;
        int alpha = -INF, beta = INF;
        best = AlphaBeta(0, alpha, beta, color, mode);
        startX = Decision.sx; startY = Decision.sy;
        resultX = Decision.ex; resultY = Decision.ey;
        return;
    }
}
