#ifndef AI_H
#define AI_H

struct Moves {
    int sx, sy, ex, ey;
};

class AI
{
private:
    int LegalMove(int x0, int y0, int x1, int y1, int g[][7]);
    void Update(int x, int y, int color, int g[][7]);
    void ProcStep(int x0, int y0, int x1, int y1, int color, int g[][7]);
    void Copy(int g1[][7], int g2[][7]);
    int MaxDepth;
    bool Moveable(int g[][7], int color);
    int GetNum(int g[][7], int color);
    int isWin(int g[][7], int color);
    int MinMax(int depth, int color, int mode);
    int AlphaBeta(int depth, int alpha, int beta, int color, int mode);
    Moves Decision;

public:
    int gInfo[20][7][7];
    void MakeDecision(int &startX, int &startY, int &resultX, int &resultY, int color, int g[][7], int mode);
    int Value(int g[][7], int color, int mode);
    int AIcolor;
};

#endif // AI_H
