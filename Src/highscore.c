#include "highscore.h"
#include "gamelevel.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

void storescore(int currentLevel, int points, int *score) {
	int scorePoints = 0;
	int x = 0, i = 0;
	FILE *scoreboard = fopen("score.txt", "w+");
	//Get last game scores.
	while (1 == fscanf(scoreboard, "%d", &scorePoints)) {
		
		score[i++] = scorePoints;
	}

	//Replace current level score with new points if higher.
	score[currentLevel - 1] = (points > score[currentLevel - 1]) ? points : score[currentLevel - 1];
	do {
		fprintf(scoreboard, "%d\n", score[x]);
		x++;
	} while (x < 5);

	fclose(scoreboard);
}

/*
game level clear : new score "900"

5 levels so array of 5 size.

score[0] - score [4]

check if new score is higher then older score
if (newscore > oldscore) set newscore into score[level]

every level clear open file stream, replace new value (or just rewrite all data)
*/