#include<cmath>
#include<vector>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include<sstream>
#include<string>
#include<graphics.h>
#include<conio.h>
using namespace std;
#define EGERGBA(r,g,b,a) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))
const int GRIDSIZE = 8;
int gridInfo[GRIDSIZE][GRIDSIZE];
const int grid_black = 1;
const int grid_white = -1;
const int obstacle = 2;
int currentcolor, rivalcolor;
int mycolor; // 人机模式下，玩家执子颜色
int dx[8] = { -1,-1,-1,0,0,1,1,1 };
int dy[8] = { -1,0,1,-1,1,-1,0,1 };
int hoverRow = -1, hoverCol = -1;
// 棋盘左上角坐标和格子大小
const int BOARD_X = 50, BOARD_Y = 50, CELL_SIZE = 60;
// 画棋盘
void drawChessBoard() {
    //COLORREF light = RGB(240, 217, 181); // 浅色
    //COLORREF dark = RGB(181, 136, 99);  // 深色
    //for (int i = 0; i < GRIDSIZE; ++i) {
    //    for (int j = 0; j < GRIDSIZE; ++j) {
    //        setfillcolor(((i + j) % 2 == 0) ? light : dark);
    //        solidrectangle(
    //            BOARD_X + j * CELL_SIZE,
    //            BOARD_Y + i * CELL_SIZE,
    //            BOARD_X + (j + 1) * CELL_SIZE,
    //            BOARD_Y + (i + 1) * CELL_SIZE
    //        );
    //    }
    //}
    COLORREF woodColors[4] = {
    RGB(222, 184, 135), // 浅棕
    RGB(205, 133, 63),  // 中棕
    RGB(210, 180, 140), // 浅黄棕
    RGB(160, 82, 45)    // 深棕
    };
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            int x1 = BOARD_X + j * CELL_SIZE;
            int y1 = BOARD_Y + i * CELL_SIZE;
            int x2 = x1 + CELL_SIZE;
            int y2 = y1 + CELL_SIZE;
            // 先画底色
            setfillcolor(woodColors[(i + j) % 2]);
            solidrectangle(x1, y1, x2, y2);
            // 叠加几条椭圆模拟木纹
            for (int k = 0; k < 3; ++k) {
                setlinecolor(woodColors[(i + j + k + 1) % 4]);
                ellipse(x1 + 5 + k * 6, y1 + 5 + k * 6, x2 - 5 - k * 6, y2 - 5 - k * 6);
            }
        }
    }
    // 绘制外边框
    setlinecolor(BLUE); // 边框颜色（深棕色或你喜欢的色）
    setlinestyle(PS_SOLID, 8);
roundrect(
    BOARD_X, BOARD_Y,
    BOARD_X + GRIDSIZE * CELL_SIZE,
    BOARD_Y + GRIDSIZE * CELL_SIZE,
    20, 20 // 圆角半径
);
setlinestyle(PS_SOLID, 1); // 恢复线宽
    setlinecolor(BLACK);
}
// 画棋子和障碍
void drawPieces() {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            int cx = BOARD_X + j * CELL_SIZE + CELL_SIZE / 2;
            int cy = BOARD_Y + i * CELL_SIZE + CELL_SIZE / 2;
            if (gridInfo[i][j] == grid_black) {
                if (i == hoverRow && j == hoverCol && gridInfo[i][j] == currentcolor) {
                    setfillcolor(EGERGBA(255, 255, 0, 128)); //128为透明度，0完全透明，255不透明
                    solidcircle(cx, cy, CELL_SIZE / 2 - 2);
                    setlinecolor(RGB(255, 215, 0)); // 金色粗边
                    setlinestyle(PS_SOLID, 4);
                    circle(cx, cy, CELL_SIZE / 2 - 3);
                    setlinestyle(PS_SOLID, 1);
                    setlinecolor(BLACK);
                }
                else {
                    setlinecolor(BLUE);
                    setfillcolor(BLACK);
                    // 棋子边框
                    setlinestyle(PS_SOLID, 4);      // 边框粗细
                    circle(cx, cy, CELL_SIZE / 2 - 5);
                    fillcircle(cx, cy, CELL_SIZE / 2 - 5);
                    setlinecolor(BLACK);
                    setlinestyle(PS_SOLID, 1); // 恢复线宽
                }
            }
            else if (gridInfo[i][j] == grid_white) {
                if (i == hoverRow && j == hoverCol && gridInfo[i][j] == currentcolor) {
                    //setfillcolor(RGB(255, 255, 0)); // 黄色高亮底
                    //solidcircle(cx, cy, CELL_SIZE / 2 - 2);
                    setfillcolor(EGERGBA(255, 255, 0, 128)); //128为透明度，0完全透明，255不透明
                    solidcircle(cx, cy, CELL_SIZE / 2 - 2);
                    setlinecolor(RGB(255, 215, 0)); // 金色粗边
                    setlinestyle(PS_SOLID, 4);
                    circle(cx, cy, CELL_SIZE / 2 - 3);
                    setlinestyle(PS_SOLID, 1);
                    setlinecolor(BLACK);
                }
                else {
                    setfillcolor(WHITE);
                    setlinecolor(BLUE);
                    setlinestyle(PS_SOLID, 4);      // 边框粗细
                    circle(cx, cy, CELL_SIZE / 2 - 5);
                    fillcircle(cx, cy, CELL_SIZE / 2 - 5);
                    setlinecolor(BLACK);
                    setlinestyle(PS_SOLID, 1); // 恢复线宽
                }
            }
            else if (gridInfo[i][j] == obstacle) {
                setfillcolor(BLUE);
                roundrect(cx - 20, cy - 20, cx + 20, cy + 20,15,15);
                fillroundrect(cx - 20, cy - 20, cx + 20, cy + 20, 15, 15);
            }
        }
    }
}
bool inMap(int x, int y) {
    if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE) {
        return false;
    }
    return true;
}
bool isPathClear(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0, dy = y1 - y0;//起点到终点的位置增量
    int step_x = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int step_y = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);//起点到终点的单位步长
    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) return false; // 不是直线或斜线
    int steps = max(abs(dx), abs(dy));//起点到终点走的最多步数
    for (int i = 1; i < steps; ++i) {
        int nx = x0 + step_x * i;
        int ny = y0 + step_y * i;
        if (!inMap(nx, ny) || gridInfo[nx][ny] != 0) return false;
    }
    return true;
}
bool legal(int x0, int y0, int x1, int y1, int x2, int y2, int color) {
    if (!inMap(x0, y0) || !inMap(x1, y1) || !inMap(x2, y2))return false;
    if (gridInfo[x1][y1] != 0)return false;
    if (gridInfo[x0][y0] != color)return false;
    if (gridInfo[x2][y2] != 0 && !(x2 == x0 && y2 == y0))return false;
    int temp = gridInfo[x0][y0];
    gridInfo[x0][y0] = 0;
    bool path = isPathClear(x0, y0, x1, y1) && isPathClear(x1, y1, x2, y2);
    gridInfo[x0][y0] = temp;
    return path;
}
bool check_fail (int color) {
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == color) {
                for (int i = 0; i < GRIDSIZE; i++) {
                    for (int step = 1; step < GRIDSIZE; step++) {
                        int nx = x + dx[i] * step;
                        int ny = y + dy[i] * step;
                        if (!inMap(nx, ny))break;
                        else if (gridInfo[nx][ny] != 0)break;
                        else {
                            for (int adir = 0; adir < 8; adir++) {
                                for (int astep = 1; astep < GRIDSIZE; astep++) {
                                    int ax = nx + dx[adir] * astep;
                                    int ay = ny + dy[adir] * astep;
                                    if (!inMap(ax, ay)) break;
                                    else if (gridInfo[ax][ay] != 0 && (!(ax == x && ay == y)))break;//这里射箭也要允许回到原位置！！！
                                    printf("check: %d %d -> %d %d -> %d %d, gridInfo[nx][ny]=%d, gridInfo[ax][ay]=%d\n", x, y, nx, ny, ax, ay, gridInfo[nx][ny], gridInfo[ax][ay]);
                                    if (legal(x, y, nx, ny, ax, ay, color)) {
                                        printf("合法走法: %d %d -> %d %d -> %d %d\n", x, y, nx, ny, ax, ay);
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (color == 1) {
        MessageBox(GetHWnd(), _T("白棋胜利！"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(GetHWnd(), _T("黑棋胜利！"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
    }
    return true;
}
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2,
    int color, bool check_only)
{
    if (!legal(x0, y0, x1, y1, x2, y2, color))return false;
    if (!check_only)
    {
        gridInfo[x0][y0] = 0;
        gridInfo[x1][y1] = color;
        gridInfo[x2][y2] = obstacle;
    }
    return true;
}
void saveGame(const string& filename) {
    ofstream fout(filename, ios::binary); // 打开文件，准备写入
    if (!fout) {
        MessageBox(GetHWnd(), _T("文件保存失败！"), _T("错误"), MB_OK | MB_ICONERROR);
        return;
    }
    fout.write((char*)gridInfo, sizeof(gridInfo)); // 写入棋盘
    fout.write((char*)&currentcolor, sizeof(currentcolor)); // 写入当前颜色
    fout.write((char*)&mycolor, sizeof(mycolor));  
    fout.close(); // 关闭文件
    MessageBox(GetHWnd(), _T("文件已保存！"), _T("提示"), MB_OK | MB_ICONINFORMATION);// 写入玩家颜色
}
void loadGame(const string& filename) {
    ifstream fin(filename, ios::binary); // 打开文件，准备读取
    if (!fin) {
        MessageBox(GetHWnd(), _T("文件读取失败！"), _T("错误"), MB_OK | MB_ICONERROR);
        return;
    }
    fin.read((char*)gridInfo, sizeof(gridInfo)); // 读回棋盘
    fin.read((char*)&currentcolor, sizeof(currentcolor)); // 读回当前颜色
    fin.read((char*)&mycolor, sizeof(mycolor));   
    // 读回玩家颜色
    fin.close(); // 关闭文件
    MessageBox(GetHWnd(), _T("文件已读取！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
}
void humanMove() {
    int x0=0, y0=0, x1=0, y1=0, x2=0, y2=0;
    //选棋子
    while (1) {
        ExMessage msg;
        int f = 0;
        while (peekmessage(&msg, EX_MOUSE|EX_KEY)) {
            if (msg.message == WM_MOUSEMOVE) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                if (inMap(row, col)&&gridInfo[row][col]==currentcolor) {
                    hoverRow = row;
                    hoverCol = col; 
                    cleardevice();
                    drawChessBoard();
                    drawPieces();
                }
            }
            // 新增：右键存盘，ESC结束游戏
            if (msg.message == WM_RBUTTONDOWN) {
                saveGame("amazons.sav");
                MessageBox(GetHWnd(), _T("游戏已保存！"), _T("提示"), MB_OK);
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                printf("点击: x=%d y=%d row=%d col=%d inMap=%d gridInfo=%d currentcolor=%d\n",
                    msg.x, msg.y, row, col, inMap(row, col), inMap(row, col) ? gridInfo[row][col] : -99, currentcolor);
                if (inMap(row, col) && gridInfo[row][col] == currentcolor) {
                    printf("选中棋子: row=%d col=%d\n", row, col);
                    x0 = row;
                    y0 = col;
                    f = 1;
                    break;
                }
            }
        }
        if (f) break;
    }
    // 选目标格
    while (true) {
        ExMessage msg;
        int f = 0;
        while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
            // 新增：右键存盘，ESC结束游戏
            if (msg.message == WM_RBUTTONDOWN) {
                saveGame("amazons.sav");
                MessageBox(GetHWnd(), _T("游戏已保存！"), _T("提示"), MB_OK);
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                if (inMap(row, col) && gridInfo[row][col] == 0&&isPathClear(x0,y0,row,col)) {
                    x1 = row; 
                    y1 = col;
                    f = 1;
                    printf("选中位置: row=%d col=%d\n", row, col);
                    break;
                }
            }
        }
        if (f)break;
    }
    //选射箭
    while (true) {
        ExMessage msg;
        int f = 0;
        while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
            // 新增：右键存盘，ESC结束游戏
            if (msg.message == WM_RBUTTONDOWN) {
                saveGame("amazons.sav");
                MessageBox(GetHWnd(), _T("游戏已保存！"), _T("提示"), MB_OK);
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                if (inMap(row, col) && (gridInfo[row][col] == 0||(row==x0&&col==y0) )&& legal(x0, y0, x1, y1, row, col, currentcolor)&&!(row==x1&&col==y1)) {
                    x2 = row;
                    y2 = col;
                    f = 1;
                    printf("选中射箭: row=%d col=%d\n", row, col);
                    break;
                }
            }
        }
        if (f&&ProcStep(x0, y0, x1, y1, x2, y2, currentcolor, false)) {
            break;
        }
    }
    cleardevice();
    drawChessBoard();
    drawPieces();
    return;
}
int mobility(int color) {
    int cnt = 0;
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == color) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step < GRIDSIZE; step++) {
                        int nx = x + dx[dir] * step;
                        int ny = y + dy[dir] * step;
                        if (!inMap(nx, ny)) break;
                        if (gridInfo[nx][ny] != 0) break;
                        // 射箭
                        for (int adir = 0; adir < 8; adir++) {
                            for (int astep = 1; astep < GRIDSIZE; astep++) {
                                int ax = nx + dx[adir] * astep;
                                int ay = ny + dy[adir] * astep;
                                if (!inMap(ax, ay)) break;
                                if (gridInfo[ax][ay] != 0 && !(ax == x && ay == y)) break;
                                // 判断完整合法性
                                // 这里用当前color，不影响主流程
                                if (legal(x, y, nx, ny, ax, ay, color)) {
                                    cnt++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return cnt;
}
void aiMove() {
    struct Move { int x0, y0, x1, y1, x2, y2; };
    vector<Move> bestmoves;
    int bestscore = -1e9;
    int backup[GRIDSIZE][GRIDSIZE];
    int color = currentcolor;
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == color) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step < GRIDSIZE; step++) {
                        int nx = x + dx[dir] * step;
                        int ny = y + dy[dir] * step;
                        if (!inMap(nx, ny)) break;
                        else if (gridInfo[nx][ny] != 0) break;
                        for (int adir = 0; adir < 8; adir++) {
                            for (int astep = 1; astep < GRIDSIZE; astep++) {
                                int ax = nx + dx[adir] * astep;
                                int ay = ny + dy[adir] * astep;
                                if (!inMap(ax, ay)) break;
                                else if (gridInfo[ax][ay] != 0 && !(ax == x && ay == y)) break;
                                if (legal(x, y, nx, ny, ax, ay, color)) {
                                    memcpy(backup, gridInfo, sizeof(gridInfo));
                                    ProcStep(x, y, nx, ny, ax, ay, color, false);
                                    int score = mobility(color) - mobility(-color);
                                    memcpy(gridInfo, backup, sizeof(gridInfo));
                                    if (score > bestscore) {
                                        bestscore = score;
                                        bestmoves.clear();
                                        bestmoves.push_back({ x, y, nx, ny, ax, ay });
                                    }
                                    else if (score == bestscore) {
                                        bestmoves.push_back({ x, y, nx, ny, ax, ay });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (!bestmoves.empty()) {
        Move m = bestmoves[rand() % bestmoves.size()];
        ProcStep(m.x0, m.y0, m.x1, m.y1, m.x2, m.y2, color, false);
        cleardevice();
        drawChessBoard();
        drawPieces();
        stringstream ss;
        ss << "AI move: " << m.x0 << " " << m.y0 << " " << m.x1 << " " << m.y1 << " " << m.x2 << " " << m.y2;
        string s = ss.str();
        printf("AI走步：%s\n", ss.str().c_str());
        wstring ws(s.begin(), s.end());
        outtextxy(0, 0, ws.c_str());
    }
    return;
}
void initBoard() {
    memset(gridInfo, 0, sizeof(gridInfo));
    gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
        = gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
        = gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
    gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
        = gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
        = gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;
}
int main() {
    srand((unsigned)time(0)); // 只需在main里调用一次即可
    initBoard(); 
    initgraph(600, 600); // 你可以根据需要调整窗口大小
    setbkcolor(RGB(128, 128, 128));
    cleardevice();
   /* drawBoard();*/
    drawChessBoard();
    drawPieces();
    settextstyle(40, 0, _T("Consolas"));
    outtextxy(100, 200, _T("点击屏幕左侧执黑(●)"));
    outtextxy(100, 300, _T("点击右侧执白 (○)"));
    while (true) {
        ExMessage msg;
        if (peekmessage(&msg, EM_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x < 300) {
                    mycolor = 1;
                    MessageBox(GetHWnd(), _T("你选择了黑棋 (●)"), _T("提示"), MB_OK);
                    break;
                }
                else {
                    mycolor = -1;
                    MessageBox(GetHWnd(), _T("你选择了白棋 (○)"), _T("提示"), MB_OK);
                    break;
                }
            }
        }
    }
    currentcolor = mycolor;
    while (true) {
    // 玩家回合
            currentcolor = mycolor;
            if (check_fail(currentcolor))break;
            humanMove();
             // AI回合
            currentcolor = -1 * mycolor;
            if (check_fail(currentcolor))break;
            aiMove();
    }
    closegraph();
}
