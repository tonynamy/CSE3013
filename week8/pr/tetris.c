#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	for(int i=0;i<BLOCK_NUM;i++) {
		nextBlock[i] = rand() % 7;
	}

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
	recommendX=recommendY=recommendR=0;

	recRoot = (RecNode*) malloc(sizeof(RecNode));
	recRoot->level = 1;
	recRoot->accumulatedScore = 0;
	memcpy(recRoot->recField, field, sizeof(field));

	recommend(recRoot);

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	move(9,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j, num;
	for(num=1;num<VISIBLE_BLOCKS;num++) {
		for( i = 0; i < 4; i++ ){
			move(4+(num-1)*6+i,WIDTH+13);
			for( j = 0; j < 4; j++ ){
				if( block[nextBlock[num]][0][i][j] == 1 ){
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}
				else printw(" ");
			}
		}
	}
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	for(int i=0;i<BLOCK_HEIGHT;i++) {
		for(int j=0;j<BLOCK_WIDTH;j++) {
			if(i+blockY < 0 || block[currentBlock][blockRotate][i][j] == 0) continue;
			if(i+blockY >= HEIGHT || j+blockX >= WIDTH || j+blockX < 0 || f[i+blockY][j+blockX] == 1) return 0;
		}
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 

	int oldBlockRotate = blockRotate;
	int oldBlockY = blockY;
	int oldBlockX = blockX;

	if(command==KEY_UP) {
		oldBlockRotate = (4+blockRotate-1)%4;
	}
	else if(command==KEY_DOWN) {
		oldBlockY--;
	}
	else if(command==KEY_RIGHT) {
		oldBlockX--;
	} else if(command==KEY_LEFT) {
		oldBlockX++;
	}

	for(int i=0;i<BLOCK_HEIGHT;i++) {
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[currentBlock][oldBlockRotate][i][j]==1 && i+oldBlockY>=0){
				move(i+oldBlockY+1,j+oldBlockX+1);
				printw("%c",'.');
			}
		}
	}

	while(CheckToMove(field, currentBlock, oldBlockRotate, ++oldBlockY, oldBlockX));

	oldBlockY--;

	for(int i=0;i<BLOCK_HEIGHT;i++) {
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[currentBlock][oldBlockRotate][i][j]==1 && i+oldBlockY>=0){
				move(i+oldBlockY+1,j+oldBlockX+1);
				printw("%c",'.');
			}
		}
	}

	move(HEIGHT,WIDTH+10);

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.

	timed_out = 0;

	if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
		return;
	}

	if(blockY < 0) {
		gameOver = 1;
	}

	score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);

	score += DeleteLine(field);

	for(int i=0;i<BLOCK_NUM-1;i++) {
		nextBlock[i] = nextBlock[i+1];
	}

	nextBlock[BLOCK_NUM-1] = rand() % 7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;

	memcpy(recRoot->recField, field, sizeof(field));

	recommend(recRoot);
	
	DrawNextBlock(nextBlock);
	PrintScore(score);

	while(!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) {
		blockY--;
	}

	DrawField();
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.

	int touched = 0;

	for(int i=0;i<BLOCK_HEIGHT;i++) {
		for(int j=0;j<BLOCK_WIDTH;j++) {
			if(block[currentBlock][blockRotate][i][j] == 0) {
				continue;
			}
			//if(i==BLOCK_HEIGHT-1 || block[currentBlock][blockRotate][i+1][j] == 1) {
			if(block[currentBlock][blockRotate][i+1][j] == 1) {
				touched++;
			}
			f[i+blockY][j+blockX] = block[currentBlock][blockRotate][i][j];
		}
	}

	return touched*10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

	int cnt=0;

	int i=0;

	while(i<HEIGHT) {

		int flag = 1;

		for(int j=0;j<WIDTH;j++) {
			if(f[i][j]==0) {
				flag = 0;
				break;
			}
		}

		if(flag == 1) {

			for(int ii=i;ii>=0;ii--) {
				for(int j=0;j<WIDTH;j++) {
					if(ii==0) {
						f[ii][j] = 0;
					} else {
						f[ii][j] = f[ii-1][j];
					}
				}
			}
			cnt++;
		}
		i++;
	}

	return cnt*cnt*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code

	while(CheckToMove(field, blockID, blockRotate, ++y, x));

	y--; 

	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int _score_number;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	if (!fp || fscanf(fp, "%d", &_score_number) == EOF) {
		score_number = 0;
		rankRoot = NULL;
		return;
	}
	else {

		score_number = _score_number;

		char _rank_name[NAMELEN];
		int _rank_score;

		RankNode* cur = NULL;

		while(fscanf(fp, "%s %d", _rank_name, &_rank_score) != EOF) {

			RankNode* node = (RankNode*) malloc(sizeof(RankNode));
			node->rank_score = _rank_score;
			strcpy(node->rank_name, _rank_name);
			node->link = NULL;

			if(cur) {
				cur->link = node;
			} else {
				rankRoot = node;
			}

			cur = node;

		}

	}
	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력
	move(0, 0);
	printw("1. list ranks from X to Y");
	move(1, 0);
	printw("2. list ranks by a specific name");
	move(2, 0);
	printw("3. delete a specific rank");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장

	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	createRankList();

	char* nameString = "name";
	char* scoreString = "score";
	int nameLen = 25;
	int scoreLen = 15;
	int namePad = (nameLen - strlen(nameString)) / 2;
	int scorePad = (scoreLen - strlen(scoreString)) / 2;

	if (ch == '1') {

		mvprintw(3, 0, "X: ");
		echo();
		if(scanw("%d", &ch)!=EOF) {
			X = ch;
		}
		noecho();

		mvprintw(4, 0, "Y: ");
		echo();
		if(scanw("%d", &ch)!=EOF) {
			Y = ch;
		}
		noecho();

    	mvprintw(5, 0, "%*s%s%*s | %*s%s%*s\n", namePad, "", nameString, namePad, "", scorePad, "", scoreString, scorePad, "");
		for(i=0;i<nameLen+scoreLen+1;i++) {
			printw("-");
		}

		if(X>Y) {
			move(8, 0);
			printw("search failure: no rank in the list");
		} else {

			RankNode* cur = rankRoot;

			for(i=1;i<X&&cur;i++) {
				cur = cur->link;
			}

			for(i=X;i<=Y&&cur;i++) {
				mvprintw(7+i-X, 0, "%s%*s | %d", cur->rank_name, nameLen-strlen(cur->rank_name)-1, "", cur->rank_score);
				cur = cur->link;
			}
		}

	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;

		mvprintw(3, 0, "input the name: ");
		echo();
		getstr(str);
		noecho();

    	mvprintw(5, 0, "%*s%s%*s | %*s%s%*s\n", namePad, "", nameString, namePad, "", scorePad, "", scoreString, scorePad, "");
		for(i=0;i<nameLen+scoreLen+1;i++) {
			printw("-");
		}

		RankNode* cur = rankRoot;

		i=0;

		while(cur) {
			if(strcmp(str, cur->rank_name) == 0) {
				mvprintw(7+i++, 0, "%s%*s | %d", cur->rank_name, nameLen-strlen(cur->rank_name)-1, "", cur->rank_score);
				check = 1;
			}
			cur = cur->link;
		}

		if(!check) {
			mvprintw(8, 0, "search failure: no name in the list");
		}


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;

		mvprintw(3, 0, "input the rank: ");
		echo();
		scanw("%d", &num);
		noecho();

		if(num < 1 || num > score_number) {
			mvprintw(5, 0, "search failure: the rank not in the list");
		} else {

			RankNode* prev = NULL;
			RankNode* cur = rankRoot;

			for(i=1;i<num;i++) {
				prev = cur;
				cur = cur->link;
			}

			if(prev == NULL) {
				rankRoot = cur->link;
			} else {
				prev->link = cur->link;
			}

			free(cur);

			score_number--;

			writeRankFile();

			mvprintw(5, 0, "result: the rank deleted");

		}

	}
	getch();

}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	RankNode* cur = rankRoot;

	while(cur) {
		fprintf(fp, "%s %d\n", cur->rank_name, cur->rank_score);
		RankNode* tmp = cur;
		cur = cur->link;
		free(tmp);
	}

	rankRoot = NULL;

	fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	printw("your name: ");
	echo();
	getstr(str);
	noecho();

	createRankList();

	RankNode* prev = NULL;
	RankNode* cur = rankRoot;

	RankNode* node = (RankNode*) malloc(sizeof(RankNode));
	node->rank_score = score;
	strcpy(node->rank_name, str);
	node->link = NULL;

	if(cur == NULL) {
		rankRoot = node;
	} else {

		while(cur) {

			if(cur->rank_score <= score) {

				if(prev) {
					prev->link = node;
				} else {
					rankRoot = node;
				}

				node->link = cur;
				
				break;

			}

			prev = cur;
			cur = cur->link;
		}

		if(!cur) {
			prev->link = node;
		}

	}

	score_number++;

	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	// user code
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode *root){
	int max=-1; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int i=0;

	root->child = (RecNode**) malloc(sizeof(RecNode*)*CHILDREN_MAX);

	// user code
	for(int r=0;r<NUM_OF_ROTATE;r++) {

		for(int x=-BLOCK_WIDTH;x<=WIDTH+BLOCK_WIDTH;x++) {
			
			int y=-1;

			while(CheckToMove(root->recField, nextBlock[root->level-1], r, ++y, x));
			y--;
			if(y<0) continue;

			RecNode* child = (RecNode*) malloc(sizeof(RecNode));
			root->child[i++] = child;

			memcpy(child->recField, root->recField, sizeof(root->recField));

			child->accumulatedScore = root->accumulatedScore;
			child->accumulatedScore += AddBlockToField(child->recField, nextBlock[root->level-1], r, y, x);
			child->accumulatedScore += DeleteLine(child->recField);

			child->level = root->level+1;

			if(root->level < BLOCK_NUM) {
				child->accumulatedScore += recommend(child);
			}

			if(child->accumulatedScore > max) {
				max = child->accumulatedScore;

				if(root->level == 1) {
					recommendR = r;
					recommendX = x;
					recommendY = y;
				}
			}
			
			free(child);

		}

	}

	free(root->child);

	return max;
}

void recommendedPlay(){
	// user code
}
