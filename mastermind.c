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

// 函数声明
void printBoard(int guesses[][CODE_LENGTH], int results[][2], int attempts);
void getGuess(int guess[], int guesses[][CODE_LENGTH], int results[][2], int attempts);


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

/*
 * 清除屏幕
 * 根据操作系统使用不同的清屏命令
 */
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/*
 * 生成随机颜色代码
 * 参数:
 *   code[]: 用于存储生成的颜色代码的数组
 * 说明:
 *   - 生成4个不重复的随机颜色
 *   - 颜色范围从0到7
 */
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

/*
 * 打印颜色指南
 * 说明:
 *   - 显示所有可用的颜色及其对应的数字
 *   - 根据当前显示模式使用色块或数字显示
 */
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

/*
 * 获取用户猜测
 * 参数:
 *   guess[]: 存储用户猜测的数组
 *   guesses[][]: 历史猜测记录
 *   results[][]: 历史猜测结果
 *   attempts: 当前尝试次数
 * 说明:
 *   - 处理用户输入的颜色猜测
 *   - 支持切换显示模式(r键)和退出游戏(q键)
 *   - 验证输入的有效性
 */
void getGuess(int guess[], int guesses[][CODE_LENGTH], int results[][2], int attempts) {
    char input[100];
    
    while (1) {
        printBoard(guesses, results, attempts);
        printColorGuide();
        
        printf("Input %d different colors (1-8), 'r' to switch display mode, or 'q' to exit: ", CODE_LENGTH);
        
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
        
        if (input[0] == 'r' || input[0] == 'R') {
            useColorBlocks = !useColorBlocks;  // 切换显示模式
            printf("Display mode changed to %s.\n", useColorBlocks ? "color blocks" : "numbers");
            continue;  // 继续循环，这将导致重新打印游戏板和颜色指南
        }
        
        size_t len = strlen(input);
        if (len != CODE_LENGTH) {
            printf("Please enter %d numbers. You entered %zu characters.\n", CODE_LENGTH, len);
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
            printf("Invalid input. Please enter %d different numbers, ranging from 1 to 8.\n", CODE_LENGTH);
        }
    }
}

/*
 * 检查猜测结果
 * 参数:
 *   code[]: 正确的颜色代码
 *   guess[]: 用户的猜测
 *   correctPosition: 位置和颜色都正确的数量
 *   correctNumber: 颜色正确但位置错误的数量
 * 说明:
 *   - 计算完全匹配的颜色数量
 *   - 计算颜色正确但位置错误的数量
 */
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

/*
 * 打印游戏面板
 * 参数:
 *   guesses[][]: 历史猜测记录
 *   results[][]: 历史猜测结果
 *   attempts: 当前尝试次数
 * 说明:
 *   - 显示游戏标题和版本信息
 *   - 显示所有历史猜测记录
 *   - 显示每次猜测的结果提示
 */
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

/*
 * 打印版本信息
 * 说明:
 *   - 显示游戏版本号
 *   - 显示作者信息
 *   - 显示联系方式和网站
 */
void printVersionInfo() {
    printf("Mastermind Game v%s\n", VERSION);
    printf("Author: %s\n", AUTHOR);
    printf("Email: %s\n", EMAIL);
    printf("Website: %s\n", WEBSITE);
    printf("Copyright (C) 2024 BigDragonSoft.com\n");
}

/*
 * 打印使用说明
 * 说明:
 *   - 显示游戏的主要特性
 *   - 显示命令行参数说明
 *   - 显示基本操作指南
 */
void printUsage() {
    printf("Mastermind Game\n\n");
    printf("This is a traditional console-based Mastermind game. The rules are as follows:\n");
    printf("1. The game will generate a 4-digit color/number code\n");
    printf("2. The range of colors/numbers is from 1 to 8\n");
    printf("3. The player has 10 chances to guess the code\n");
    printf("4. After each guess, the system will provide hints:\n");
    printf("   - Green plus sign (+) indicates both color and position are correct\n");
    printf("   - Red minus sign (-) indicates the color is correct but the position is wrong\n");
    printf("5. The player needs to gradually guess the correct code based on the hints\n");
    printf("6. During the game, you can enter 'r' at any time to switch display mode (color blocks/numbers)\n");
    printf("7. During the game, you can enter 'q' at any time to exit the game\n\n");
    printf("Usage:\n");
    printf("  ./mastermind         Start the game (use color blocks)\n");
    printf("  ./mastermind -n      Start the game (use numbers)\n");
    printf("  ./mastermind -v      Display version information\n");
    printf("  ./mastermind -h      Display this help information\n");
    printf("\nFor more information, please use 'man mastermind' to view the game manual page\n");
}

/*
 * 主函数
 * 参数:
 *   argc: 命令行参数数量
 *   argv: 命令行参数数组
 * 说明:
 *   - 处理命令行参数
 *   - 初始化游戏
 *   - 实现主游戏循环
 *   - 处理游戏结束和重新开始
 */
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
            getGuess(guess, guesses, results, attempts);
            
            if (guess[0] == -1) {  // 用户输入了 'q'
                printf("Game exited.\n");
                return 0;
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
