
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>


int planeX, planeY;
int enemyX, enemyY;
int bulletX, bulletY;
int score = 0, enemyHorizontalDirection=1,time0=0,time1=0,time2=0,time3=0;
int lives = 6;
int numBlocks = 0;
int blockX[5], blockY[5];
int specialItemX, specialItemY;
int timeout = 0;  // 


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
	specialItemX = -1;
	specialItemY = -1;
}

void moveEnemy(int timeout) {
	if (score >= 6||timeout==1) {
		if (score >= 9 && timeout == 0) {//當到9分
			time1++;
			if (time1 > 2) {//快的
				enemyY++;
				// 在達到底部時重新設定敵機位置
				if (enemyY > 20) {
					enemyX = rand() % 60 + 10;
					enemyY = 2;
				}
				time1 = 0;
			}
		} //當到9分
		time1++;
		if (time1 > 5) {//慢的
			enemyY++;
			// 在達到底部時重新設定敵機位置
			if (enemyY > 20) {
				//aaaaatimeout = 0;
				enemyX = rand() % 60 + 10;
				enemyY = 2;
			}
			time1 = 0;
		}
	}
	else if (score >= 3) {
		time0++;
		// 啟動敵機左右移動，改變水平移動方向
		if (time0 >2) {
			enemyX += enemyHorizontalDirection;
			time0 = 0;
		}
		// 在達到邊界時改變方向
		if (enemyX <= 0 || enemyX >= 70) {
			enemyHorizontalDirection = -enemyHorizontalDirection;
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
void moveBlocks(int highScore) {
	if (timeout == 1) {
		time3++;
		if (time3 > 3) {
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
						system("cls");
						printf("Game Over!\n");
						printf("High Score: %d  ", highScore);
						printf("Score: %d\n", score);
						system("pause");
					}
				}
				// 檢查方塊是否到達底部
				if (blockY[i] >= 23) {
					blockX[i] = rand() % 60 + 10;
					blockY[i] = 3;
				}
		}
			time3 = 0;
	}
	}
	else if (timeout == 0) {
		time3++;
		if (time3 > 1) {
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
						system("cls");
						printf("Game Over!\n");
						printf("High Score: %d  ", highScore);
						printf("Score: %d\n", score);
						system("pause");
					}
				}
				// 檢查方塊是否到達底部
				if (blockY[i] >= 23) {
					blockX[i] = rand() % 60 + 10;
					blockY[i] = 3;
				}
			}
			time3 = 0;
		}
	}
}

void saveHighScore(int highScore) {
	FILE* file = fopen("highscore.txt", "w");
	if (file != NULL) {
		fprintf(file, "%d", highScore);
		fclose(file);
	}
}

int loadHighScore() {
	FILE* file = fopen("highscore.txt", "r");
	int highScore = 0;

	if (file != NULL) {
		fscanf(file, "%d", &highScore);
		fclose(file);
	}

	return highScore;
}
void moveSpecialItem() {
	time2++;
	
	if (specialItemY != -1) {
		if (time2 > 3) {
			specialItemY++;

			// 檢查特殊道具是否與飛機發生碰撞
			if (planeX <= specialItemX && planeX + 3 >= specialItemX && planeY <= specialItemY + 1 && planeY + 2 >= specialItemY) {
				// 觸發特殊效果，這裡你可以根據需要加入不同的效果
				// 這邊示範的是增加一次射擊的功能
				specialItemX = -1;
				specialItemY = -1;
				bulletX = planeX + 2;
				bulletY = planeY - 1;
				timeout = 1;
				moveEnemy(timeout);  // 呼叫減慢敵機的函數
			}
			//timeout = 0;不能這樣寫好像沒用
			// 檢查特殊道具是否到達底部
			if (specialItemY >= 23) {
				specialItemX = -1;
				specialItemY = -1;
			}
			time2 = 0;
		}
	}
}
void generateSpecialItem() {
	if (score > 12) {//大於12分才開始落道具
		if (specialItemY == -1 && rand() % 100 < 5) {  // 5% 的機率產生特殊道具
			specialItemX = rand() % 60 + 10;
			specialItemY = 2;
		}
	}
}

void drawSpecialItem() {
	if (specialItemY != -1) {
		gotoxy(specialItemX, specialItemY);
		printf("@");
	}
}
int main() {
	char ch;
	int highScore = loadHighScore();
	system("cls");
	gameSetup();
	srand(time(NULL));

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
			timeout = 0;
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
				system("cls");
				printf("Game Over!\n");
				printf("High Score: %d  ", highScore);
				printf("Score: %d\n", score);
				system("pause");
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
		moveBlocks(highScore);
		for (int i = 0; i < numBlocks; i++) {
			drawBlock(i);
		}

		// 檢查是否與子彈發生碰撞
		for (int i = 0; i < numBlocks; i++) {
			drawBlock(i);
		}
		if (score > highScore) {
			highScore = score;
			saveHighScore(highScore);
		}
		moveEnemy(timeout);
		addBlock();
		generateSpecialItem();
		moveSpecialItem();  // 新增呼叫特殊道具移動函數
		drawSpecialItem();
		Sleep(50);
		system("cls");
	}//大迴圈
	system("cls");
	printf("Game Over!\n");
	printf("High Score: %d  ", highScore);
	printf("Score: %d\n", score);
	system("pause");
	return 0;
}
