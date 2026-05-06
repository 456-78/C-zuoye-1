/**
 * @file Game.cpp
 * @brief Game Management Class Implementation
 */

#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <graphics.h>
#include <conio.h>

const COLORREF COLOR_BG = RGB(20, 20, 40);//背景
const COLORREF COLOR_UI_TEXT = RGB(255, 255, 255);//界面文字
const COLORREF COLOR_SCORE = RGB(255, 215, 0);//分数显示
const COLORREF COLOR_LIFE = RGB(255, 100, 100);//生命值显示
const COLORREF COLOR_TITLE = RGB(100, 200, 255);//标题文字

Game::Game()
    : m_hwnd(NULL)
    , m_state(GameState::MENU)
    , m_difficulty(Difficulty::NORMAL)
    , m_player(nullptr)
    , m_score(0)
    , m_highScore(0)
    , m_enemySpawnTimer(0)
    , m_frameCount(0)
    , m_enemySpawnInterval(60)
    , m_enemySpeedMultiplier(1)
{
    srand((unsigned)time(NULL));
    initgraph(GAME_WIDTH, GAME_HEIGHT);//4.27到此//初始化窗口
    m_hwnd = GetHWnd();
    SetWindowText(m_hwnd, _T("Aircraft Battle"));
    srand(GetTickCount());
}

Game::~Game()
{
    delete m_player;
    m_player = nullptr;
    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        delete m_enemies[i];
    }
    m_enemies.clear();
    for (size_t i = 0; i < m_bullets.size(); i++)
    {
        delete m_bullets[i];
    }
    m_bullets.clear();
}

void Game::init()
{
}

void Game::run()
{
    m_state = GameState::MENU;
    drawMenu();

    while (true)
    {
        ExMessage msg;
        while (peekmessage(&msg, EM_KEY))
        {
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
            {
                return;
            }
            if (msg.message == WM_KEYDOWN)
            {
                m_keyStates[msg.vkcode] = true;
            }
            else if (msg.message == WM_KEYUP)
            {
                m_keyStates[msg.vkcode] = false;
            }
        }

        switch (m_state)
        {
        case GameState::MENU:
            if (m_keyStates[VK_RETURN] || m_keyStates[VK_SPACE])
            {
                m_keyStates[VK_RETURN] = false;
                m_keyStates[VK_SPACE] = false;
                resetGame();
                applyDifficulty();
                m_state = GameState::PLAYING;
            }
            if (m_keyStates[0x31])
            {
                m_difficulty = Difficulty::EASY;
                m_keyStates[0x31] = false;
            }
            else if (m_keyStates[0x32])
            {
                m_difficulty = Difficulty::NORMAL;
                m_keyStates[0x32] = false;
            }
            else if (m_keyStates[0x33])
            {
                m_difficulty = Difficulty::HARD;
                m_keyStates[0x33] = false;
            }
            drawMenu();
            Sleep(16);
            break;

        case GameState::PLAYING:
            handleInput();
            update();
            render();
            Sleep(16);
            break;

        case GameState::PAUSED:
            if (m_keyStates['P'] || m_keyStates[VK_ESCAPE])
            {
                m_keyStates['P'] = false;
                m_keyStates[VK_ESCAPE] = false;
                m_state = GameState::PLAYING;
            }
            drawPaused();
            Sleep(16);
            break;

        case GameState::GAMEOVER:
            if (m_keyStates[VK_RETURN] || m_keyStates[VK_SPACE])
            {
                m_keyStates[VK_RETURN] = false;
                m_keyStates[VK_SPACE] = false;
                resetGame();
                applyDifficulty();
                m_state = GameState::PLAYING;
            }
            if (m_keyStates[VK_ESCAPE])
            {
                m_keyStates[VK_ESCAPE] = false;
                m_state = GameState::MENU;
                drawMenu();
            }
            drawGameOver();
            Sleep(16);
            break;
        }
    }
}

void Game::handleInput()
{
    if (m_player)
    {
        if ( m_keyStates[VK_UP])
        {
            m_player->move(0, -1);
        }
        if ( m_keyStates[VK_DOWN])
        {
            m_player->move(0, 1);
        }
        if ( m_keyStates[VK_LEFT])
        {
            m_player->move(-1, 0);
        }
        if ( m_keyStates[VK_RIGHT])
        {
            m_player->move(1, 0);
        }
        if (m_keyStates[VK_SPACE])
        {
            if (m_player->canShoot())           
            {
                int bulletX = m_player->getX() + PLAYER_WIDTH / 2 - BULLET_WIDTH / 2;
                int bulletY = m_player->getY();
                m_bullets.push_back(new Bullet(bulletX, bulletY, PLAYER_BULLET_SPEED, PLAYER_BULLET_DAMAGE, true));
                m_player->startCooldown();      
            }
        }
    }

    if (m_keyStates['P'])
    {
        m_keyStates['P'] = false;
        m_state = GameState::PAUSED;
    }
    if (m_keyStates[VK_ESCAPE])
    {
        m_keyStates[VK_ESCAPE] = false;
        m_state = GameState::PAUSED;
    }
}

void Game::update()
{
    if (!m_player) return;

    m_frameCount++;
    m_player->update();

    spawnEnemies();

    for (int i = (int)m_enemies.size() - 1; i >= 0; i--)
    {
        m_enemies[i]->update();
        if (m_enemies[i]->isOutOfScreen())
        {
            delete m_enemies[i];
            m_enemies.erase(m_enemies.begin() + i);
        }
    }
    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        if (m_enemies[i]->canShoot())
        {
            m_bullets.push_back(new Bullet(m_enemies[i]->getBulletX(),m_enemies[i]->getBulletY(),5,1,false));
        }
    }
    for (int i = (int)m_bullets.size() - 1; i >= 0; i--)
    {
        m_bullets[i]->update();
        if (m_bullets[i]->isOutOfScreen())
        {
            delete m_bullets[i];
            m_bullets.erase(m_bullets.begin() + i);
        }
    }

    checkCollisions();

    if (m_player->getHP() <= 0)
    {
        m_state = GameState::GAMEOVER;
        if (m_score > m_highScore)
        {
            m_highScore = m_score;
        }
    }
}

void Game::render()
{
    cleardevice();
    setfillcolor(COLOR_BG);
    solidrectangle(0, 0, GAME_WIDTH, GAME_HEIGHT);

    if (m_player)
    {
        m_player->draw();
    }

    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        m_enemies[i]->draw();
    }

    for (size_t i = 0; i < m_bullets.size(); i++)
    {
        m_bullets[i]->draw();
    }

    drawUI();
    FlushBatchDraw();
}

void Game::drawUI()
{
    settextcolor(COLOR_UI_TEXT);

    TCHAR scoreText[64];
    swprintf_s(scoreText, _T("Score: %d"), m_score);
    outtextxy(10, 10, scoreText);

    TCHAR highScoreText[64];
    swprintf_s(highScoreText, _T("High: %d"), m_highScore);
    outtextxy(GAME_WIDTH - 120, 10, highScoreText);

    if (m_player)
    {
        TCHAR lifeText[64];
        swprintf_s(lifeText, _T("HP: %d"), m_player->getHP());
        settextcolor(COLOR_LIFE);
        outtextxy(10, 35, lifeText);
    }

    settextcolor(COLOR_TITLE);
    TCHAR diffText[32];
    switch (m_difficulty)
    {
    case Difficulty::EASY:
        swprintf_s(diffText, _T("[Easy]"));
        break;
    case Difficulty::NORMAL:
        swprintf_s(diffText, _T("[Normal]"));
        break;
    case Difficulty::HARD:
        swprintf_s(diffText, _T("[Hard]"));
        break;
    }
    outtextxy(GAME_WIDTH - 80, 35, diffText);
}

void Game::drawMenu()
{
    cleardevice();
    setfillcolor(COLOR_BG);
    solidrectangle(0, 0, GAME_WIDTH, GAME_HEIGHT);

    settextstyle(48, 0, _T("Arial"));
    settextcolor(COLOR_TITLE);
    TCHAR title[] = _T("Aircraft Battle");
    int titleWidth = textwidth(title);
    outtextxy((GAME_WIDTH - titleWidth) / 2, 150, title);

    settextstyle(20, 0, _T("Arial"));
    settextcolor(COLOR_UI_TEXT);
    TCHAR instructions[][64] = {
        _T("Controls:"),
        _T("Arrow Keys"),
        _T("Shoot: Space"),
        _T("Pause: P or ESC")
    };
    for (int i = 0; i < 4; i++)
    {
        outtextxy(100, 380 + i * 30, instructions[i]);
    }

    settextstyle(24, 0, _T("Arial"));
    TCHAR difficultyTitle[] = _T("Difficulty:");
    outtextxy(100, 520, difficultyTitle);

    TCHAR diff1[] = _T("1. Easy");
    TCHAR diff2[] = _T("2. Normal");
    TCHAR diff3[] = _T("3. Hard");

    COLORREF easyColor = (m_difficulty == Difficulty::EASY) ? COLOR_SCORE : COLOR_UI_TEXT;
    COLORREF normalColor = (m_difficulty == Difficulty::NORMAL) ? COLOR_SCORE : COLOR_UI_TEXT;
    COLORREF hardColor = (m_difficulty == Difficulty::HARD) ? COLOR_SCORE : COLOR_UI_TEXT;

    settextcolor(easyColor);
    outtextxy(120, 555, diff1);
    settextcolor(normalColor);
    outtextxy(120, 585, diff2);
    settextcolor(hardColor);
    outtextxy(120, 615, diff3);

    settextstyle(28, 0, _T("Arial"));
    settextcolor(RGB(255, 200, 100));
    TCHAR startHint[] = _T("Press Enter or Space to Start");
    int hintWidth = textwidth(startHint);
    outtextxy((GAME_WIDTH - hintWidth) / 2, 670, startHint);

    FlushBatchDraw();
}

void Game::drawGameOver()
{
    setfillcolor(RGB(0, 0, 0));
    solidrectangle(0, 0, GAME_WIDTH, GAME_HEIGHT);

    settextstyle(48, 0, _T("Arial"));
    settextcolor(RGB(255, 100, 100));
    TCHAR gameOverText[] = _T("GAME OVER");
    int textWidth = textwidth(gameOverText);
    outtextxy((GAME_WIDTH - textWidth) / 2, 200, gameOverText);

    settextstyle(32, 0, _T("Arial"));
    settextcolor(COLOR_SCORE);
    TCHAR finalScore[64];
    swprintf_s(finalScore, _T("Score: %d"), m_score);
    textWidth = textwidth(finalScore);
    outtextxy((GAME_WIDTH - textWidth) / 2, 300, finalScore);

    TCHAR highScoreText[64];
    swprintf_s(highScoreText, _T("High: %d"), m_highScore);
    textWidth = textwidth(highScoreText);
    outtextxy((GAME_WIDTH - textWidth) / 2, 350, highScoreText);

    settextstyle(28, 0, _T("Arial"));
    settextcolor(RGB(100, 200, 255));
    TCHAR hint1[] = _T("Press Enter to Restart");
    textWidth = textwidth(hint1);
    outtextxy((GAME_WIDTH - textWidth) / 2, 480, hint1);

    FlushBatchDraw();
}

void Game::drawPaused()
{
    setfillcolor(RGB(0, 0, 0));
    solidrectangle(0, 0, GAME_WIDTH, GAME_HEIGHT);

    settextstyle(48, 0, _T("Arial"));
    settextcolor(COLOR_TITLE);
    TCHAR pausedText[] = _T("PAUSED");
    int textWidth = textwidth(pausedText);
    outtextxy((GAME_WIDTH - textWidth) / 2, 280, pausedText);

    settextstyle(24, 0, _T("Arial"));
    settextcolor(COLOR_UI_TEXT);
    TCHAR hint1[] = _T("Press P or ESC to Continue");
    textWidth = textwidth(hint1);
    outtextxy((GAME_WIDTH - textWidth) / 2, 360, hint1);

    FlushBatchDraw();
}

void Game::checkCollisions()
{
    if (!m_player) return;

    int px = m_player->getX();
    int py = m_player->getY();
    int pw = m_player->getWidth();
    int ph = m_player->getHeight();

    // 1. 玩家子弹打敌机
    for (int i = (int)m_bullets.size() - 1; i >= 0; i--)
    {
        if (!m_bullets[i]->isPlayerBullet()) continue;

        int bx = m_bullets[i]->getX();
        int by = m_bullets[i]->getY();
        int bw = m_bullets[i]->getWidth();
        int bh = m_bullets[i]->getHeight();

        for (int j = (int)m_enemies.size() - 1; j >= 0; j--)
        {
            int ex = m_enemies[j]->getX();
            int ey = m_enemies[j]->getY();
            int ew = m_enemies[j]->getWidth();
            int eh = m_enemies[j]->getHeight();

            if (bx < ex + ew && bx + bw > ex && by < ey + eh && by + bh > ey)
            {
                int dmg = m_bullets[i]->getDamage();
                delete m_bullets[i];
                m_bullets.erase(m_bullets.begin() + i);
                m_enemies[j]->takeDamage(dmg);
                if (m_enemies[j]->getHP() <= 0)
                {
                    m_score += m_enemies[j]->getScore();
                    delete m_enemies[j];
                    m_enemies.erase(m_enemies.begin() + j);
                }
                break;
            }
        }
    }

    // 2. 敌机撞玩家
    for (int i = (int)m_enemies.size() - 1; i >= 0; i--)
    {
        int ex = m_enemies[i]->getX();
        int ey = m_enemies[i]->getY();
        int ew = m_enemies[i]->getWidth();
        int eh = m_enemies[i]->getHeight();

        if (px < ex + ew && px + pw > ex && py < ey + eh && py + ph > ey)
        {
            m_player->takeDamage(m_enemies[i]->getHP());
            delete m_enemies[i];
            m_enemies.erase(m_enemies.begin() + i);
        }
    }

    // 3. 敌弹打玩家
    for (int i = (int)m_bullets.size() - 1; i >= 0; i--)
    {
        if (m_bullets[i]->isPlayerBullet()) continue;

        int bx = m_bullets[i]->getX();
        int by = m_bullets[i]->getY();
        int bw = m_bullets[i]->getWidth();
        int bh = m_bullets[i]->getHeight();

        if (px < bx + bw && px + pw > bx && py < by + bh && py + ph > by)
        {
            m_player->takeDamage(1);
            delete m_bullets[i];
            m_bullets.erase(m_bullets.begin() + i);
        }
    }
}
    
    

void Game::spawnEnemies()
{
    m_enemySpawnTimer++;

    if (m_enemySpawnTimer >= m_enemySpawnInterval)
    {
        m_enemySpawnTimer = 0;

        EnemyType type;
        int randValue = rand() % 100;
        if (randValue < 30)
        {
            type = EnemyType::SMALL;
        }
        else if (randValue < 80)
        {
            type = EnemyType::MEDIUM;
        }
        else
        {
            type = EnemyType::LARGE;
        }

        Enemy* enemy = new Enemy(type);
        enemy->setSpeedMultiplier(m_enemySpeedMultiplier);
        m_enemies.push_back(enemy);
    }
}

void Game::resetGame()
{
    delete m_player;
    m_player = nullptr;

    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        delete m_enemies[i];
    }
    m_enemies.clear();

    for (size_t i = 0; i < m_bullets.size(); i++)
    {
        delete m_bullets[i];
    }
    m_bullets.clear();

    m_player = new Player();
    m_score = 0;
    m_enemySpawnTimer = 0;
    m_frameCount = 0;
}

void Game::applyDifficulty()
{
    switch (m_difficulty)
    {
    case Difficulty::EASY:
        m_enemySpawnInterval = 150;
        m_enemySpeedMultiplier = 0.5;
        break;
    case Difficulty::NORMAL:
        m_enemySpawnInterval = 100;
        m_enemySpeedMultiplier = 1.0;
        break;
    case Difficulty::HARD:
        m_enemySpawnInterval = 60;
        m_enemySpeedMultiplier = 1.5;
        break;
    }
}
