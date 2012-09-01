#include "Base.h"
#include "Timer.h"
#include "Sound.h"
#include "Object.h"
#include <iostream>
#include <fstream>
#include <ctime>

bool systemOver = false;//ϵͳ����
ScreenSurface screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE, "iPIG", "images\\Icon.png");//��Surface

const int CHAR_W = 21;
const int CHAR_H = 20;//�ַ�
const int DELTA_CHAR_NUM = 2;//�ַ����
const int TOTAL_STAGE = 9;//�ܹؿ���

const int IPIG_INIT_X = SCREEN_WIDTH / 2;
const int IPIG_INIT_Y = SCREEN_HEIGHT / 4;//��ʼλ��
const int BAR_BKGRND_W = 320;
const int BAR_BKGRND_H = 40;
const int BAR_BKGRND_X = 0;
const int BAR_BKGRND_Y = SCREEN_HEIGHT - BAR_BKGRND_H;//״̬������λ��
const int BAR_W = 320;
const int BAR_H = 38;
const int BAR_X = 0;
const int BAR_Y = SCREEN_HEIGHT - BAR_H; //״̬��λ��
const int BUTTON_C_X = SCREEN_WIDTH / 2;
const int BUTTON_C_Y = GAME_H + BAR_BKGRND_H / 2;
const int BUTTON_C_R = 18;//��ťλ��
const int RESULT_W = 200, RESULT_H = 200;
const int RESULT_X = (SCREEN_WIDTH - RESULT_W) / 2;
const int RESULT_Y = (SCREEN_HEIGHT - BAR_BKGRND_H - RESULT_H) / 2;//ʤ��/ʧ��ǰ��λ��
const int ENTER_X = SCREEN_WIDTH / 2;
const int ENTER_Y = SCREEN_HEIGHT / 2 - CHAR_H;//�����λ��
const int HIGHSCORE_NAME_X = SCREEN_WIDTH / 2;
const int HIGHSCORE_NAME_Y = CHAR_H * 1.8;
const int HIGHSCORE_LEVEL_X = SCREEN_WIDTH / 2 - CHAR_W * 3;
const int HIGHSCORE_LEVEL_Y = CHAR_H * 3;
const int HIGHSCORE_SCORE_X = SCREEN_WIDTH / 2;
const int HIGHSCORE_SCORE_Y = CHAR_H * 3;//�߷�����\�ؿ���\����λ��

//��̬ͼƬ
StaticImage mainBackGround;//������
StaticImage backGround[TOTAL_STAGE];//�ؿ�����
StaticImage enterYourName;//�����ǰ��
StaticImage highScoreBackGround;//�߷�ǰ��
StaticImage resultWin;//ʤ������
StaticImage resultLose;//ʧ�ܻ���
StaticImage barBackGround;//״̬��
StaticImage pauseUnpressBar;//��ͣ(δ��)��ť 
StaticImage pausePressBar; //��ͣ(�Ѱ�)��ť
StaticImage playUnpressBar;//������(δ��)��ť
StaticImage playPressBar;//������(�Ѱ�)��ť
StaticImage gamePaused;//��ͣǰ��

//����Ч�йص�ȫ�ֱ�������
const int TOTAL_MUSIC = 5;//��������
MusicSound bgm[TOTAL_MUSIC];//����������
MusicSound mbgm;//����������
int musicNum = TOTAL_MUSIC;//������
EffectSound hitSE;//ײ����Ч
EffectSound deadSE;//������Ч
EffectSound landSE;//��½��Ч
EffectSound winSE;//ʤ����Ч

//��ʱ���йص�ȫ�ֱ�������
const int SCORE_HIT = -100;
const int PER_SECOND = 1000;//һ��
const int TRANS_FRAMES = 8;//����֡��
const int TIME_PER_FRAME_MIN = 10;//��Сÿ֡ʱ��
const int TIME_PER_FRAME = 40;//ÿ֡ʱ��
const int TIME_PER_STAGE = 15000;//ÿ��ʱ��
const int TIME_WIN = TIME_PER_STAGE * TOTAL_STAGE - TIME_PER_FRAME;//ʤ��ʱ��
const int TIME_ALARM = 1000;//ײ������ʱ��
int redTime = 0;//ײ���������ʱ��
int gameTime = 0;//��Ϸʱ��

std::ofstream fout("log.txt");//������־

Cloud cloudRight, cloudLeft;//�Ʊ���
SDL_Rect CloudRightRect, CloudLeftRect;//��ԴͼƬ
int cloudRightcH, cloudLeftcH;//��ѭ���߶�

double FORCE_ACC;//��Ҽ��ٶ�
double AIR_Y_K;//Y�����������
double AIR_X_K;//X�����������
SDL_Rect *rectsWalkRight,*rectsWalkLeft;
int totalRectsWalkRight, totalRectsWalkLeft;
SDL_Rect *rectsFlyRight, *rectsFlyLeft;
int totalRectsFlyRight,  totalRectsFlyLeft;
SDL_Rect *rectsHitRight, *rectsHitLeft;
int totalRectsHitRight,  totalRectsHitLeft;//����\����\ײ����ԴͼƬ
int stdWidth, stdHeight;//�����ײ���

const int TOTAL_FLOOR = 8; //Floor����
const int FLOOR_NUMBER_INIT = 10;//��ʼFloor����
const int FLOOR_NUMBER = 100;//Floorѭ�����г���
SDL_Rect FloorRect[TOTAL_FLOOR];//FloorԴͼƬ
SDL_Rect FloorRectBlink;//VANISH_FLOORԴͼƬ2
double MOVING_FLOOR_LOWER_SPEED_VELX;//MOVING_FLOOR_LOWER_SPEED���ٶ�
double MOVING_FLOOR_HIGHER_SPEED_VELX;//MOVING_FLOOR_HIGHER_SPEED���ٶ�
double ICE_FLOOR_VELM;//ICE_FLOOR�ĳ��ٶ�
double GRASS_FLOOR_VELM;//GRASS_FLOOR�ĳ��ٶ�

RGBSurface trans;//���丨��ͼƬ

const int SCORE_W = 20;
const int SCORE_H = 24;
const int SCORE_X = SCREEN_WIDTH / 2 + 60; 
const int SCORE_Y = BAR_Y + 10;//��ǰ����λ��
SDL_Rect NumberGreenRect[TOTAL_0_to_9];//����(��)ԴͼƬ
SDL_Rect NumberRedRect[TOTAL_0_to_9];//����(��)ԴͼƬ
SDL_Rect StringRect[TOTAL_ALPHABET + TOTAL_0_to_9 + 1];//�ַ�ԴͼƬ
SDL_Rect NumberLevelRect[TOTAL_0_to_9];//�ؿ���ԴͼƬ
Number score;//����
const int TOTAL_HIGHSCORE = 5;//�߷ָ���
Number highScoreScore[TOTAL_HIGHSCORE];//�߷ַ���
String highScoreName[TOTAL_HIGHSCORE];//�߷�����
Number highScoreLevel[TOTAL_HIGHSCORE];//�߷ֹؿ���
String name;//��ǰ����
Number stage;//��ǰ�ؿ���
SDL_Rect StageRect;//�ؿ�(LEVEL)ԴͼƬ

const int STAGE_STR_X = SCREEN_WIDTH / 2 - 150;
const int STAGE_STR_Y = SCORE_Y;
const int STAGE_NUM_X = SCREEN_WIDTH / 2 - 80;
const int STAGE_NUM_Y = SCORE_Y;//��ǰ�ؿ���
StaticImage stageStr;//�ؿ�

double SCREEN_VELY[TOTAL_STAGE];//�����ٶ�
int DIST_FLOOR_BASE;
int DIST_FLOOR_RANGE; //����Floor����
int ROLL_MIN[TOTAL_STAGE][TOTAL_FLOOR];
int ROLL_MAX[TOTAL_STAGE][TOTAL_FLOOR];//Floor���ּ���
double GRAVITY_ACC[TOTAL_STAGE];//�������ٶ�

double distance(int x1, int y1, int x2, int y2)
{
	return std::sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}//���뺯��

void readRect(std::istream& is, SDL_Rect* pRect)
{
	is >> pRect->x >> pRect->y >> pRect->w >> pRect->h;
}

SDL_Rect* readRect(std::istream& is, SDL_Rect pRect[], int& size)
{
	is >> size;
	pRect = new SDL_Rect[size];
	for (int i = 0; i != size; ++i)
		is >> pRect[i].x >> pRect[i].y >> pRect[i].w >> pRect[i].h;
	return pRect;
}

void readNote(std::istream& is) {
	std::string tmp;
	is >> tmp;
}

FLOOR_CLASS rollFloorClass(int nowStage)//ѡȡFloor���� {
{
	int dice = rand() % 100;
	for (int i = 0; i != TOTAL_FLOOR; ++i) {
		if (dice >= ROLL_MIN[nowStage][i] && dice < ROLL_MAX[nowStage][i])
			return FLOOR_CLASS(i);
	}
	return FLOOR_CLASS(0);
}

int rollFloorDist() {
	return DIST_FLOOR_BASE + rand() % DIST_FLOOR_RANGE;
}

void init() {//��ʼ��
	SDL_Surface *pSurface = 0;
	SDL_Surface *pTemp;
	if ((pTemp = IMG_Load("images\\Icon.png")) == 0)
		throw SDL_GetError();
	if ((pSurface = SDL_DisplayFormatAlpha(pTemp)) == 0)
		throw SDL_GetError();
	if (pTemp != 0)
		SDL_FreeSurface(pTemp);
	SDL_WM_SetIcon(pSurface, 0);

	trans = RGBSurface(screen, SCREEN_WIDTH, SCREEN_HEIGHT - BAR_BKGRND_H, SCREEN_BPP);

	std::ifstream fin;

	fin.open("data\\System.dat");
	for (int i = 0; i != TOTAL_STAGE; ++i) {
		std::string tmpStr = "images\\map";
		tmpStr += char(i + '1');
		tmpStr += ".png";
		backGround[i] = StaticImage(tmpStr, screen);
	}
	mainBackGround = StaticImage("images\\MainBackGround.jpg", screen);
	enterYourName = StaticImage("images\\EnterYourName.png", screen);
	highScoreBackGround = StaticImage("images\\HighScoreBackGround.png", screen);
	resultWin = StaticImage("images\\YouWin.png", screen, 0, RESULT_X, RESULT_Y);
	resultLose = StaticImage("images\\YouLose.png", screen, 0, RESULT_X, RESULT_Y);
	barBackGround = StaticImage("images\\BarBackGround.png", screen, 0, BAR_BKGRND_X, BAR_BKGRND_Y);
	pauseUnpressBar = StaticImage("images\\PauseUnpressBar.png", screen, 0, BAR_X, BAR_Y);
	pausePressBar = StaticImage("images\\PausePressBar.png", screen, 0, BAR_X, BAR_Y);
	playUnpressBar = StaticImage("images\\PlayUnpressBar.png", screen, 0, BAR_X, BAR_Y);
	playPressBar = StaticImage("images\\PlayPressBar.png", screen, 0, BAR_X, BAR_Y);
	gamePaused = StaticImage("images\\GamePaused.png", screen);
	fin.clear();
	fin.close();

	fin.open("data\\Cloud.dat");
	readRect(fin, &CloudRightRect);
	fin >> cloudRightcH;
	cloudRight = Cloud("images\\Cloud.png", screen, &CloudRightRect, SCREEN_WIDTH - CloudRightRect.w, 0, cloudRightcH);
	readRect(fin, &CloudLeftRect);
	fin >> cloudLeftcH;
	cloudLeft =  Cloud("images\\Cloud.png", screen, &CloudLeftRect, 0, 0, cloudLeftcH);
	fin.clear();
	fin.close();//������

	fin.open("data\\iPIG.dat");
	fin >> stdWidth >> stdHeight;
	rectsWalkRight = readRect(fin, rectsWalkRight, totalRectsWalkRight);
	rectsWalkLeft = readRect(fin, rectsWalkLeft, totalRectsWalkLeft);
	rectsFlyRight = readRect(fin, rectsFlyRight, totalRectsFlyRight);
	rectsFlyLeft = readRect(fin, rectsFlyLeft, totalRectsFlyLeft);
	rectsHitRight = readRect(fin, rectsHitRight, totalRectsHitRight);
	rectsHitLeft = readRect(fin, rectsHitLeft, totalRectsHitLeft);//�������
	fin.clear();
	fin.close();

	fin.open("data\\Floor.dat");
	for (int i = 0; i != TOTAL_FLOOR; ++i) {
		readRect(fin, FloorRect + i);
		if (i == 7)
			readRect(fin, &FloorRectBlink);
	}
	fin.clear();
	fin.close();//Floor����

	fin.open("data\\Level&Number.dat");
	readRect(fin, &StageRect);
	stageStr = StaticImage("images\\Level&Number.png", screen, &StageRect, STAGE_STR_X, STAGE_STR_Y);
	for (int i = 0; i != TOTAL_0_to_9; ++i)
		readRect(fin, NumberGreenRect + i);
	for (int i = 0; i != TOTAL_0_to_9; ++i)
		readRect(fin, NumberRedRect + i);
	score = Number("images\\Level&Number.png", screen, NumberGreenRect, NumberRedRect, SCORE_X, SCORE_Y);
	score.set(0);
	stage = Number("images\\Level&Number.png", screen, NumberGreenRect, NumberRedRect, STAGE_NUM_X, STAGE_NUM_Y);
	stage.set(1);
	fin.clear();
	fin.close();//�ؿ����뼰��ʼ��

	fin.open("data\\Level.dat");
	for (int i = 0; i != TOTAL_0_to_9; ++i)
		readRect(fin, NumberLevelRect + i);
	fin.clear();
	fin.close();//�ؿ�(LEVEL)���뼰��ʼ��

	fin.open("data\\Char.dat");
	for (int i = 0; i != TOTAL_ALPHABET + TOTAL_0_to_9 + 1; ++i)
		readRect(fin, &StringRect[i]);
	name = String("images\\Char.png", screen, StringRect, ENTER_X, ENTER_Y, "");
	fin.clear();
	fin.close();//�ַ�����

	//�����Ѷ�����
	fin.open("settings\\System.set");
	readNote(fin);
	for (int i = 0; i != TOTAL_STAGE; ++i) {
		int tmpStage;
		fin >> tmpStage >> SCREEN_VELY[i] >> GRAVITY_ACC[i];
		for (int j = 0; j != TOTAL_FLOOR; ++j) {
			int tmpCate;
			fin >> tmpCate >> ROLL_MIN[i][j] >> ROLL_MAX[i][j];
		}
	}
	readNote(fin);
	fin >> DIST_FLOOR_BASE;	readNote(fin);
	fin >> DIST_FLOOR_RANGE;readNote(fin);
	fin >> FORCE_ACC;		readNote(fin);
	fin >> AIR_Y_K;			readNote(fin);
	fin >> AIR_X_K;			readNote(fin);
	fin >> MOVING_FLOOR_LOWER_SPEED_VELX;	readNote(fin);
	fin >> MOVING_FLOOR_HIGHER_SPEED_VELX;	readNote(fin);
	fin >> ICE_FLOOR_VELM;	readNote(fin);
	fin >> GRASS_FLOOR_VELM;readNote(fin);
	fin.clear();
	fin.close();//�Ѷ��趨����

	bgm[0] = MusicSound("music\\MapMus1.wav");
	bgm[1] = MusicSound("music\\MapMus2.wav");
	bgm[2] = MusicSound("music\\MapMus3.wav");
	bgm[3] = MusicSound("music\\MapMus4.wav");
	bgm[4] = MusicSound("music\\MapMus5.wav");
	mbgm = MusicSound("music\\MainBackGround.wav");
	hitSE = EffectSound("SE\\Hit.wav");
	deadSE = EffectSound("SE\\Dead.wav");
	landSE = EffectSound("SE\\Land.wav");
	winSE = EffectSound("SE\\Win.wav");//������Ч����

	srand(time(0));//�����ʼ��
}

void game() {//��Ϸ����
	//Music
	mbgm.stop();
	if (musicNum != TOTAL_MUSIC)
		bgm[musicNum].stop();
	musicNum = rand() % TOTAL_MUSIC;
	bgm[musicNum].play();
	//Player
	IPIG iPIG("images\\iPIG.png", screen,
		rectsWalkRight, rectsWalkRight + totalRectsWalkRight,
		rectsWalkLeft, rectsWalkLeft + totalRectsWalkLeft,
		rectsFlyRight, rectsFlyRight + totalRectsFlyRight,
		rectsFlyLeft, rectsFlyLeft + totalRectsFlyLeft,
		rectsHitRight, rectsHitRight + totalRectsHitRight,
		rectsHitLeft, rectsHitLeft + totalRectsHitLeft,
		stdWidth, stdHeight,
		IPIG_INIT_X, IPIG_INIT_Y,
		AIR_X_K, AIR_Y_K);

	//Floor
	Floor floor[FLOOR_NUMBER];
	PFloor pFloorBeg(floor, floor, floor + FLOOR_NUMBER);
	PFloor pFloorEnd(floor, floor, floor + FLOOR_NUMBER);
	for (int i = 0; i != FLOOR_NUMBER_INIT; ++i) {//Initializing the Floors
		FLOOR_CLASS cFloor = ORDINARY_FLOOR_3_BLOCKS;
		double posX = rand() % (SCREEN_WIDTH - FloorRect[cFloor].w * 2) + (FloorRect[cFloor].w);
		double posY = 0.0;
		if (pFloorEnd != pFloorBeg)
			posY += (pFloorEnd - 1)->getPosY() + (FloorRect[(pFloorEnd - 1)->getClass()].h / 2);
		posY +=  rollFloorDist();
		*pFloorEnd = Floor("images\\Floor.png", screen, &(FloorRect[cFloor]), 0, posX, posY, 0.0, 0.0, 1.0, cFloor);
		++pFloorEnd;
	}
	//entering transition
	while (trans.getAlpha() != SDL_ALPHA_TRANSPARENT && !systemOver)
	{	
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) 
			systemOver = (gameEvent.type == SDL_QUIT);
		backGround[stage.get() - 1].show();
		cloudLeft.show();
		cloudRight.show();
		for (PFloor p = pFloorBeg; p != pFloorEnd; ++p)
			p->show();
		iPIG.show();
		trans.setAlpha(std::max(SDL_ALPHA_TRANSPARENT, trans.getAlpha() - SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		barBackGround.show();
		pauseUnpressBar.show();
		stageStr.show();
		stage.show();
		score.show();
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}

	//Timer
	Timer iPIGTimer;
	iPIGTimer.start();

	bool gameOver = false;
	gameTime = 0;
	while (!gameOver && !systemOver)
	{
		fout << std::endl <<  "TIME: " << iPIGTimer.get() << std::endl;
		backGround[stage.get() - 1].show();
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			systemOver = (gameEvent.type == SDL_QUIT);
			gameOver = (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_ESCAPE);
			if ((gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_p) 
				||(gameEvent.type == SDL_MOUSEBUTTONUP && distance(BUTTON_C_X, BUTTON_C_Y, gameEvent.motion.x, gameEvent.motion.y) <= BUTTON_C_R))	{	 
					if (iPIGTimer.isPaused()) 
						iPIGTimer.resume();
					else
						iPIGTimer.pause();
			}
			if (!(SDL_GetAppState() & SDL_APPINPUTFOCUS) && !iPIGTimer.isPaused())
				iPIGTimer.pause();
		}
		if (systemOver) return;//�˳�
		if (iPIGTimer.isPaused()) {//��ͣ
			backGround[stage.get() - 1].show();	
			gamePaused.show();
			barBackGround.show();
			if 	((gameEvent.type == SDL_KEYDOWN && gameEvent.key.keysym.sym == SDLK_s)
				||(gameEvent.type == SDL_MOUSEBUTTONDOWN && distance(BUTTON_C_X, BUTTON_C_Y, gameEvent.motion.x, gameEvent.motion.y) <= BUTTON_C_R))
				playPressBar.show();
			else
				playUnpressBar.show();	
			screen.flip();
		}
		else {
			gameTime += TIME_PER_FRAME;
			stage.set(int(gameTime / TIME_PER_STAGE) + 1);	
			backGround[stage.get() - 1].show();

			//����ת��
			int opaqueTime = stage.get() * TIME_PER_STAGE;
			int lastOpaqueTime = opaqueTime - TIME_PER_STAGE;
			if (opaqueTime - gameTime < PER_SECOND)
				trans.setAlpha(SDL_ALPHA_OPAQUE - SDL_ALPHA_OPAQUE * (opaqueTime - gameTime) / PER_SECOND);
			if (stage.get() != 1 && gameTime - lastOpaqueTime < PER_SECOND)
				trans.setAlpha(std::max(SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE - SDL_ALPHA_OPAQUE * (gameTime - lastOpaqueTime) / PER_SECOND));

			if (trans.getAlpha() != SDL_ALPHA_TRANSPARENT)
				trans.show();

			//�ƹ���Ļ
			cloudRight.move(0, SCREEN_VELY[stage.get() - 1] / 2);
			cloudLeft.move(0, SCREEN_VELY[stage.get() - 1] / 2);
			for (PFloor p = pFloorBeg; p != pFloorEnd; ++p)
				p->move(0, SCREEN_VELY[stage.get() - 1]);
			iPIG.move(0, SCREEN_VELY[stage.get() - 1]);

			//��ɫ״̬�ı�
			Uint8 *k;
			k = SDL_GetKeyState(0);
			double fX = 0, fY = GRAVITY_ACC[stage.get() - 1];
			if (k[SDLK_LEFT]) fX -= FORCE_ACC;
			if (k[SDLK_RIGHT]) fX += FORCE_ACC;
			iPIG.change(fX, fY, pFloorBeg, pFloorEnd);

			//��ײ����Ļ����
			if (iPIG.isHit()) {
				hitSE.play();
				score += SCORE_HIT;
				score.changeRed();
				redTime = gameTime + TIME_ALARM; 
			}
			//��½
			if (iPIG.isLand())
				landSE.play();

			//�������
			if (iPIG.getPosY() > 60) {
				score += (int(iPIG.getPosY() / 60));
				score.changeGreen();
			}
			//��ʾͼƬ
			cloudRight.show();
			cloudLeft.show();
			for (PFloor p = pFloorBeg; p != pFloorEnd; ++p) {
				if (p->getPosY() < 0 && p == pFloorBeg) 
					++pFloorBeg;
				else 
					p->show();
			}
			//����Floor
			if ((pFloorEnd - 1)->getPosY() < SCREEN_HEIGHT) {
				FLOOR_CLASS cFloor = rollFloorClass(stage.get() - 1);
				double posX  = rand() % (SCREEN_WIDTH - FloorRect[cFloor].w * 2) + FloorRect[cFloor].w;
				double posY  = SCREEN_HEIGHT + rollFloorDist();
				double velX = 0.0;
				double velM = 1.0;
				if (cFloor == MOVING_FLOOR_LOWER_SPEED)
					velX = MOVING_FLOOR_LOWER_SPEED_VELX;
				if (cFloor == MOVING_FLOOR_HIGHER_SPEED)
					velX = MOVING_FLOOR_HIGHER_SPEED_VELX;
				if (cFloor == ICE_FLOOR)
					velM = ICE_FLOOR_VELM;
				if (cFloor == GRASS_FLOOR)
					velM = GRASS_FLOOR_VELM;
				*pFloorEnd = 
					Floor("images\\Floor.png", screen, &FloorRect[cFloor], (cFloor == VANISH_FLOOR ? &FloorRectBlink : 0),
					posX, posY, velX, 0.0, velM, cFloor);
				++pFloorEnd;
			}
			iPIG.show();

			//��ʾϵͳ״̬��
			barBackGround.show();
			if 	((gameEvent.type == SDL_KEYDOWN && gameEvent.key.keysym.sym == SDLK_s)
				||(gameEvent.type == SDL_MOUSEBUTTONDOWN && distance(BUTTON_C_X, BUTTON_C_Y, gameEvent.motion.x, gameEvent.motion.y) <= BUTTON_C_R))
				pausePressBar.show();
			else
				pauseUnpressBar.show();
			stageStr.show();
			stage.show();
			score.show();
			screen.flip();

			//ʤ������
			if (iPIG.getPosY() >= GAME_H + stdHeight) {
				deadSE.play();
				if (bgm[musicNum].playing())
					bgm[musicNum].stop();
				gameOver = true;
			}
			if (gameTime >= TIME_WIN)  {
				winSE.play();
				if (bgm[musicNum].playing())
					bgm[musicNum].stop();
				break;
			}
			SDL_Delay(std::max(gameTime - iPIGTimer.get(), 0));
		}
	}
	//ʤ����ʾ
	if (!gameOver && gameTime >= TIME_WIN)
		resultWin.show();
	else
		resultLose.show();
	screen.flip();
	if (!systemOver)
		SDL_Delay(PER_SECOND);
	bool pressAnyKey = false;
	while (!pressAnyKey && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			systemOver = (gameEvent.type == SDL_QUIT);
			if (gameEvent.type == SDL_KEYUP) 
				pressAnyKey = true;
		}
		SDL_Delay(TIME_PER_FRAME);
	}

	//�˳�����
	while (trans.getAlpha() != SDL_ALPHA_OPAQUE && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			systemOver = (gameEvent.type == SDL_QUIT);
		}
		backGround[stage.get() - 1].show();
		cloudLeft.show();
		cloudRight.show();
		for (PFloor p = pFloorBeg; p != pFloorEnd; ++p)
			p->show();
		if (!gameOver)
			resultWin.show();
		else
			resultLose.show();
		trans.setAlpha(std::min(SDL_ALPHA_OPAQUE, trans.getAlpha() + SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		barBackGround.show();
		pauseUnpressBar.show();
		stageStr.show();
		stage.show();
		score.show();
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}
}

void entername()//�������
{	
	int insertPos = TOTAL_HIGHSCORE;
	//�ж��Ƿ������߷�
	std::ifstream fin;
	fin.open("data\\HighScore.dat");
	for (int i = 0; i != TOTAL_HIGHSCORE; ++i)
	{
		std::string nName;
		int nLevel, nScore;
		fin >> nName >> nLevel >> nScore;
		highScoreName[i] = String("images\\Char.png", screen, StringRect, 0, 0, nName);
		highScoreLevel[i] = Number("images\\Level.png", screen, NumberLevelRect, 0, 0, 0, nLevel);
		highScoreScore[i] = Number("images\\Level&Number.png", screen, NumberGreenRect, NumberRedRect, 0, 0, nScore);
		if (insertPos == TOTAL_HIGHSCORE && score.get() > nScore)
			insertPos = i;
	}
	fin.clear();
	fin.close();
	if (insertPos == TOTAL_HIGHSCORE) return;

	//�������
	while (trans.getAlpha() != SDL_ALPHA_TRANSPARENT && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent))
			systemOver = (gameEvent.type == SDL_QUIT);
		backGround[stage.get() - 1].show();
		enterYourName.show();
		trans.setAlpha(std::max(SDL_ALPHA_TRANSPARENT, trans.getAlpha() - SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}

	//��������
	if (!mbgm.playing())
		mbgm.play();
	name.clear();
	bool gameOver = false;
	while (!gameOver && !systemOver) {
		backGround[stage.get() - 1].show();
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			systemOver = (gameEvent.type == SDL_QUIT);
			gameOver = (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_RETURN);
			if (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym >= SDLK_a && gameEvent.key.keysym.sym <= SDLK_z)
				name.append(char(gameEvent.key.keysym.sym));
			if (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym >= SDLK_0 && gameEvent.key.keysym.sym <= SDLK_9)
				name.append(char(gameEvent.key.keysym.sym));
			if (gameEvent.type == SDL_KEYUP && gameEvent.key.keysym.sym == SDLK_BACKSPACE)
				name.backSpace();
		}
		enterYourName.show();
		name.show();
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}

	//�����ļ�
	std::ofstream fout;
	fout.open("data\\HighScore.dat");	
	for (int i = 0; i != insertPos; ++i)
		fout << highScoreName[i].get() << std::endl << highScoreLevel[i].get() << ' ' << highScoreScore[i].get() << std::endl;
	fout << name.get() << std::endl << (gameTime >= TIME_WIN ? 0 : stage.get()) << ' ' << score.get() << std::endl;
	for (int i = insertPos; i != TOTAL_HIGHSCORE - 1; ++i)
		fout << highScoreName[i].get() << std::endl << highScoreLevel[i].get() << ' ' << highScoreScore[i].get() << std::endl;
	fout.clear();
	fout.close();

	//�˳�����
	while (trans.getAlpha() != SDL_ALPHA_OPAQUE && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent))
			systemOver = (gameEvent.type == SDL_QUIT);
		backGround[stage.get() - 1].show();
		enterYourName.show();
		name.show();
		trans.setAlpha(std::min(SDL_ALPHA_OPAQUE, trans.getAlpha() + SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}
}

void highscore()//�߷ֹ���
{
	//�����ļ�
	std::ifstream fin;
	fin.open("data\\HighScore.dat");
	for (int i = 0; i != TOTAL_HIGHSCORE; ++i) {
		std::string nName;
		int nLevel, nScore;
		fin >> nName >> nLevel >> nScore;
		highScoreName[i] = String
			("images\\Char.png", screen, StringRect, HIGHSCORE_NAME_X, HIGHSCORE_NAME_Y + CHAR_H * 2 * i, nName);
		highScoreLevel[i] = Number
			("images\\Level.png", screen, NumberLevelRect, 0, HIGHSCORE_LEVEL_X, HIGHSCORE_LEVEL_Y + CHAR_H * 2 * i - DELTA_CHAR_NUM, nLevel);
		highScoreScore[i] = Number
			("images\\Level&Number.png", screen, NumberGreenRect, NumberRedRect, HIGHSCORE_SCORE_X, HIGHSCORE_SCORE_Y + CHAR_H * 2 * i - DELTA_CHAR_NUM, nScore);
	}
	fin.clear();
	fin.close();

	while (trans.getAlpha() != SDL_ALPHA_TRANSPARENT && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent))
			systemOver = (gameEvent.type == SDL_QUIT);
		int D = 3;//������
		backGround[stage.get() - 1].show();
		highScoreBackGround.show();
		for (int i = 0; i != TOTAL_HIGHSCORE; ++i) {
			highScoreName[i].show();
			highScoreLevel[i].show();
			highScoreScore[i].show();
		}
		trans.setAlpha(std::max(SDL_ALPHA_TRANSPARENT, trans.getAlpha() - SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}
	if (bgm[musicNum].playing() && !mbgm.playing())
		bgm[musicNum].stop();
	if (!mbgm.playing())
		mbgm.play();
	name.clear();
	bool gameOver = false;
	while (!gameOver && !systemOver) {
		backGround[stage.get() - 1].show();
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent)) {
			systemOver = (gameEvent.type == SDL_QUIT);
			gameOver = (gameEvent.type == SDL_KEYUP);
		}
		highScoreBackGround.show();
		for (int i = 0; i != TOTAL_HIGHSCORE; ++i) {
			highScoreName[i].show();
			highScoreLevel[i].show();
			highScoreScore[i].show();
		}
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}

	while (trans.getAlpha() != SDL_ALPHA_OPAQUE && !systemOver) {
		SDL_Event gameEvent;
		while (SDL_PollEvent(&gameEvent))
			systemOver = (gameEvent.type == SDL_QUIT);
		backGround[stage.get() - 1].show();
		highScoreBackGround.show();
		for (int i = 0; i != TOTAL_HIGHSCORE; ++i) {
			highScoreName[i].show();
			highScoreLevel[i].show();
			highScoreScore[i].show();
		}
		trans.setAlpha(std::min(SDL_ALPHA_OPAQUE, trans.getAlpha() + SDL_ALPHA_OPAQUE / TRANS_FRAMES));
		trans.show();	
		screen.flip();
		SDL_Delay(TIME_PER_FRAME);
	}

}

int main(int argc, char* argv[]) {
	try {
		init();	
		bool gameStarted = false;
		while (!systemOver) {
			SDL_Event systemEvent;
			while (SDL_PollEvent(&systemEvent)) {
				systemOver = 
					(systemEvent.type == SDL_QUIT 
					|| systemEvent.type == SDL_KEYUP && systemEvent.key.keysym.sym == SDLK_ESCAPE);
				if	((systemEvent.type == SDL_KEYUP && systemEvent.key.keysym.sym == SDLK_s)
					||(systemEvent.type == SDL_MOUSEBUTTONUP && distance(BUTTON_C_X, BUTTON_C_Y, systemEvent.motion.x, systemEvent.motion.y) <= BUTTON_C_R))
					gameStarted = true;
			}
			if (gameStarted) {
				while (trans.getAlpha() != SDL_ALPHA_OPAQUE && !systemOver) {
					mainBackGround.show();
					trans.setAlpha(std::min(SDL_ALPHA_OPAQUE, trans.getAlpha() + SDL_ALPHA_OPAQUE / TRANS_FRAMES));
					trans.show();
					barBackGround.show();
					playPressBar.show();
					stageStr.show();
					stage.show();
					score.show();
					screen.flip();
					SDL_Delay(TIME_PER_FRAME);
				}
				score.set(0);
				stage.set(1);//��¼����
				game();
				entername();
				highscore();
				while (trans.getAlpha() != SDL_ALPHA_TRANSPARENT && !systemOver) {
					mainBackGround.show();
					trans.setAlpha(std::max(SDL_ALPHA_TRANSPARENT, trans.getAlpha() - SDL_ALPHA_OPAQUE / TRANS_FRAMES));
					trans.show();
					screen.flip();
					SDL_Delay(TIME_PER_FRAME);
				}
				gameStarted = false;
			}
			else {
				if (!mbgm.playing())
					mbgm.play();
				mainBackGround.show();
				barBackGround.show();
				if 	((systemEvent.type == SDL_KEYDOWN && systemEvent.key.keysym.sym == SDLK_s)
					||(systemEvent.type == SDL_MOUSEBUTTONDOWN && distance(BUTTON_C_X, BUTTON_C_Y, systemEvent.motion.x, systemEvent.motion.y) <= BUTTON_C_R))
					playPressBar.show();
				else
					playUnpressBar.show();
				stageStr.show();
				stage.show();
				score.show();
				screen.flip();
			}
			SDL_Delay(TIME_PER_FRAME_MIN);
		}
	}
	catch (const char* s) {
		std::cerr << s << std::endl;
		SDL_Quit();
		fout.close();
		return -1;
	}
	SDL_Quit();
	fout.close();
	return 0;
}
