#include <stdio.h>
#include <stdlib.h>

#define s(x) scanf("%d", &x)
#define sc(x) scanf(" %c", &x)

typedef struct Node { 
    int x, y;
    struct Node* next;
    struct Node* prev; 
} Node; 

Node* newNode() {
    return (Node*)calloc(1, sizeof(Node));
}

Node* addFirst(Node* head, int x, int y, char** board) {
    Node* newHead = newNode();
    board[head->x][head->y] = '$';
    board[x][y] = '@';
    newHead->x = x;
    newHead->y = y;
    newHead->next = head;
    newHead->prev = NULL;
    head->prev = newHead;

    return newHead;
}

Node* removeLast(Node* tail, char** board) {
    board[tail->x][tail->y] = ' ';
    board[tail->prev->x][tail->prev->y] = '<';
    Node* remove = tail;
    tail = tail->prev;
    tail->next = NULL;
    free(remove);
    return tail;
}

void buildBoard(char** board, int row, int col) {
    for(int i = 0; i < row; i++) {
        *(board + i) = (char*)calloc(col, sizeof(char));
        for(int j = 0; j < col; j++) {
            board[i][j] = ' ';
        }
    }
    board[0][0] = '<';
    board[0][1] = '$';
    board[0][2] = '@';
}

void displayBoard(char** board, int row, int col) {
    for(int i = 0; i <= row; i++) printf("##");

    for(int i = 0; i < row; i++) {
        printf("\n#");
        for(int j = 0; j < col; j++) {
            printf("%c ", board[i][j]);
        }
        printf("#");
    }
    printf("\n");
    for(int i = 0; i <= row; i++) printf("##");
}

void createSnake(Node* head, Node* tail) {
    // head
    head->x = 0;
    head->y = 2;
    head->prev = NULL;
    // tail
    tail->x = 0;
    tail->y = 0;
    tail->next = NULL;
    // body
    Node* body = newNode();
    body->x = 0;
    body->y = 1;
    body->prev = head;
    body->next = tail;
    tail->prev = body;
    head->next = body;
}

void setFruit(char** board, int row, int col, int fruits) {
    do {
        int x = rand() % row;
        int y = rand() % col;
        if(board[x][y] == ' ') {
            board[x][y] = 'O';
            fruits--;
        }
    } while(fruits);
}

int isFruit(char** board, int x, int y) {
    return board[x][y] == 'O' ? 1 : 0;
}

int isSnake(char** board, int x, int y) {
    return board[x][y] == ' ' ? 0 : 1;
}

int main()
{
    char restart = 'n';
    do {
        int row = 20, col = 20, score = 0;
        char** board = (char**)calloc(row, sizeof(char*));
        buildBoard(board, row, col);
        
        Node* head = newNode();
        Node* tail = newNode();

        createSnake(head, tail);
        setFruit(board, row, col, 6);

        char ip = 'i';
        do {
            system("clear");
            printf("@ - head\t< - tail\n\nBoundaries are safe. Enjoy!! ;P\n\n");
            displayBoard(board, row, col);
            printf("\n\nPress W to move up");
            printf("\nPress A to move left");
            printf("\nPress S to move down");
            printf("\nPress D to move right");
            printf("\nPress X to end the game\n");
            sc(ip);
            int err = 0, x = 0, y = 0;
            if(ip == 'w' || ip == 'W') {
                x = head->x == 0 ? row - 1 : head->x - 1;
                y = head->y;
                if(x == head->next->x) err = 1;
            }
            else if(ip == 's' || ip == 'S') {
                x = (head->x + 1) % row;
                y = head->y;
                if(x == head->next->x) err = 1;
            }
            else if(ip == 'd' || ip == 'D') {
                y = (head->y + 1) % col;
                x = head->x;
                if(y == head->next->y) err = 1;
            }
            else if(ip == 'a' || ip == 'A') {
                y = head->y == 0 ? 19 : head->y - 1;
                x = head->x;
                if(y == head->next->y) err = 1;
            }
            else err = 1;
            if(!err) {
                if(isFruit(board, x, y)) {
                    score++;
                    setFruit(board, row, col, 1);
                } 
                else {
                    if(isSnake(board, x, y)) break;
                    tail = removeLast(tail, board);
                }
                head = addFirst(head, x, y, board);
            }
            system("clear");
        } while(ip != 'x' && ip != 'X');
        system("clear");
        if(ip == 'x' || ip == 'X') {
            printf("You Quit!\n");
        }
        else {
            printf("Ooops! You Ate Yourself :'(\n");
        }
        printf("Your Score: %d\nThanks For Playing!! :)\n", score);
        printf("\nWould You Like To Play Again? (Y/N)\n");
        sc(restart);
    } while(restart == 'y' && restart != 'Y');    

    printf("\nSee You Soon. Byeeeee!!\n");
    return 0;
}
