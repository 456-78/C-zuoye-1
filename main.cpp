/**
 * @file main.cpp
 * @brief 飞机大战游戏 - 主程序入口
 * @version 1.0
 */

#include "Game.h"
#include <graphics.h>
#include <conio.h>
#include <windows.h>

 // 屏蔽 EasyX 的 unicode 警告
#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif

int main()
{
    // 创建游戏实例并启动
    Game game;
    game.run();

    // 关闭图形窗口
    closegraph();

    return 0;
}