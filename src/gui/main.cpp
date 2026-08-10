#include<iostream>
#include<cmath>
#include<vector>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include<sstream>
#include<string>
#include<graphics.h>
#include<conio.h>
#include<algorithm>
#include <mmsystem.h>
#include<queue>
#include<chrono>
#pragma comment(lib, "winmm.lib")
using namespace std;
using namespace chrono;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
steady_clock::time_point startTime;
const int GRIDSIZE = 8;
int gridInfo[GRIDSIZE][GRIDSIZE];
const int grid_black = 1;
const int grid_white = -1;
const int obstacle = 2;
int currentcolor;
int mycolor; // 人机模式下，玩家执子颜色
int dx[8] = { -1,-1,-1,0,0,1,1,1 };
int dy[8] = { -1,0,1,-1,1,-1,0,1 };
int hoverr = -1, hoverc = -1;//鼠标悬停的行以及列
vector<vector<int>>legalmove;//所有合法棋子路径
vector<vector<int>>legalarrow;//所有合法射箭路径
int destx, desty = 0;//记录当前选定的目标棋子位置
//update：图形界面鼠标选择菜单
// 棋盘左上角坐标和格子大小
const int BOARD_X = 50, BOARD_Y = 90, CELL_SIZE = 60;//棋盘左上角点的坐标，单个格子边长
const int lowerboundary = BOARD_X + GRIDSIZE * CELL_SIZE;//棋盘下边界
const int rightboundary = BOARD_Y + GRIDSIZE * CELL_SIZE;//棋盘右边界
int hoverbutton = -1;
vector<wstring> icons{ L"♻️", L"📂", L"❌" };//三个菜单功能的标识符
struct button {
    int x, y, w, h;//按钮左上角坐标x,y，宽w，高h
    wstring text;//按钮的文字
};
vector<button>buttons{
    {180,140,180,50,L"新开始"},
    {180,220,180,50,L"读盘"},{180,300,180,50,L"退出"}
};
void drawbackground() {
    for (int i = 0; i < 700; i += 2) {
        int g = 140 + (i * 80) / 700; // 渐变的绿色分量
        int r = 80 + (i * 80) / 700;
        int b = 80 + (i * 80) / 700;
        setfillcolor(RGB(r, g, b));
        solidrectangle(0, i, 700, i + 2);
    }
}
void drawmenu(int hoverbutton) {
    drawbackground();
    setbkcolor(RGB(128, 128, 128));
    wstring ws = L"主菜单";
    settextstyle(30, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    settextcolor(BLUE);
    outtextxy(230,80,ws.c_str());
    for (int i = 0; i < buttons.size(); i++) {
            // 悬停高亮
            if (i == hoverbutton) {
                setfillcolor(RGB(220, 220, 100)); // 黄色高亮
            }
            else {
                setfillcolor(RGB(210, 210, 210)); // 普通灰色
            }
            roundrect(buttons[i].x, buttons[i].y, buttons[i].x + buttons[i].w, 
                buttons[i].y + buttons[i].h,30,30);
            fillroundrect(buttons[i].x, buttons[i].y, buttons[i].x + buttons[i].w,
                buttons[i].y + buttons[i].h, 30, 30);
            settextstyle(30, 0, _T("微软雅黑"));
            setbkmode(TRANSPARENT);
            settextcolor(BLACK);
            outtextxy(buttons[i].x+50, buttons[i].y+10, buttons[i].text.c_str());
            outtextxy(buttons[i].x+10, buttons[i].y+10, icons[i].c_str());
    }
}
void drawstatusbar(bool cansave) {//增加一个参数，表示只有人类未选定或者ai走步完成时刻允许存盘
    //状态栏x=0,y=650,w=600,h=80
    int savex = 440, savey = 630, savew = 120, saveh = 40;
    int exitx = 300, exity = 630, exitw = 120, exith = 40;
    setfillcolor(YELLOW);
    if(cansave)fillroundrect(savex, savey, savew + savex, savey + saveh, 20, 20);
    fillroundrect(exitx, exity, exitw + exitx, exity + exith, 20, 20);
    settextstyle(24, 0, _T("微软雅黑"));
    settextcolor(BLUE);
    if(cansave)outtextxy(savex+40, savey+10, L"📂存盘");//只有允许存盘时，画出存盘按钮
    outtextxy(exitx + 40, exity + 10, L"❌退出");
}

int selectmenu() {
    drawmenu(hoverbutton);
    while (1) {
        ExMessage msg;
        if (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_MOUSEMOVE) {
                hoverbutton = -1;
                for (int i = 0; i < buttons.size(); i++) {
                    if (msg.x >= buttons[i].x && msg.x <= buttons[i].x + 180
                        && msg.y >= buttons[i].y && msg.y <= buttons[i].y + 50) {
                        hoverbutton = i;
                        break;
                    }
                }
                drawmenu(hoverbutton);
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                hoverbutton = -1;
                for (int i = 0; i < buttons.size(); i++) {
                    if (msg.x >= buttons[i].x && msg.x <= buttons[i].x + 180
                        && msg.y >= buttons[i].y && msg.y <= buttons[i].y + 50) {
                        hoverbutton = i;
                        return hoverbutton;
                    }
                }
            }
        }
       
    }
}
void drawChessBoard() {
    drawbackground();
    int gap = 4; // 格子间隙
    int shadowLayers = 8; // 阴影渐变层数
    // 1. 画棋盘底色
    setfillcolor(RGB(210, 210, 210));
    solidrectangle(BOARD_X, BOARD_Y,
        BOARD_X + GRIDSIZE * CELL_SIZE,
        BOARD_Y + GRIDSIZE * CELL_SIZE);
    // 2. 画交点高光（只画内部交点，且半径不大于gap）
    for (int i = 1; i < GRIDSIZE; i++) {
        for (int j = 1; j < GRIDSIZE; j++) {
            int cx = BOARD_X + j * CELL_SIZE;
            int cy = BOARD_Y + i * CELL_SIZE;
            for (int r = gap; r > 0; r--) {
                int bright = 255 - (gap - r) * 10;
                setfillcolor(RGB(bright, bright, bright));
                solidcircle(cx, cy, r);
            }
        }
    }
    // 3. 画格子阴影（右下渐变）
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE;j++) {
            int x1 = BOARD_X + j * CELL_SIZE + gap;
            int y1 = BOARD_Y + i * CELL_SIZE + gap;
            int x2 = BOARD_X + (j + 1) * CELL_SIZE - gap;
            int y2 = BOARD_Y + (i + 1) * CELL_SIZE - gap;
            // 多层阴影（右下偏移）
            for (int k = shadowLayers; k > 0; k--) {
                int alpha = 180 + k * 6; // 由深到浅
                    int rx1 = x1 + k, ry1 = y1 + k;//阴影举行左上角顶点
                    int rx2 = min(x2 + k, lowerboundary), ry2 = min(y2 + k, rightboundary);
                    //阴影矩形右下角顶点
                    if (rx1 >= lowerboundary || ry1 >= rightboundary) continue; // 跳过超出棋盘的阴影
                    setfillcolor(RGB(alpha, alpha, alpha));
                    solidrectangle(rx1, ry1, rx2, ry2);
            }
        }
    }
    // 4. 画格子本体（白色，最后画，遮住高光和阴影，只在缝隙处露出）
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            int x1 = BOARD_X + j * CELL_SIZE + gap;
            int y1 = BOARD_Y + i * CELL_SIZE + gap;
            int x2 = BOARD_X + (j + 1) * CELL_SIZE - gap;
            int y2 = BOARD_Y + (i + 1) * CELL_SIZE - gap;
            setfillcolor(WHITE);
            solidrectangle(x1, y1, x2, y2);
        }
    }
    //覆盖合法路径高亮
    for (int k = 0; k < legalmove.size(); k++) {
        int i = legalmove[k][0], j = legalmove[k][1];
        //// 不高亮选中棋子本身
        //if (i == hoverr && j == hoverc) continue;
        int x1 = BOARD_X + j * CELL_SIZE + gap;
        int y1 = BOARD_Y + i * CELL_SIZE + gap;
        int x2 = BOARD_X + (j + 1) * CELL_SIZE - gap;
        int y2 = BOARD_Y + (i + 1) * CELL_SIZE - gap;
        // 画多层右下偏移渐变阴影
        int shadowLayers = 8;
        for (int k = shadowLayers; k > 0; k--) {
            setfillcolor(RGB(255, 255, 100 + k * 15)); // 渐变黄
            solidrectangle(x1 + k, y1 + k, x2 + k, y2 + k);
        }
        // 最上层高亮
        setfillcolor(RGB(220, 180, 40));
        solidrectangle(x1, y1, x2, y2);
    }
    for (int k = 0; k < legalarrow.size(); ++k) {
        int i = legalarrow[k][0], j = legalarrow[k][1];
        // 选中棋子本身保持黄色
        if (i == destx && j == desty) {
            int x1 = BOARD_X + j * CELL_SIZE + gap;
            int y1 = BOARD_Y + i * CELL_SIZE + gap;
            int x2 = BOARD_X + (j + 1) * CELL_SIZE - gap;
            int y2 = BOARD_Y + (i + 1) * CELL_SIZE - gap;
            int shadowLayers = 8;
            for (int k = shadowLayers; k > 0; k--) {
                setfillcolor(RGB(255, 255, 100 + k * 15)); // 渐变黄色
                solidrectangle(x1 + k, y1 + k, x2 + k, y2 + k);
            }
            setfillcolor(RGB(220, 180, 40));
            solidrectangle(x1, y1, x2, y2);
            continue;

        }
        int x1 = BOARD_X + j * CELL_SIZE + gap;
        int y1 = BOARD_Y + i * CELL_SIZE + gap;
        int x2 = BOARD_X + (j + 1) * CELL_SIZE - gap;
        int y2 = BOARD_Y + (i + 1) * CELL_SIZE - gap;
        int shadowLayers = 8;
        for (int k = shadowLayers; k > 0; k--) {
            setfillcolor(RGB(255, 80 + k * 10, 80 + k * 15)); // 渐变红
            solidrectangle(x1 + k, y1 + k, x2 + k, y2 + k);
        }
        setfillcolor(RGB(200, 40, 40));
        solidrectangle(x1, y1, x2, y2);
    }
    // 绘制上方和左侧的数字坐标（0-7）
    settextstyle(20, 0, _T("华文行楷")); // 小字体
    settextcolor(BLACK);

    // 上方列坐标
    for (int j = 0; j < GRIDSIZE; ++j) {
        int x = BOARD_X + j * CELL_SIZE + CELL_SIZE / 2 - 5; // 居中微调
        int y = BOARD_Y - 22; // 比棋盘上边界略高，避免和提示栏冲突
        wchar_t buf[2];
        swprintf(buf, 2, L"%d", j);
        outtextxy(x, y, buf);
    }

    // 左侧行坐标
    for (int i = 0; i < GRIDSIZE; ++i) {
        int x = BOARD_X - 18; // 比棋盘左边界略左
        int y = BOARD_Y + i * CELL_SIZE + CELL_SIZE / 2 - 8; // 居中微调
        wchar_t buf[2];
        swprintf(buf, 2, L"%d", i);
        outtextxy(x, y, buf);
    }
}// 画棋子和障碍
void drawPieces() {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            int cx = BOARD_X + j * CELL_SIZE + CELL_SIZE / 2;
            int cy = BOARD_Y + i * CELL_SIZE + CELL_SIZE / 2;
            int shadowradius = CELL_SIZE / 2 - 6;
            int pieceradius = CELL_SIZE / 2 - 10;
            if (gridInfo[i][j] == grid_black) {
                bool isHover = (i == hoverr && j == hoverc && 
                    gridInfo[i][j] == currentcolor);
                // 画多层同心圆模拟淡淡的渐变阴影
                // 画阴影：悬停绿色渐变，否则灰色渐变
                for (int r = shadowradius; r > shadowradius - 8; --r) {
                    if (isHover) {
                        int g = 255;
                        int base = 180 + (shadowradius - r) * 5;
                        setfillcolor(RGB(base, g, base)); // 亮绿色渐变
                    }
                    else {
                        int gray = 200 + (shadowradius - r) * 4;
                        setfillcolor(RGB(gray, gray, gray));
                    }
                    solidcircle(cx, cy, r);
                }
                setfillcolor(BLACK);
                solidcircle(cx, cy, pieceradius);
            }
            else if (gridInfo[i][j] == grid_white) {
                // 阴影参数
                bool ishover = (i == hoverr && j == hoverc &&
                    gridInfo[i][j] == currentcolor);
                // 画多层同心圆模拟淡淡的渐变阴影
                // 画阴影：悬停绿色渐变，否则灰色渐变
                for (int r = shadowradius; r > shadowradius - 8; --r) {
                    if (ishover) {
                        int g = 255;
                        int base = 180 + (shadowradius - r) * 5;
                        setfillcolor(RGB(base, g, base)); // 亮绿色渐变
                    }
                    else {
                        int gray = 200 + (shadowradius - r) * 4;
                        setfillcolor(RGB(gray, gray, gray));
                    }
                    solidcircle(cx, cy, r);
                }
                    setfillcolor(WHITE);
                    setlinecolor(BLACK);
                    circle(cx, cy, pieceradius);//白棋黑边框
                    fillcircle(cx, cy, pieceradius);
            }
            else if (gridInfo[i][j] == obstacle) {
                for (int r = shadowradius; r > shadowradius - 8; --r) {
                    int gray = 200 + (shadowradius - r) * 4;
                    setfillcolor(RGB(gray, gray, gray));
                    solidcircle(cx, cy, r);
                }
                setfillcolor(BLUE);
                fillcircle(cx, cy, pieceradius);
                setlinecolor(BLACK);
                circle(cx, cy, pieceradius);//障碍黑色边框
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
    int stepx=0;
    if (dx == 0)stepx = 0;
    else {
        if (dx > 0)stepx = 1;
        else stepx = -1;
    }
    int stepy = 0;
    if (dy == 0)stepy = 0;
    else {
        if (dy > 0)stepy = 1;
        else stepy = -1;
    }
    //起点到终点的单位步长
    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) return false; // 不是直线或斜线
    int steps = max(abs(dx), abs(dy));//起点到终点走的最多步数
    for (int i = 1; i < steps; i++ ) {
        int nx = x0 + stepx * i;
        int ny = y0 + stepy * i;
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
                                    else if (ax == nx && ay == ny)continue;
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
bool isfail(int color) {
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
                                    else if (ax == nx && ay == ny)continue;
                                    else if (gridInfo[ax][ay] != 0 && (!(ax == x && ay == y)))break;//这里射箭也要允许回到原位置！！
                                    if (legal(x, y, nx, ny, ax, ay, color)) {
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
    fout.write((char*)&mycolor, sizeof(mycolor));  //写入玩家颜色
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
    legalmove.clear();
    legalarrow.clear();
    //选棋子
    while (1) {
        ExMessage msg;
        int f = 0;
        // 选棋子前
        settextstyle(32, 0, _T("微软雅黑"));
        settextcolor(BLUE); // 深蓝色
        outtextxy(400, 0, L"请点击棋子");
        while (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN && msg.x>=300&&msg.x<=420
                &&msg.y>=630&&msg.y<=670) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_MOUSEMOVE) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                if (inMap(row, col)&&gridInfo[row][col]==currentcolor) {
                    hoverr = row;
                    hoverc = col; 
                    cleardevice();
                    drawChessBoard();
                    drawPieces();
                    drawstatusbar(true);//棋子尚未选定，允许存盘
                }
                //每次经过合法棋子，更新hoverr和hoverc,调用drawboard，刷新棋盘
                //这里实现人类鼠标悬停的位置棋子绿色高亮
            }
            if (msg.message == WM_LBUTTONDOWN && (msg.x>=440)&&(msg.x<=560)&&(msg.y>=630)&&(msg.y<=670)) {
                saveGame("amazons.sav");
                MessageBox(GetHWnd(), _T("游戏已保存！"), _T("提示"), MB_OK);
                
            }
  
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;//转换坐标
                printf("点击: x=%d y=%d row=%d col=%d inMap=%d gridInfo=%d currentcolor=%d\n",
                msg.x, msg.y, row, col, inMap(row, col), inMap(row, col) ? gridInfo[row][col] : -99, currentcolor);
                //调试输出点击位置，如果点击位置在棋盘外部那么输出-99
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
    for (int dir = 0; dir < 8; dir++) {
        for (int step = 1; step < GRIDSIZE; step++) {
            int nx = x0 + dx[dir] * step;
            int ny = y0 + dy[dir] * step;
            if (!inMap(nx, ny) || gridInfo[nx][ny] != 0) break;
            legalmove.push_back({ nx, ny });
        }
    }
    cleardevice();
    drawChessBoard();
    drawPieces();
    drawstatusbar(false);//此时已经不允许存盘
    // 选目标格
    while (true) {
        ExMessage msg;
        int f = 0;
        // 选目标格前
        settextstyle(32, 0, _T("微软雅黑"));
        settextcolor(BLUE);
        outtextxy(400, 0, L"请选择目标位置");
        while (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN && msg.x >= 300 
                && msg.x <= 420 && msg.y >= 630 && msg.y <= 670) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                if (inMap(row, col) && gridInfo[row][col] == 0&&isPathClear(x0,y0,row,col)) {
                    x1 = row; 
                    y1 = col;
                    destx = row;
                    desty = col;
                    f = 1;
                    cout << "选中位置：row=" << row << " col=" << col << endl;
                    break;
                }
            }
        }
        if (f)break;
    }
    for (int i = 0; i < 8; i++) {
        for (int step = 0; step < 8; step++) {
            int ax = x1 + dx[i] * step;
            int ay = y1 + dy[i] * step;
            if (!inMap(ax, ay) || (gridInfo[ax][ay] != 0) && (ax != x0 && ay != y0))break;
            if (legal(x0, y0, x1, y1, ax, ay, currentcolor)) {
                legalarrow.push_back({ ax,ay });
            }
        }
    }
    legalmove.clear();
    cleardevice();
    drawChessBoard();
    drawPieces();
    drawstatusbar(false);
    //选射箭
    while (true) {
        // 选射箭前
        settextstyle(32, 0, _T("微软雅黑"));
        settextcolor(BLUE);
        outtextxy(400, 0, L"请选择射箭位置");
        ExMessage msg;
        int f = 0;
        while (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN && msg.x >= 300 && 
                msg.x <= 420 && msg.y >= 630 && msg.y <= 670) {
                closegraph();
                exit(0);
            }
            if (msg.message == WM_LBUTTONDOWN) {
                int row = (msg.y - BOARD_Y) / CELL_SIZE;
                int col = (msg.x - BOARD_X) / CELL_SIZE;
                //人类的合法射箭判别需要排除掉目标点
                if (inMap(row, col) && (gridInfo[row][col] == 0||(row==x0&&col==y0) )
                    && legal(x0, y0, x1, y1, row, col, currentcolor)&&!(row==x1&&col==y1)) {
                    x2 = row;
                    y2 = col;
                    f = 1;
                    cout << "选中射箭： row=" << row << " col=" << col << endl;
                    break;
                }
            }
        }
        if (f&&ProcStep(x0, y0, x1, y1, x2, y2, currentcolor, false)) {
            break;
        }
    }
    legalarrow.clear();
    cleardevice();
    drawChessBoard();
    drawPieces();
    return;
}
int fastmobility(int color) {
    int score = 0;
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == color) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step <= 4; step++) {
                        int nx = x + dx[dir] * step, ny = y + dy[dir] * step;
                        if (!inMap(nx, ny) || gridInfo[nx][ny] != 0) break;
                        score += 5;
                    }
                }
            }
        }
    }
    return score;
}
int blockscore(int color) {
    int c = 0;
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == obstacle) {
                bool block = false;
                for (int dir = 0; dir < 8; dir++) {
                    int nx = x + dx[dir], ny = y + dy[dir];
                    if (inMap(nx, ny) && gridInfo[nx][ny] == -color) {
                        block = true;
                        break;
                    }
                }
                if (block) {
                    c += 10;

                }
            }
        }
    }
    return c;
}
int evaluate() {
    int score = 0;
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == currentcolor) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step <= 4; step++) {
                        int nx = x + dx[dir] * step, ny = y + dy[dir] * step;
                        if (!inMap(nx, ny) || gridInfo[nx][ny] != 0) break;
                        score += 5;
                    }//每个方向每步+5
                }
            }
            else if (gridInfo[x][y] == -currentcolor) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step <= 4; step++) {
                        int nx = x + dx[dir] * step, ny = y + dy[dir] * step;
                        if (!inMap(nx, ny) || gridInfo[nx][ny] != 0) break;
                        score -= 5;
                    }
                }
            }
            else if (gridInfo[x][y] == obstacle) {
                bool block = false;
                for (int dir = 0; dir < 8; dir++) {
                    int nx = x + dx[dir], ny = y + dy[dir];
                    if (inMap(nx, ny) && gridInfo[nx][ny] == -currentcolor) {
                        block = true;
                        break;
                    }
                }
                if (block) {
                    score += 10;
                }
            }
        }
    }
    return score;
}
const int MAX_SEARCH_TIME = 900; // ms
bool timeOut = false;
int minimax(int color, int alpha, int beta, int depth, int maxdepth) {
    if (timeOut) return evaluate();
    auto now = steady_clock::now();
    if (duration_cast<milliseconds>(now - startTime).count() >= MAX_SEARCH_TIME) {
        timeOut = true;
        return evaluate(); // 或用你的评估函数
    }
    if (isfail(color))return -1e9;//输的子节点返回极小值
    if (isfail(-color))return 1e9;//赢的子节点返回最大值
    if (depth == maxdepth || timeOut) return evaluate();
    struct Move { int x0, y0, x1, y1, x2, y2; };
    vector<Move>moves;//枚举所有走法存入moves结构体数组
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == color) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step <= 4; step++) {
                        int nx = x + dx[dir] * step;
                        int ny = y + dy[dir] * step;
                        if (!inMap(nx, ny)) break;
                        if (gridInfo[nx][ny] != 0) break;
                        // 射箭
                        for (int adir = 0; adir < 8; adir++) {
                            for (int astep = 1; astep <= 4; astep++) {
                                int ax = nx + dx[adir] * astep;
                                int ay = ny + dy[adir] * astep;
                                if (!inMap(ax, ay)) break;
                                else if (gridInfo[ax][ay] != 0 && !(ax == x && ay == y)) break;
                                // 判断完整合法性
                                // 这里用当前color，不影响主流程
                                if (legal(x, y, nx, ny, ax, ay, color)) {
                                    moves.push_back({ x,y,nx,ny,ax,ay });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (color == currentcolor) {
        for (size_t i = 0; i < moves.size() /*&& i < static_cast<size_t>(n)*/; i++) {
            int backup[GRIDSIZE][GRIDSIZE];
            memcpy(backup, gridInfo, sizeof(gridInfo));
            ProcStep(moves[i].x0, moves[i].y0, moves[i].x1, moves[i].y1, moves[i].x2,
                moves[i].y2, color, false);
            /*printf("AI候选走法: %d %d -> %d %d -> %d %d\n", x, y, nx, ny, ax, ay);*/
            int score = minimax(-color, alpha, beta, depth + 1, maxdepth);
            /* printf("minimax分数: %d\n", score);*/
            alpha = max(score, alpha);
            memcpy(gridInfo, backup, sizeof(backup));
            if (alpha >= beta)break;
            if (timeOut)break;

        }
        return alpha;
    }
    else {
        for (size_t i = 0; i < moves.size() /*&& i < static_cast<size_t>(n)*/; i++) {
            int backup[GRIDSIZE][GRIDSIZE];
            memcpy(backup, gridInfo, sizeof(gridInfo));
            ProcStep(moves[i].x0, moves[i].y0, moves[i].x1, moves[i].y1, moves[i].x2,
                moves[i].y2, color, false);
            int score = minimax(-color, alpha, beta, depth + 1, maxdepth);
            beta = min(score, beta);
            memcpy(gridInfo, backup, sizeof(backup));
            if (alpha >= beta)break;
            if (timeOut)break;
        }
        return beta;
    }
}
void aiMove() {
    struct Move { int x0, y0, x1, y1, x2, y2; };
    vector<Move>bestmoves;
    int bestscore = -1e9;
    int backup[GRIDSIZE][GRIDSIZE];
    vector<Move>moves;//枚举所有走法存入moves结构体数组
    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if (gridInfo[x][y] == currentcolor) {
                for (int dir = 0; dir < 8; dir++) {
                    for (int step = 1; step <= 4; step++) {
                        int nx = x + dx[dir] * step;
                        int ny = y + dy[dir] * step;
                        if (!inMap(nx, ny)) break;
                        if (gridInfo[nx][ny] != 0) break;
                        // 射箭
                        for (int adir = 0; adir < 8; adir++) {
                            for (int astep = 1; astep <= 4; astep++) {
                                int ax = nx + dx[adir] * astep;
                                int ay = ny + dy[adir] * astep;
                                if (!inMap(ax, ay)) break;
                                else if (gridInfo[ax][ay] != 0 && !(ax == x && ay == y)) break;
                                // 判断完整合法性
                                if (legal(x, y, nx, ny, ax, ay, currentcolor)) {
                                    moves.push_back({ x,y,nx,ny,ax,ay });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    sort(moves.begin(), moves.end(), [&](const Move& a, const Move& b) {
        int backup[GRIDSIZE][GRIDSIZE];
        memcpy(backup, gridInfo, sizeof(gridInfo));
        ProcStep(a.x0, a.y0, a.x1, a.y1, a.x2, a.y2, currentcolor, false);
        int scoreA = fastmobility(currentcolor) - fastmobility(-currentcolor) + 5 * blockscore(currentcolor);
        memcpy(gridInfo, backup, sizeof(gridInfo));
        ProcStep(b.x0, b.y0, b.x1, b.y1, b.x2, b.y2, currentcolor, false);
        int scoreB = fastmobility(currentcolor) - fastmobility(-currentcolor) + 5 * blockscore(currentcolor);
        memcpy(gridInfo, backup, sizeof(gridInfo));
        return scoreA > scoreB;
        });
    Move lastBestMove = { -1, -1, -1, -1, -1, -1 };
    for (int depth = 1; depth <= 5; depth++) {
        if (timeOut)break;
        bestscore = -1e9;
        bestmoves.clear();
        for (size_t i = 0; i < moves.size(); i++) {
            memcpy(backup, gridInfo, sizeof(gridInfo));
            ProcStep(moves[i].x0, moves[i].y0, moves[i].x1, moves[i].y1, moves[i].x2, moves[i].y2, currentcolor, false);
            int score = minimax(-currentcolor, -1e9, 1e9, 1, depth);
            memcpy(gridInfo, backup, sizeof(gridInfo));
            if (score > bestscore) {
                bestscore = score;
                bestmoves.clear();
                bestmoves.push_back({ moves[i].x0, moves[i].y0, moves[i].x1, moves[i].y1, moves[i].x2, moves[i].y2
                    });
            }
            else if (score == bestscore) {
                bestmoves.push_back({ moves[i].x0, moves[i].y0, moves[i].x1, moves[i].y1, moves[i].x2, moves[i].y2 });
            }
        }
        if (!timeOut && !bestmoves.empty()) {
            lastBestMove = bestmoves[rand() % bestmoves.size()];
        }
    }
    // 兜底：如果lastBestMove未被更新且moves非空，随机输出一个合法走法
    if (lastBestMove.x0 == -1 && !moves.empty()) {
        lastBestMove = moves[rand() % moves.size()];
    }
    cout << "AI走步: " << lastBestMove.y0 << " " << lastBestMove.x0 << " "
        << lastBestMove.y1 << " " << lastBestMove.x1 << " "
        << lastBestMove.y2 << " " << lastBestMove.x2 << endl;
    if (lastBestMove.x0 != -1) {
        ProcStep(lastBestMove.x0, lastBestMove.y0, lastBestMove.x1, lastBestMove.y1, lastBestMove.x2, lastBestMove.y2, currentcolor, false);
    }
    cleardevice();
    drawChessBoard();
    drawPieces();
    drawstatusbar(true); // AI走步完成，画出两个按钮
    settextstyle(32, 0, _T("微软雅黑"));
    settextcolor(BLUE);
    wstringstream wss;
    wss << L"AI走步: " << lastBestMove.x0 << L" " << lastBestMove.y0 << L" "
        << lastBestMove.x1 << L" " << lastBestMove.y1 << L" "
        << lastBestMove.x2 << L" " << lastBestMove.y2;
    outtextxy(50, 0, wss.str().c_str());
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
    srand((unsigned)time(0)); 
    initBoard(); 
    initgraph(600, 700); 
    cleardevice();
    setbkcolor(RGB(128, 128, 128));
    drawbackground();
    drawmenu(hoverbutton);
    while (1) {
        int x = selectmenu();
        if (x == 0) {
            PlaySound(_T("GameSelectionSound.wav"), NULL, SND_FILENAME | SND_ASYNC);
            cleardevice();
            drawbackground();

            // 绘制黑棋按钮
            setfillcolor(YELLOW);
            fillroundrect(150, 140, 450, 200, 30, 30);
            settextcolor(BLACK);
            settextstyle(36, 0, _T("微软雅黑"));
            outtextxy(200, 155, L"⚫ 点击这里执黑");
            // 绘制白棋按钮
            setfillcolor(YELLOW);
            fillroundrect(150, 240, 450, 300, 30, 30);
            settextcolor(BLACK);
            settextstyle(36, 0, _T("微软雅黑"));
            outtextxy(200, 255, L"⚪ 点击这里执白");
            while (1) {
                ExMessage msg;
                if (peekmessage(&msg, EM_MOUSE)) {
                    if (msg.message == WM_LBUTTONDOWN) {
                        PlaySound(_T("Game Selection Sound.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        if (msg.x >= 150 && msg.x <= 450 && msg.y >= 140 && msg.y <= 200) {
                            mycolor = 1;
                            MessageBox(GetHWnd(), _T("你选择了黑棋"), _T("提示"), MB_OK);
                            currentcolor = mycolor;
                            cleardevice();
                            drawChessBoard();
                            drawPieces();
                            break;
                        }
                        if (msg.x >= 150 && msg.x <= 450 && msg.y >= 240 && msg.y <= 300) {
                            mycolor = -1;
                            MessageBox(GetHWnd(), _T("你选择了白棋"), _T("提示"), MB_OK);
                            currentcolor = mycolor;
                            cleardevice();
                            drawChessBoard();
                            drawPieces();
                            drawstatusbar(true);
                            break;
                        }
                    }
                }
            }
            break;
        }
        else if (x== 1){
            loadGame("amazons.sav");
            cleardevice();
            drawChessBoard();
            drawPieces();
            break;
        }
        else if (x == 2) {
            closegraph();
            exit(0);
        }
    }
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