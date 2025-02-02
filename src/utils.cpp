/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:28:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/03/30 14:13:50 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "circlegame.hpp"

float getRandomNumber(float min, float max)
{
    static std::default_random_engine engine;
    static std::uniform_real_distribution<> ret(min, max);
    return ret(engine);
}

void    setCurrentTime(Data &data)
{
    data.currentTime = GetTime();
}

void    setStartTime(Data &data)
{
    data.startTime = GetTime();
}

void    newRound(Data &data)
{
    for (int i = 0; i < data.circleAmount; ++i)
	{
		data.circle[i].activateCircle();
		data.circle[i].resetCircle();
	}
	data.player.setupPlayer();
	data.gameMode = START_SCREEN;
	data.gameover = 0;
    setStartTime(data);
    setCurrentTime(data);
    data.modeTime = data.currentTime;
}

void    setFPS(Data &data)
{
    currentFPS = GetFPS();
}

void    powerUpSpawn(Data &data)
{
    for (int i = 0; i < data.powerUpAmount; ++i)
    {
        if (data.powerUp[i].getState() == DISABLED && data.powerUp[i].getNextSpawnTime() == data.currentTime)
        {
            data.powerUp[i].setSpawnTime(data.currentTime);
            data.powerUp[i].resetPowerUp();
        }
    }
}

void    powerUpDraw(Data &data)
{
    for (int i = 0; i < data.powerUpAmount; ++i)
    {
        int powerUpState = data.powerUp[i].getState();
        if (powerUpState == SPAWNED || powerUpState == DESPAWNING)
        {
            if (data.currentTime - data.powerUp[i].getSpawnTime() > 7)
            {
                data.powerUp[i].setState(DESPAWNING);
            }
            data.powerUp[i].drawPowerUp();
        }
        if (powerUpState == DESPAWNED)
        {
            data.powerUp[i].setState(DISABLED);
            data.powerUp[i].setNextSpawnTime(data.currentTime + GetRandomValue(8, 20));
        }
    }
}

void    takePowerUp(Data &data)
{
    for (int i = 0; i < data.powerUpAmount; ++i)
    {
        if (data.powerUp[i].getState() != DISABLED && CheckCollisionCircles(data.player.getPos(), data.player.getSize(), data.powerUp[i].getPos(), data.powerUp[i].getSize()))
        {
            data.player.setPowerUpHold(data.powerUp[i].getID());
            data.powerUp[i].setState(DISABLED);
            data.powerUp[i].setNextSpawnTime(data.currentTime + GetRandomValue(8, 20));
        }
    }
}

void    usePowerUp(Data &data)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        int powerUpHold = data.player.getPowerUpHold();
        switch (powerUpHold) {
            case SHIELD :
                data.player.setShieldTimer(data.currentTime + 10);
                data.player.setActiveShield(1);
                data.player.setPowerUpHold(NONE);
                break ;
            case SKIPMODE :
                data.player.setMode(STANDARD);
                data.player.setPowerUpHold(NONE);
                break ;
        }
    }
    if (data.player.getShieldTimer() == data.currentTime && data.player.getActiveShield() == 1)
    {
        data.player.setActiveShield(0);
    }
}
