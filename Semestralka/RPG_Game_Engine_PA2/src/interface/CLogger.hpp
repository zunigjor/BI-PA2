//
// Created by jorge on 6/15/20.
//

#ifndef RPG_GAME_ENGINE_CLOGGER_HPP
#define RPG_GAME_ENGINE_CLOGGER_HPP

#include "../CGame.hpp"

/**
 * Logger class used to log messages and errors.
 */
class CLogger {
public:
    static void printToLog(const char * message);
};


#endif //RPG_GAME_ENGINE_CLOGGER_HPP
