/**
 * @file Player.cpp
 * @brief 玩家飞机类实现
 */

#include "Player.h"
#include "Game.h"

 // 玩家颜色定义
const COLORREF PLAYER_BODY_COLOR = RGB(255, 50, 50);
const COLORREF PLAYER_WING_COLOR = RGB(80, 160, 220);
const COLORREF PLAYER_COCKPIT_COLOR = RGB(150, 220, 255);

Player::Player() {
    m_x = GAME_WIDTH / 2 - PLAYER_WIDTH / 2;
    m_y = GAME_HEIGHT - PLAYER_HEIGHT - 50;
    m_hp = PLAYER_MAX_HP;
    m_shootCooldown = 0;
}

Player::~Player()
{
}

void Player::update()
{
    if (m_shootCooldown > 0)
    {
        m_shootCooldown--;
    }
}

void Player::move(int dx, int dy)
{
    m_x += dx * PLAYER_SPEED;
    m_y += dy * PLAYER_SPEED;

    clampPosition();//限制不让飞出画框
}

void Player::draw()
{   
    //画机身
    setfillstyle(BS_SOLID);//设置图形的填充样式：纯色填充

    setfillcolor(PLAYER_BODY_COLOR);//填充颜色
    solidrectangle(m_x, m_y, m_x + PLAYER_WIDTH, m_y + PLAYER_HEIGHT);//设置一个实心矩形
    //画机头
    setfillcolor(PLAYER_COCKPIT_COLOR);//换个颜色
    
    POINT nose[] = {
        {m_x + PLAYER_WIDTH / 2, m_y},
        {m_x, m_y + PLAYER_HEIGHT / 2},
        {m_x + PLAYER_WIDTH, m_y + PLAYER_HEIGHT / 2}
    };
    solidpolygon(nose, 3);

    setfillcolor(PLAYER_WING_COLOR);
    POINT leftWing[] = {
        {m_x, m_y + PLAYER_HEIGHT / 2},
        {m_x - 15, m_y + PLAYER_HEIGHT / 2 + 20},
        {m_x + 10, m_y + PLAYER_HEIGHT / 2 + 15}
    };
    solidpolygon(leftWing, 3);

    POINT rightWing[] = {
        {m_x + PLAYER_WIDTH, m_y + PLAYER_HEIGHT / 2},
        {m_x + PLAYER_WIDTH + 15, m_y + PLAYER_HEIGHT / 2 + 20},
        {m_x + PLAYER_WIDTH - 10, m_y + PLAYER_HEIGHT / 2 + 15}
    };
    solidpolygon(rightWing, 3);
}

void Player::takeDamage(int damage)
{
    m_hp -= damage;
    if (m_hp < 0)
    {
        m_hp = 0;
    }
}

void Player::clampPosition()
{
    if (m_x < 0)
    {
        m_x = 0;
    }
    if (m_x > GAME_WIDTH - PLAYER_WIDTH)
    {
        m_x = GAME_WIDTH - PLAYER_WIDTH;
    }

    if (m_y < 0)
    {
        m_y = 0;
    }
    if (m_y > GAME_HEIGHT - PLAYER_HEIGHT)
    {
        m_y = GAME_HEIGHT - PLAYER_HEIGHT;
    }
}