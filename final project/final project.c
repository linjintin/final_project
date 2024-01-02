
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

int planeX, planeY;
int enemyX, enemyY;
int bulletX, bulletY;
int score = 0, enemyHorizontalDirection=1,time=0,time1=0;
int lives = 6;
int numBlocks = 0;
int blockX[5], blockY[5];

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawPlane() {
	gotoxy(planeX, planeY);
	printf("  ^  ");
	gotoxy(planeX, planeY + 1);
	printf(" /|\\ ");
	gotoxy(planeX, planeY + 2);
	printf("/ | \\");
}

void drawEnemy() {
	gotoxy(enemyX, enemyY);
	printf("VVV \n");
	gotoxy(enemyX, enemyY+1);
	printf(" V  \n");
}

void drawBullet() {
	gotoxy(bulletX, bulletY);//一直印出
	printf("|");
}

void drawBlock(int index) {
	gotoxy(blockX[index], blockY[index]);
	printf("*");
}

//void clearBullet() {
	//gotoxy(bulletX, bulletY);
	//printf(" ");
//}




void updateScore() {
	printf("Score: %d", score);
	Sleep(50);
}

void gameSetup() {
	planeX = 20;
	planeY = 20;
	enemyX = rand() % 60 + 10;
	enemyY = 2;
	bulletX = planeX + 1;
	bulletY = planeY - 1;

	numBlocks = 0;
}

void moveEnemy() {
	if (score >= 6) {
		if (score >= 9) {
			time1++;
			if (time1 > 2) {
				enemyY++;
				// 在達到底部時重新設定敵機位置
				if (enemyY > 20) {
					enemyX = rand() % 60 + 10;
					enemyY = 2;
				}
				time1 = 0;
			}
		}
		time1++;
		if (time1 > 3) {
			enemyY++;
			// 在達到底部時重新設定敵機位置
			if (enemyY > 20) {
				enemyX = rand() % 60 + 10;
				enemyY = 2;
			}
			time1 = 0;
		}
	}
	else if (score >= 3) {
		time++;
		// 啟動敵機左右移動，改變水平移動方向
		if (time >2) {
			enemyX += enemyHorizontalDirection;
			time = 0;
		}
		// 在達到邊界時改變方向
		if (enemyX <= 0 || enemyX >= 70) {
			enemyHorizontalDirection = -enemyHorizontalDirection;
		}
	}
}

void moveBlocks() {
	for (int i = 0; i < numBlocks; i++) {
		blockY[i]++;
		// 檢查是否與飛機發生碰撞
		if (blockX[i] >= planeX && blockX[i] <= planeX + 3 && blockY[i] >= planeY && blockY[i] <= planeY + 2) {
			lives--;
			if (lives > 0) {
				updateScore();
				blockX[i] = rand() % 60 + 10;
				blockY[i] = 3;
			}
			else {
				printf("\nGame Over!");
				exit(0);
			}
		}
		// 檢查方塊是否到達底部
		if (blockY[i] >= 20) {
			blockX[i] = rand() % 60 + 10;
			blockY[i] = 3;
		}
	}
}

void addBlock() {
	if (numBlocks < 3) {
		// 得到9分後每3分加一格
		if (score > 9 && (score - 9) % 3 == 0) {
			// 產生不靠近平面的隨機區塊位置
			do {
				blockX[numBlocks] = rand() % 60 + 10;
				blockY[numBlocks] = rand() % 15 + 3;
			} while (abs(blockX[numBlocks] - planeX) < 10 && abs(blockY[numBlocks] - planeY) < 5);

			numBlocks++;
		}
	}
}

int main() {
	char ch;

	system("cls");
	gameSetup();

	while (lives>0) {//大迴圈
		if (_kbhit()) {//函式檢測輸入
			ch = _getch();
			//printf("Key pressed: %c\n", ch);
			switch (ch) {
			case 'a':                         //注意大小寫
				if (planeX > 0) planeX--;
				break;
			case 'd':
				if (planeX < 70) planeX++;
				break;
			case ' ':
				bulletX = planeX + 2;
				bulletY = planeY - 1;
				break;
			case 'x':
				exit(0);
			}

		}
		printf("Score: %d Lives %d", score, lives);
		//clearBullet();
		if (bulletY > 0) {           //只要大於0就一直做
			bulletY--;               //y值越大越下面
			drawBullet();            //子彈軌跡
		}
		drawPlane();                 //繪製飛機
		drawEnemy();                 //繪製敵機


		if ((bulletX >= enemyX && bulletX < enemyX + 3 && bulletY >= enemyY && bulletY < enemyY + 2)) {
			score++;
			updateScore();
			enemyX = rand() % 60 + 10;
			enemyY = rand() % 4 + 1;
			Sleep(50);
		}
		if ((planeX <= enemyX + 2 && planeX + 3 >= enemyX) && (planeY <= enemyY + 1 && planeY + 2 >= enemyY)) {
			lives--;
			if (lives > 0) {
				updateScore();
				// 碰撞後重置敵人位置
				enemyX = rand() % 60 + 10;
				enemyY = rand() % 4 + 1;
			}
			else {
				printf("\nGame Over!");
				break;
			}
		}

		// Check for collision with block
		for (int i = 0; i < numBlocks; i++) {
			if (bulletX == blockX[i] && bulletY == blockY[i]) {
				// Bullet hit a block, stop bullet and remove the block
				bulletY = 0;
				for (int j = i; j < numBlocks - 1; j++) {
					blockX[j] = blockX[j + 1];
					blockY[j] = blockY[j + 1];
				}
				numBlocks--;
				updateScore();  
			}
		}

		// 繪製和移動區塊
		moveBlocks();
		for (int i = 0; i < numBlocks; i++) {
			drawBlock(i);
		}

		// 檢查是否與子彈發生碰撞
		for (int i = 0; i < numBlocks; i++) {
			drawBlock(i);
		}

		moveEnemy();
		addBlock();

		Sleep(50);
		system("cls");
	}//大迴圈

	return 0;
}