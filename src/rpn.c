
/**
 * @file rpn.c
 * @author maszynatrurla
 * @brief Programming exercise: reverse polish notiation calculator.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/19/rpn-calculator/s
 */

#include <stdio.h>


#define STACK_SIZE      4096
#define CBUF_SIZE       48

#define TRACE_OP        1


typedef float cell_t;

typedef struct {
    cell_t cells[STACK_SIZE];
    int top;
} MyStack_t;

typedef struct {
    char line[CBUF_SIZE];
    int i;
    int decimalPoint;
} Cbuf_t;


static void   push(cell_t data);
static cell_t pop(void);
static cell_t peek(void);
static void   printStack(void);
static void   updateStack(void);
static void   append(int ch);
static void   operation(void (*op)(void));

static void plus(void);
static void minus(void);
static void mul(void);
static void div(void);



static MyStack_t s_stack = {
    .cells = {0,},
    .top   = 0,
};

static Cbuf_t s_line = {
    .line = {0,},
    .i = 0,
    .decimalPoint = 0,
};



static void push(cell_t data) {
    if (s_stack.top < STACK_SIZE - 1) {
        s_stack.cells[++s_stack.top] = data;
    } else {
        printf("Error: Stack overflow.");
    }
}

static cell_t pop(void) {
    if (s_stack.top >= 0) {
        return s_stack.cells[s_stack.top--];
    }
    return 0;
}

static cell_t peek(void) {
    if (s_stack.top >= 0 && s_stack.top < STACK_SIZE) {
        return s_stack.cells[s_stack.top];
    }
    return 0;
}

static void printStack(void) {
    printf("\n= %10g\n\n>", peek());
}

static void plus(void) {
    cell_t B = pop();
    cell_t A = pop();
    
    push(A + B);
#ifdef TRACE_OP
    printf("%s(%g, %g) = %g\n", __FUNCTION__, A, B, A + B);
#endif
}

static void minus(void) {
    cell_t B = pop();
    cell_t A = pop();
    
    push(A - B);
#ifdef TRACE_OP
    printf("%s(%g, %g) = %g\n", __FUNCTION__, A, B, A - B);
#endif
}

static void mul(void) {
    cell_t B = pop();
    cell_t A = pop();
    
    push(A * B);
#ifdef TRACE_OP
    printf("%s(%g, %g) = %g\n", __FUNCTION__, A, B, A * B);
#endif    
}

static void div(void) {
    cell_t B = pop();
    cell_t A = pop();
    
    push(A / B);
#ifdef TRACE_OP
    printf("%s(%g, %g) = %g\n", __FUNCTION__, A, B, A / B);
#endif
}

static void updateStack(void) {
    if ((s_line.i == 1 && !s_line.decimalPoint) ||
            s_line.i > 1) {
        cell_t cell;
        
        s_line.line[s_line.i] = 0;
        sscanf(s_line.line, "%g", (float *) &cell);
        s_line.i = 0;
        s_line.decimalPoint = 0;
        push(cell);
    }
}

static void append(int ch) {
    if (s_line.i < CBUF_SIZE) {
        s_line.line[s_line.i++] = ch;
    } else {
        printf("Error: buffer overflow.");
    }
}

static void operation(void (*op)(void)) {
    updateStack();
    op();
    /* printStack(); */
}


int main(int argc, char ** argv) {
    int ch;
    
    printf("RPN Calculator v.0.1    type 'q' to quit\n\n");
    printStack();
    
    while (EOF != (ch = getchar())) {
        switch (ch) {
            case 'q':
            case 'Q':
                printf("\nBye\n");
                return 0;
            case '+':
                operation(plus);
                break;
            case '-':
                operation(minus);
                break;
            case '*':
                operation(mul);
                break;
            case '/':
                operation(div);
                break;
            case 0x20:
            case '\r':
                updateStack();
                break;
            case '\n':
                updateStack();
                printStack();
                break;
            case '.':
            case ',':
                append('.');
                break;
            default:
                if (ch >= '0' && ch <= '9') {
                    append(ch);
                } else {
                    printf("Warning: Ignoring char '%c'", ch);
                }
                break;
        }
    }
    return 0;
}
