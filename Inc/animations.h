//---------------------------------------------------------
// animations.h
// Code for animations in the game
//
// authors:		 Hafiz
// contributors: 
//
//	Hafiz:
// - Animations
// - Effects (Glowing Characters) / Lose HP Effect
// 
//---------------------------------------------------------
// 
// Copyright(c) 2022 DigiPen Institute of Technology

#pragma once
#include "cprocessing.h"
#include <stdbool.h>
//For Bob Animations and Effects
void glowingBob(float x, float y, bool BobImmune);
void drawBob(float, float , bool, bool);
void lostHealth(int , int*);
void levelBoxesGlow(float x, float y, float w, float h);
void GlowingBob(float x, float y, float w, float h);
void pointsToReach(int);
void starRatings(int points);
