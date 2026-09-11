#include "main-game.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using namespace acg;
using namespace std;

/**
 * @brief Готовит консоль Windows к выводу игры.
 *
 * Исходники и строки в кодировке UTF-8, а игра рисует псевдографику и цвета
 * ANSI-escape. По умолчанию консоль Windows использует OEM-кодировку (866) и
 * не обрабатывает escape-последовательности — из-за этого ASCII-арт
 * превращается в «тЦИтЦИ…». Здесь мы явно включаем UTF-8 и VT-обработку,
 * чтобы игра одинаково работала в cmd, PowerShell и Windows Terminal.
 */
static void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

int main() {
    setupConsole();

    auto* mission = new Mission;
    mission->setMaxShips(20);
    mission->setBudget(100000);

    // Значения по умолчанию для игрового поля (игрок может переопределить в меню)
    mission->setBaseACoordinates({1.0, 1.0});
    mission->setBaseBCoordinates({18.0, 18.0});
    mission->setSizeBaseA(2);
    mission->setSizeBaseB(2);
    mission->setNecessaryDamage(3000);

    auto* game = new GameController(mission);
    game->gameProcess();

    delete mission;
    delete game;
    return 0;
}
