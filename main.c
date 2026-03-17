#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t byte;

#define SIZE 32
#define MASK 31

byte mem[SIZE] = {0};
byte stack[SIZE] = {0};
byte sp = 0;
byte pc = 0;
byte running = 1;

byte at(byte i) { return stack[(sp + i) & MASK]; }
void set(byte i, byte v) { stack[(sp + i) & MASK] = v; }

void stop(byte _) { running = 0; }

void push(byte v) {
    sp = (sp - 1) & MASK;
    stack[sp] = v;
}

void pop(byte _) {
    sp = (sp + 1) & MASK;
}

void add(byte b) {
    set(0, at(0) + at(b));
}

void sub(byte b) {
    set(0, at(0) - at(b));
}

void jump(byte addr) {
    pc = addr;
}

void jumpz(byte addr) {
    if (at(0) == 0)
        pc = addr;
}

void swap(byte b) {
    byte t = at(0);
    set(0, at(b));
    set(b, t);
}

typedef void (*op)(byte);
op ops[] = {stop, push, pop, add, sub, jumpz, jump, swap};

int main() {
    int n;
    printf("Enter number of instructions: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char cmd[10];
        int val = 0;

        printf(">> ");
        scanf("%s", cmd);

        if (strcmp(cmd, "push") == 0) {
            scanf("%d", &val);
            mem[i] = (1 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "pop") == 0) {
            mem[i] = (2 << 5);
        }
        else if (strcmp(cmd, "add") == 0) {
            scanf("%d", &val);
            mem[i] = (3 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "sub") == 0) {
            scanf("%d", &val);
            mem[i] = (4 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "jumpz") == 0) {
            scanf("%d", &val);
            mem[i] = (5 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "jump") == 0) {
            scanf("%d", &val);
            mem[i] = (6 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "swap") == 0) {
            scanf("%d", &val);
            mem[i] = (7 << 5) | (val & 31);
        }
        else if (strcmp(cmd, "stop") == 0) {
            mem[i] = (0 << 5);
        }
    }

    while (running) {
        byte instr = mem[pc];
        pc = (pc + 1) & MASK;

        byte opcode  = instr >> 5;
        byte operand = instr & 31;

        ops[opcode](operand);

        printf("TOS: %u\n", at(0));
    }

    return 0;
}
