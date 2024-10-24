/*
 * Mastermind 游戏
 * 版权所有 (C) 2024 [大龙软件 bigdragonsoft.com]
 * 
 * 这是一个在控制台中运行的Mastermind（猜颜色）游戏，主要功能包括：
 * 1. 提供一个4位的颜色代码供玩家猜测
 * 2. 支持8种不同颜色，玩家需要在10次尝试内猜出正确的颜色组合
 * 3. 实现了颜色代码的随机生成和验证系统
 * 4. 具有猜测评估、提示生成等核心游戏逻辑
 * 5. 提供友好的命令行界面，支持彩色显示和数字模式
 * 6. 包含版本信息、帮助说明等附加功能
 * 
 * Mastermind Game
 * Copyright (C) 2024 [bigdragonsoft.com]
 * This is a console-based Mastermind game, with main features including:
 * 1. Providing a 4-digit color code for players to guess
 * 2. Supporting 8 different colors, players need to guess the correct color combination within 10 attempts
 * 3. Implementing a random code generation and validation system
 * 4. Possessing core game logic such as guess evaluation and hint generation
 * 5. Offering a user-friendly command-line interface with support for color display and number mode
 * 6. Including additional features like version information and help instructions
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CODE_LENGTH 4
#define MAX_ATTEMPTS 10
#define NUM_COLORS 8

#define VERSION "0.1.0"
#define AUTHOR "Qiang Guo"
#define EMAIL "bigdragonsoft@gmail.com"
#define WEBSITE "https://github.com/bigdragonsoft/mastermind"

// 定义颜色的ANSI转义序列
const char *COLOR_CODES[NUM_COLORS] = {
    "\033[0;41m  \033[0m", // 红
    "\033[0;44m  \033[0m", // 蓝
    "\033[0;42m  \033[0m", // 绿
    "\033[0;43m  \033[0m", // 黄
    "\033[0;45m  \033[0m", // 紫
    "\033[0;48;5;208m  \033[0m", // 橙
    "\033[0;47m  \033[0m", // 白
    "\033[0;46m  \033[0m"  // 青
};

// 定义颜色
const char *COLORS[NUM_COLORS] = {"Red", "Blue", "Green", "Yellow", "Purple", "Orange", "White", "Cyan"};

int useColorBlocks = 1; // 默认使用色块

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void generateCode(int code[]) {
    int used[NUM_COLORS] = {0};
    int count = 0;
    while (count < CODE_LENGTH) {
        int color = rand() % NUM_COLORS;
        if (!used[color]) {
            code[count] = color;
            used[color] = 1;
            count++;
        }
    }
}

void printColorGuide() {
    printf("\nColor Guide:\n");
    for (int i = 0; i < NUM_COLORS; i++) {
        if (useColorBlocks) {
            printf("%s %d: %-6s ", COLOR_CODES[i], i + 1, COLORS[i]);
        } else {
            printf("%d: %-6s ", i + 1, COLORS[i]);
        }
        if (i % 4 == 3 || i == NUM_COLORS - 1) {
            printf("\n");
        } else {
            printf("  ");
        }
    }
    printf("\n");
}

void getGuess(int guess[]) {
    char input[100];
    printColorGuide();
    
    while (1) {
        printf("Enter %d different colors (1-8), or 'q' to quit: ", CODE_LENGTH);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        // 移除输入末尾的换行符
        input[strcspn(input, "\n")] = 0;
        
        if (input[0] == 'q' || input[0] == 'Q') {
            guess[0] = -1;  // 用特殊值表示退出
            return;
        }
        
        size_t len = strlen(input);
        if (len != CODE_LENGTH) {
            printf("Please enter exactly %d numbers. You entered %zu characters.\n", CODE_LENGTH, len);
            continue;
        }
        
        int valid = 1;
        int used[NUM_COLORS] = {0};
        for (int i = 0; i < CODE_LENGTH; i++) {
            if (input[i] < '1' || input[i] > '8') {
                valid = 0;
                break;
            }
            int color = input[i] - '1';  // 将输入的1-8转换为0-7
            if (used[color]) {
                valid = 0;
                break;
            }
            used[color] = 1;
            guess[i] = color;
        }
        
        if (valid) {
            break;
        } else {
            printf("Invalid input. Please enter %d different numbers between 1 and 8.\n", CODE_LENGTH);
        }
    }
}

void checkGuess(int code[], int guess[], int *correctPosition, int *correctNumber) {
    *correctPosition = 0;
    *correctNumber = 0;
    int codeCopy[NUM_COLORS] = {0};
    int guessCopy[NUM_COLORS] = {0};
    
    // 首先检查位置和颜色都正确的情况
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (code[i] == guess[i]) {
            (*correctPosition)++;
        } else {
            codeCopy[code[i]]++;
            guessCopy[guess[i]]++;
        }
    }
    
    // 然后检查颜色正确但位置错误的情况
    for (int i = 0; i < NUM_COLORS; i++) {
        *correctNumber += (codeCopy[i] < guessCopy[i]) ? codeCopy[i] : guessCopy[i];
    }
}

void printBoard(int guesses[][CODE_LENGTH], int results[][2], int attempts) {
    clearScreen();
    printf("\n");
    //printf("         -----------\n");
    printf("\033[1;32m\n");  // 设置亮绿色
    printf("             Mastermind\n");
    printf("\033[0m");  // 重置颜色
    printf("             -----------\n");
    printf("               v%s\n\n", VERSION);

    
    printf("No.   Guess               Hints\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        printf("%2d    ", i + 1);
        if (i < attempts) {
            for (int j = 0; j < CODE_LENGTH; j++) {
                if (useColorBlocks) {
                    printf("%s ", COLOR_CODES[guesses[i][j]]);
                } else {
                    printf("%d ", guesses[i][j] + 1);
                }
            }
            
            // 调整提示符号的起始位置
            if (useColorBlocks) {
                printf("        ");
            } else {
                printf("            ");  // 额外的空格来对齐
            }
            
            for (int j = 0; j < results[i][0]; j++) {
                printf("\033[0;32m+ \033[0m"); // 绿色加号表示位置和颜色都正确
            }
            for (int j = 0; j < results[i][1]; j++) {
                printf("\033[0;31m- \033[0m"); // 红色减号表示颜色正确但位置错误
            }
        } else {
            if (useColorBlocks) {
                printf("                    ");
            } else {
                printf("                        ");  // 额外的空格来对齐
            }
        }
        printf("\n");
        printf("----------------------------------------\n"); // 每行的分割线
    }
    printf("\n");
}

void printVersionInfo() {
    printf("Mastermind Game v%s\n", VERSION);
    printf("Author: %s\n", AUTHOR);
    printf("Email: %s\n", EMAIL);
    printf("Website: %s\n", WEBSITE);
    printf("Copyright (C) 2024 BigDragonSoft.com\n");
}

void printUsage() {
    printf("Mastermind Game\n\n");
    printf("This is a console-based Mastermind game. Main features include:\n");
    printf("1. 4-digit color/number code\n");
    printf("2. 8 different colors/numbers\n");
    printf("3. 10 attempts\n");
    printf("4. Random color/number code generation and validation system\n");
    printf("5. User-friendly command-line interface\n\n");
    printf("Usage:\n");
    printf("  ./mastermind         Start the game (use color blocks)\n");
    printf("  ./mastermind -n      Start the game (use numbers)\n");
    printf("  ./mastermind -v      Display version information\n");
    printf("  ./mastermind -h      Display this help information\n");
    printf("\nFor more information, please use 'man mastermind' to view the game manual page\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    // 检查命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            useColorBlocks = 0;
        } else if (strcmp(argv[i], "-v") == 0) {
            printVersionInfo();
            return 0;
        } else if (strcmp(argv[i], "-h") == 0) {
            printUsage();
            return 0;
        }
    }
    
    int code[CODE_LENGTH], guess[CODE_LENGTH];
    int guesses[MAX_ATTEMPTS][CODE_LENGTH] = {0};
    int results[MAX_ATTEMPTS][2] = {0};
    int attempts = 0, correctPosition, correctNumber;
    
    generateCode(code);
    
    clearScreen();
    printf("Welcome to Mastermind!\n");
    printf("Try to guess the combination of %d colors out of 8.\n", CODE_LENGTH);
    printf("Hint symbols:\n");
    printf("  \033[0;32m+\033[0m : Correct color and position\n");
    printf("  \033[0;31m-\033[0m : Correct color but wrong position\n\n");
    
    char input;  // 在这里声明 input 变量

    do {
        attempts = 0;
        generateCode(code);
        
        while (attempts < MAX_ATTEMPTS) {
            printBoard(guesses, results, attempts);
            
            getGuess(guess);
            
            if (guess[0] == -1) {  // 用户输入了 'q'
                printf("Game exited.\n");
                return 0;  // 直接退出程序
            }
            
            // 存储猜测
            for (int i = 0; i < CODE_LENGTH; i++) {
                guesses[attempts][i] = guess[i];
            }
            
            checkGuess(code, guess, &correctPosition, &correctNumber);
            results[attempts][0] = correctPosition;
            results[attempts][1] = correctNumber;
            
            attempts++;
            
            if (correctPosition == CODE_LENGTH) {
                printBoard(guesses, results, attempts);
                printf("Congratulations! You won in %d attempts.\n", attempts);
                break;
            }
        }
        
        if (attempts == MAX_ATTEMPTS) {
            printBoard(guesses, results, attempts);
            printf("Sorry, you didn't guess the correct answer in %d attempts.\n", MAX_ATTEMPTS);
            printf("The correct answer was: ");
            for (int i = 0; i < CODE_LENGTH; i++) {
                if (useColorBlocks) {
                    printf("%s ", COLOR_CODES[code[i]]);
                } else {
                    printf("%d ", code[i] + 1);
                }
            }
            printf("\n");
        }
        
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &input);
        while(getchar() != '\n');  // Clear input buffer
    } while (input == 'y' || input == 'Y');

    printf("Thanks for playing. Goodbye!\n");
    return 0;
}
