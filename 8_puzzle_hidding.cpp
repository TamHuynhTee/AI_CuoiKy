#include<conio.h>
#include<iostream>
#include<time.h>-
#include<Windows.h>
using namespace std;

#define SIDE 3
enum Actions { LEFT, RIGHT, UP, DOWN };
int GOAL_A[SIDE][SIDE] = { 1,2,3,8,0,4,7,6,5 };
int GOAL_B[SIDE][SIDE] = { 0,1,2,3,4,5,6,7,8 };
int START_DEMO4[SIDE][SIDE] = { 8,3,2,4,0,1,6,5,7 };
int Display_Col = 0;

typedef struct Node_array
{
	int state[SIDE][SIDE];
	Actions action;
	struct Node_array* parent; // tree search

	int path_cost;
	int depth;
	struct Node_array* nextNode; // list

} Node;

typedef struct Node_List {
	unsigned int nodeCount;
	Node* head;
	Node* tail;
}NodeList;

Node* newNode()
{
	Node* temp = new Node();
	temp->parent = temp->nextNode = NULL;
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			temp->state[i][j] = -1;
	return temp;
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void Switch (int state[][SIDE], int i, int j, int x, int y)
{
	int temp = state[i][j];
	state[i][j] = state[x][y];
	state[x][y] = temp;
}

void state_Copy(int root[][SIDE], int goal[][SIDE])
{
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			goal[i][j] = root[i][j];
}

int random(int min, int max)
{
	return min + rand() % (max + 1 - min);
}

bool existedIn_Arr(int arr[][SIDE], int r)
{
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (arr[i][j] == r)
				return true;
	return false;
}

void state_Input(int state[][SIDE])
{
	srand((int)time(0)); int r;
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
		{
			do
				r = random(0, 8);
			while (existedIn_Arr(state, r));
			state[i][j] = r;
		}
}

void state_Output(int state[][SIDE])
{
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
			if (state[i][j] == 0)
				printf("    | ");
			else
			printf("%2d  | ", state[i][j]);
		printf("\n");
		for (int j = 0; j < SIDE; j++)
			printf("----|-");
		printf("\n");
	} printf("\n");
}
// Print the final Start and Goal
void print_Start_Result(int temp[][SIDE], int start[][SIDE], int y)
{
	int x = 0;
	gotoxy(x, y);
	printf("Start State:");
	y++;
	gotoxy(x, y);
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
			if (temp[i][j] == 0)
				printf("    | ");
			else
				printf("%2d  | ", temp[i][j]);
		y++; gotoxy(x, y);
		for (int j = 0; j < SIDE; j++)
			printf("----|-");
		y++; gotoxy(x, y);
	}
	int temp_x = x + 19;
	int temp_y = y - 4;
	gotoxy(temp_x, temp_y);
	printf("-->");
	x = SIDE * 8;
	y -= SIDE * SIDE - 2;
	gotoxy(x, y);
	printf("Goal State:");
	y++;
	gotoxy(x, y);
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
			if (start[i][j] == 0)
				printf("    | ");
			else
				printf("%2d  | ", start[i][j]);
		y++; gotoxy(x, y);
		for (int j = 0; j < SIDE; j++)
			printf("----|-");
		y++; gotoxy(x, y);
	}gotoxy(0, y);
}
// Test the hidding Start
void print_Test(int temp[][SIDE], int start[][SIDE], int y)
{
	int x = 0;
	gotoxy(x, y);
	printf("Temp State:");
	y++;
	gotoxy(x, y);
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
			if(temp[i][j] == 0)
				printf("    | ");
			else if (temp[i][j] != -1)
				printf("%2d  | ", temp[i][j]);
			else
				printf(" .  | ");
		y++; gotoxy(x, y);
		for (int j = 0; j < SIDE; j++)
			printf("----|-");
		y++; gotoxy(x, y);
	}
	x = SIDE * 8;
	y -= SIDE * SIDE - 2;
	gotoxy(x, y);
	printf("Start:");
	y++;
	gotoxy(x, y);
	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
			printf("%2d  | ", start[i][j]);
		y++; gotoxy(x, y);
		for (int j = 0; j < SIDE; j++)
			printf("----|-");
		y++; gotoxy(x, y);
	}gotoxy(0, y);
	printf("____________________________________________**\n");
}

bool state_Configuration(int start[][SIDE])// Calculate to decide which GOAL can be reached
{
	int sum = 0;
	int temp[9];
	int count = 0;
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
		{
			temp[count++] = start[i][j];
		}
	for (int i = 0; i < count - 1; i++)
	{
		if (temp[i] != 0)
		{
			int check = temp[i];
			for (int j = i + 1; j < count; j++)
				if (temp[j] != 0 && check > temp[j])
					sum++;
		}
	}
	return sum % 2;
}

void decide_GOAL(int goal[][SIDE], bool decide)
{
	if (decide == 1)
	{
		for (int i = 0; i < SIDE; i++)
			for (int j = 0; j < SIDE; j++)
				goal[i][j] = GOAL_A[i][j];
	}
	else
	{
		for (int i = 0; i < SIDE; i++)
			for (int j = 0; j < SIDE; j++)
				goal[i][j] = GOAL_B[i][j];
	}
}

void find_Smallest(int state[][SIDE], int &x, int &y)
{
	int min = 8;
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (state[i][j] != 0 && state[i][j] != -1 && state[i][j] < min)
			{
				min = state[i][j];
				x = i;
				y = j;
			}
}
//Step 1
void state_Change_and_Print(int temp[][SIDE], int start[][SIDE], int i, int j, int x, int y)/*Main switching function*/
{
	Switch(temp, i, j, x, y); Switch(start, i, j, x, y);
	print_Test(temp, start, Display_Col); Display_Col += 9;
}

void find_Zero(int &x, int &y, int start[][SIDE])
{
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (start[i][j] == 0)
			{
				x = i; y = j;
			}
}

void make_Zero_center(int temp[][SIDE], int start[][SIDE])
{
	int i, j;
	find_Zero(i, j, start);
	temp[i][j] = 0;
	print_Test(temp, start, Display_Col); Display_Col += 9;
	while (i != 1 || j != 1)
	{
		_getch();
		if (j == 0)
		{
			int oldJ = j; j++;
			Switch(temp, i, j, i, oldJ); Switch(start, i, j, i, oldJ);
			if (i == 1)
				temp[1][1] = 0;
			print_Test(temp, start, Display_Col); Display_Col += 9;
		}
		else
			if (j == 2)
			{
				int oldJ = j; j--;
				Switch(temp, i, j, i, oldJ); Switch(start, i, j, i, oldJ);
				if (i == 1)
					temp[1][1] = 0;
				print_Test(temp, start, Display_Col); Display_Col += 9;
			}
			else
				if (i == 0)
				{
					int oldI = i; i++; 
					Switch(temp, i, j, oldI, j); Switch(start, i, j, oldI, j);
					temp[1][1] = 0;
					print_Test(temp, start, Display_Col); Display_Col += 9;
				}
				else
					if (i == 2)
					{
						int oldI = i; i--;
						Switch(temp, i, j, oldI, j); Switch(start, i, j, oldI, j);
						temp[1][1] = 0;
						print_Test(temp, start, Display_Col); Display_Col += 9;
					}
	}
}
//Step 2
void open(int temp[][SIDE], int start[][SIDE], int x, int y)
{
	_getch();
	Switch(temp, 1, 1, x, y);
	Switch(start, 1, 1, x, y);
	temp[1][1] = start[1][1];
	print_Test(temp, start, Display_Col); Display_Col += 9;
	_getch();
	Switch(temp, 1, 1, x, y);
	Switch(start, 1, 1, x, y);
	print_Test(temp, start, Display_Col); Display_Col += 9;
}

void open_Edged_tiles(int temp[][SIDE], int start[][SIDE])
{
	// Find non-opened tiles
	int X[4] = { 0,1,2,1 };
	int Y[4] = { 1,2,1,0 };
	for (int k = 0; k < 4; k++)
	{
		if (temp[X[k]][Y[k]] != -1)
		{
			X[k] = -1;
			Y[k] = -1;
			break;
		}
	}
	// Open those tiles
	for (int k = 0; k < 4; k++)
		if (X[k] != -1 && Y[k] != -1)
			open(temp, start, X[k], Y[k]);
}
//Step 3
void set_Zero_Center(int temp[][SIDE], int start[][SIDE], int i, int j, int Oi, int Oj)
{
	if (Oj == 1)
	{
		_getch(); state_Change_and_Print(temp, start, 1, j, i, j);
		_getch(); state_Change_and_Print(temp, start, 1, 1, 1, j);
		temp[i][Oj] = start[i][Oj];
		_getch(); state_Change_and_Print(temp, start, 1, 1, i, Oj);
	}
	if (Oi == 1)
	{
		_getch(); state_Change_and_Print(temp, start, i, 1, i, j);
		_getch(); state_Change_and_Print(temp, start, 1, 1, i, 1);
		temp[Oi][j] = start[Oi][j];
		_getch(); state_Change_and_Print(temp, start, 1, 1, Oi, j);
	}
}

void open_Corner_tile_J(int temp[][SIDE], int start[][SIDE], int i, int j)// Open if the pointer is above or beneath
{
	int loop1 = 2;
	while (loop1 > 0)
	{
		int loop2 = 2;
		_getch(); state_Change_and_Print(temp, start, 1, 1, i, j);
		int oldJ = j; j--;
		while (loop2 > 0)
		{
			_getch(); state_Change_and_Print(temp, start, i, j, i, oldJ);
			set_Zero_Center(temp, start, i, j, -1, oldJ);
			j += 2; loop2--;
		} 
		if (loop1 == 2) 
		{
			_getch(); state_Change_and_Print(temp, start, 1, 1, i, oldJ);
			i = SIDE - i - 1;
		}
		j = 1; loop1--;
	}
	_getch(); state_Change_and_Print(temp, start, 1, 1, i, j);
}

void open_Corner_tile_I(int temp[][SIDE], int start[][SIDE], int i, int j)// Open if the pointer is left or right
{
	int loop1 = 2;
	while (loop1 > 0)
	{
		int loop2 = 2;
		_getch(); state_Change_and_Print(temp, start, 1, 1, i, j);
		int oldI = i; i--;
		while (loop2 > 0)
		{
			_getch(); state_Change_and_Print(temp, start, i, j, oldI, j);
			set_Zero_Center(temp, start, i, j, oldI, -1);
			i += 2; loop2--;
		}
		if (loop1 == 2)
		{
			_getch(); state_Change_and_Print(temp, start, 1, 1, oldI, j);
			j = SIDE - j - 1;
		}
		i = 1; loop1--;
	}
	_getch();
	state_Change_and_Print(temp, start, 1, 1, i, j);
}

void open_Corner_tiles(int temp[][SIDE], int start[][SIDE])
{
	int i, j;
	print_Test(temp, start, Display_Col); Display_Col += 9;// test
	find_Smallest(temp, i, j);// Find smallest tile "min"
	if (j == 1)// If the position of min is above or beneath 0
		open_Corner_tile_J(temp, start, i, j);
	else
		if (i == 1)
			open_Corner_tile_I(temp, start, i, j);
}
//AI seeking
void start_state_AI_Seeking(int start[][SIDE])
{
	gotoxy(0, Display_Col); Display_Col += 2;
	printf("Begin: \t\tPress any key to open...\n");
	// Begin step: Show the {1,1} tile
	int temp[SIDE][SIDE] = { -1,-1,-1,-1,start[1][1],-1,-1,-1,-1 };
	// Step 1: Find 0 and locate 0 to {1,1}
	make_Zero_center(temp, start);
	// Step 2: Open the left edged tiles
	open_Edged_tiles(temp, start);
	// Step 3: Find the smallest tile "min", move 0 around all corners all open all hidden tiles by swapping that tile with 0 at the center.
	open_Corner_tiles(temp, start);
}

//-------------------------------------- 
void find_X(int &x, int &y, int value, int goal[][SIDE])
{
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (goal[i][j] == value)
			{
				x = i; y = j;
			}
}
// Heuristic
int Heuristic(int state[][SIDE], int goal[][SIDE])
{
	int distance = 0;
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (state[i][j] != goal[i][j] && state[i][j] != 0)
			{
				int x, y;
				find_X(x, y, state[i][j], goal);
				if (i > x)
					distance += (i - x);
				else
					distance += (x - i);
				if (y > j)
					distance += (y - j);
				else
					distance += (j - y);
			}
	return distance;
}

void enQueue(NodeList* list, Node* node)
{
	if (list->head == NULL)
	{
		list->head = node;
		list->head->nextNode = NULL;
		list->nodeCount = 1;
	}
	else if (list->tail == NULL)
	{
		list->tail = node;
		list->head->nextNode = list->tail;
		list->tail->nextNode = NULL;
		list->nodeCount = 2;
	}
	else
	{
		list->nodeCount++;
		list->tail->nextNode = node;
		list->tail = node;
	}
}

Node* deQueue(NodeList* list)
{
	if (list->head != NULL)
	{
		list->nodeCount--;
		Node* temp = list->head;
		list->head = list->head->nextNode;
		return temp;
	}
	return NULL;
}

bool reach_Goal(int a[][SIDE], int b[][SIDE])
{
	for (int i = 0; i < SIDE; i++)
		for (int j = 0; j < SIDE; j++)
			if (a[i][j] != b[i][j])
				return false;
	return true;
}

bool isLegalAction(Node* node, Actions action)
{
	int index_i, index_j;
	find_Zero(index_i, index_j, node->state);
	if (action == Actions::LEFT && index_j == 0)  return false;
	if (action == Actions::RIGHT && index_j == 2)  return false;

	switch (index_i)
	{
	case 0:
		if (action == Actions::UP) return false;
		return true;
	case 2:
		if (action == Actions::DOWN) return false;
		return true;
	default:
		return true;
	}
	return true;
}

void FIFO_insert(NodeList* list, Node* node)
{
	if (list->head == NULL)
	{
		list->head = node;
		list->head->nextNode = NULL;
		list->nodeCount = 1;
	}
	else if (list->tail == NULL)
	{
		list->tail = node;
		list->tail->nextNode = NULL;
		list->head->nextNode = list->tail;
		list->nodeCount = 2;
	}
	else
	{
		list->nodeCount++;
		if (list->head->path_cost > node->path_cost)
		{
			node->nextNode = list->head;
			list->head = node;
		}
		else
		{
			Node* run = list->head;
			while (run->nextNode != NULL && run->nextNode->path_cost < node->path_cost)
				run = run->nextNode;
			if (run->nextNode == NULL)
			{
				run->nextNode = node;
				list->tail = node;
				list->tail->nextNode = NULL;
			}
			else
			{
				node->nextNode = run->nextNode;
				run->nextNode = node;
			}
		}
	}
}

Node* Child_node(Node* node, Actions action)
{
	Node* child = newNode();
	child->parent = node;
	child->action = action;
	child->depth = node->depth + 1;
	child->nextNode = NULL;

	state_Copy(node->state, child->state);
	int x, y, i, j;
	find_Zero(x, y, node->state);
	i = x; j = y;
	switch (action)
	{
	case Actions::UP: i--;
		break;
	case Actions::DOWN: i++;
		break;
	case Actions::LEFT: j--;
		break;
	default: j++;
	}
	child->path_cost = child->depth;
	Switch(child->state, x, y, i, j);
	return child;
}

bool checkExist(NodeList* list, Node* node)
{
	Node* temp = list->head;
	while (temp != NULL)
	{
		if (reach_Goal(temp->state, node->state))
			return true;
		temp = temp->nextNode;
	}
	return false;
}

void print_Action(Actions act)
{
	switch (act)
	{
	case Actions::LEFT: printf("LEFT\n"); break;
	case Actions::RIGHT: printf("RIGHT\n"); break;
	case Actions::UP: printf("UP\n"); break;
	default: printf("DOWN\n"); break;
	}
}

void showResult(Node* goal)
{
	while (goal != NULL)
	{
		Node* run = goal;
		while (run->parent != NULL && run->parent->parent != NULL)
		{
			run = run->parent;
		}
		if (run->parent != NULL)
		{
			printf("Next Step: ");
			print_Action(run->action);
			state_Output(run->parent->state);
			delete run->parent;
			run->parent = NULL;
		}
		else
		{
			printf("Last Step:\n");
			state_Output(run->state);
			delete run;
			run = NULL;
			printf("\n***********************************");
			return;
		}
	}
}
// main algorithm
void ASTAR(Node* root, Node* goal)
{
	NodeList* queue = new NodeList();
	NodeList* Checked = new NodeList();
	Checked->head = Checked->tail = queue->head = queue->tail = NULL;

	enQueue(queue, root);

	Node* temp = NULL;

	while (queue->head != NULL)
	{
		temp = deQueue(queue);
		if (reach_Goal(temp->state, goal->state))
		{
			printf("\n***********************************");
			printf("\nASTAR FOUND, %d steps\n\n", temp->depth);
			showResult(temp);
			return;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (isLegalAction(temp, Actions(i)))
				{
					Node* child = Child_node(temp, Actions(i));
					if (checkExist(Checked, child) == false)
					{
						child->path_cost += Heuristic(child->state, goal->state);
						FIFO_insert(queue, child);
					}
				}
			}
		}
		enQueue(Checked, temp);
	}
}

void BFS(Node* root, Node* goal)
{
	NodeList* queue = new NodeList();
	NodeList* Checked = new NodeList();
	Checked->head = Checked->tail = queue->head = queue->tail = NULL;
	enQueue(queue, root);
	Node* temp = NULL;
	while (queue != NULL)
	{
		temp = deQueue(queue);
		if (reach_Goal(temp->state, goal->state))
		{
			printf("\n***********************************");
			printf("\nBFS FOUND, %d steps\n\n", temp->depth);
			showResult(temp);
			return;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (isLegalAction(temp, Actions(i)))
				{
					Node* child = Child_node(temp, Actions(i));
					if (checkExist(Checked,child) == false)
					{
						enQueue(queue, child);
					}
				}
			}
			//delete Checked;
		}
		enQueue(Checked, temp);
	}
}
//--------------------------------------
void main()
{
	Node* START = newNode();
	Node* GOAL = newNode();
	//state_Input(START->state);
	state_Copy(START_DEMO4, START->state);
	//start_state_AI_Seeking(START->state);
	decide_GOAL(GOAL->state, state_Configuration(START->state));
	print_Start_Result(START->state, GOAL->state, Display_Col);
	printf("\nPress any key to start solving...");
	clock_t start, end;
	//start = clock();
	//ASTAR(START, GOAL);
	//end = clock();
	//double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	//printf("\n\nExecute time: %fs", cpu_time_used);
	//printf("\n\n");
	start = clock();
	BFS(START, GOAL);
	end = clock();
	double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("\n\nExecute time: %fs", cpu_time_used);
	_getch();
}