# MayN Shell Project

## Introduction

MayN Shell is a custom Unix shell implementation inspired by bash, developed as a project at 42 Istanbul. This shell provides a user-friendly command-line interface with support for various shell features like command execution, input/output redirection, pipes, logical operators, environment variables, and more.

## Project Structure

```
.
├── inc/                    # Header files
│   ├── __minishell.h       # Main header file
│   ├── a_command.h         # Command processing
│   ├── b_lexer.h           # Lexical analysis
│   ├── c_parser.h          # Command parsing
│   ├── d_executer.h        # Command execution
│   └── e_utils.h           # Utility functions
├── src/                    # Source files
│   ├── a_command/          # Command processing implementation
│   ├── b_lexer/            # Lexical analysis implementation
│   ├── c_parser/           # Parser implementation
│   ├── d_executor/         # Command execution implementation
│   ├── e_utils/            # Utility functions implementation
│   ├── f_builtins/         # Built-in commands implementation
│   └── main.c              # Main program entry point
├── lib/                    # External libraries
│   └── libft/              # Custom C library
├── build/                  # Build artifacts
└── Makefile                # Build configuration
```

## Core Components and Logical Flow

### 1. Command Processing (`a_command`)

The command processing component handles user input and prepares it for further processing by the shell. This is the first step in the shell's pipeline.

#### Logical Flow:

1. **Input Reading**: 
   - Uses `readline()` to get input from the user with a custom prompt
   - Adds the command to history for recall
   - Handles multi-line commands with continuation

2. **Input Validation**:
   - Checks for all-whitespace commands (ignored)
   - Detects unclosed quotes and parentheses
   - Handles backslash escaping for special characters

3. **Line Continuation**:
   - If a command is incomplete (has unclosed quotes or parentheses)
   - Uses a secondary prompt to continue reading input
   - Maintains a linked list of input lines (`t_line_list` and `t_line_node`)
   - Combines multiple lines into a single command string

4. **Command Preparation**:
   - Trims unnecessary whitespace and backslashes
   - Produces a clean command string for lexical analysis

#### Key Data Structures:

```c
// Line input linked list
typedef struct s_line_node
{
    char                *line;
    struct s_line_node  *next;
    struct s_line_node  *prev;
}    t_line_node;

typedef struct s_line_list
{
    t_line_node  *head;
    t_line_node  *tail;
}    t_line_list;
```

### 2. Lexical Analysis (`b_lexer`)

The lexer component takes the raw command string and breaks it down into tokens, identifying different elements like commands, operators, and arguments.

#### Logical Flow:

1. **Tokenization**:
   - Scans the command string character by character
   - Identifies token boundaries based on spaces and special characters
   - Respects quotes (characters inside quotes are part of the same token)
   - Handles escape sequences with backslashes

2. **Token Classification**:
   - Classifies each token into a specific type (WORD, PIPE, RDRT_IN, etc.)
   - Preserves the original token value for later use
   - Creates token nodes and adds them to a token list

3. **Token List Creation**:
   - Builds a doubly-linked list of tokens (`t_t_list` and `t_t_node`)
   - Maintains token order for parsing

#### Key Data Structures:

```c
// Token types
typedef enum e_t_type
{
    WORD,        // Command or argument
    PIPE,        // Pipe operator |
    RDRT_IN,     // Input redirection <
    RDRT_OUT,    // Output redirection >
    APPEND,      // Append redirection >>
    HEREDOC,     // Here-document <<
    SEMICOLON,   // Command separator ;
    AND,         // Logical AND &&
    OR,          // Logical OR ||
    PRNT_OPEN,   // Open parenthesis (
    PRNT_CLOSE   // Close parenthesis )
} t_t_type;

// Token linked list
typedef struct s_t_node
{
    char            *value;
    t_t_type        type;
    struct s_t_node *next;
    struct s_t_node *prev;
} t_t_node;

typedef struct s_t_list
{
    t_t_node    *head;
    t_t_node    *tail;
} t_t_list;
```

### 3. Parsing (`c_parser`)

The parser component transforms the linear token list into an Abstract Syntax Tree (AST) that represents the command structure hierarchically.

#### Logical Flow:

1. **Recursive Descent Parsing**:
   - Implements a recursive descent parser with precedence rules
   - Builds the AST from top to bottom
   - Handles different command structures (simple commands, pipelines, logical operators)

2. **Command Parsing Hierarchy**:
   - `parse`: Main entry point that starts parsing from the first token
   - `parse_command`: Parses a complete command which could be a pipeline, AND/OR, or group
   - `parse_and_or`: Parses AND/OR expressions (cmd1 && cmd2 or cmd1 || cmd2)
   - `parse_pipeline`: Parses pipelines (cmd1 | cmd2 | cmd3)
   - `parse_simple_command`: Parses a simple command with its arguments and redirections
   - `parse_group`: Parses parenthesized command groups
   - `parse_redirection`: Parses redirection operators and their targets

3. **AST Construction**:
   - Creates different types of AST nodes based on the token types
   - Builds command nodes with arguments and redirections
   - Creates operator nodes (pipe, AND, OR) with left and right children
   - Forms group nodes for parenthesized expressions

4. **Error Handling**:
   - Detects syntax errors during parsing
   - Returns NULL if the AST cannot be constructed properly

#### Key Data Structures:

```c
// AST node types
typedef enum e_ast_type
{
    AST_COMMAND,  // Simple command
    AST_PIPE,     // Pipe operator |
    AST_REDIR_IN, // Redirection <
    AST_REDIR_OUT,// Redirection >
    AST_APPEND,   // Append >>
    AST_HEREDOC,  // Here document <<
    AST_AND,      // Logical AND &&
    AST_OR,       // Logical OR ||
    AST_GROUP     // Grouping with parentheses ()
} t_ast_type;

// Redirection structure
typedef struct s_redir
{
    t_t_type        type;   // Redirection type from lexer
    char            *file;  // Target file name
    struct s_redir  *next;  // Next redirection
} t_redir;

// Command structure
typedef struct s_cmd
{
    char    **args;         // Command arguments (args[0] is the command)
    t_redir *redirs;        // List of redirections
} t_cmd;

// AST Node structure
typedef struct s_ast_node
{
    t_ast_type  type;       // Node type
    union 
    {
        t_cmd   cmd;        // For AST_COMMAND
        struct
        {
            struct s_ast_node  *left;  // Left child
            struct s_ast_node  *right; // Right child
        } op;               // For operators like PIPE, AND, OR
        struct s_ast_node  *group;     // For parenthesized group
    } data;
} t_ast_node;
```

### 4. Command Execution (`d_executor`)

The executor component traverses the AST and executes the commands with their appropriate contexts, handling redirections, pipes, and logical operators.

#### Logical Flow:

1. **AST Traversal and Execution**:
   - `execute_ast`: Main entry point that recursively executes the AST
   - Executes different node types based on their type:
     - `execute_command`: Executes a simple command with arguments
     - `execute_pipe`: Executes a pipeline
     - `execute_and`: Executes AND logical operator
     - `execute_or`: Executes OR logical operator
     - `execute_group`: Executes a parenthesized group

2. **Command Execution Process**:
   - Expands environment variables in command arguments
   - Sets up redirections
   - Searches for the command in PATH
   - Forks a child process to execute external commands
   - Executes built-in commands directly
   - Collects exit status

3. **Pipeline Handling**:
   - Creates pipes between commands
   - Redirects stdout of left command to stdin of right command
   - Executes commands in separate processes

4. **Logical Operators**:
   - AND (`&&`): Executes right command only if left command succeeds
   - OR (`||`): Executes right command only if left command fails

5. **Redirection Setup**:
   - Opens files for redirection
   - Duplicates file descriptors to stdin/stdout
   - Handles input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)

#### Key Data Structures:

```c
// Execution context
typedef struct s_exec_context
{
    char    **env;              // Environment variables
    int     last_exit_status;   // Last command exit status
    int     pipe_fd[2];         // Pipe file descriptors
    pid_t   last_pid;           // Last process ID
} t_exec_context;
```

## Shell Loop Overview

The main shell loop ties all components together in a continuous cycle:

1. **Display prompt and read input** (a_command)
   - Read command line input from user
   - Handle multi-line commands if needed

2. **Lexical analysis** (b_lexer)
   - Convert raw command into token list
   - Identify different token types

3. **Parsing** (c_parser)
   - Convert token list to Abstract Syntax Tree
   - Structure commands hierarchically

4. **Execution** (d_executor)
   - Traverse AST and execute commands
   - Handle redirections, pipes, and logical operators
   - Set exit status for next command

5. **Cleanup**
   - Free resources (AST, token list, command line)
   - Start loop again for next command

## Features

- Command execution with arguments
- Pipes (`|`) to connect multiple commands
- Input/output redirections (`<`, `>`, `>>`)
- Logical operators (`&&`, `||`)
- Command grouping with parentheses
- Environment variable expansion (using `&` prefix)
- Quoting to handle special characters and spaces
- Custom prompt display
- Command history
- Error reporting and handling

## Building and Running

```bash
# Build the shell
make

# Run the shell
./build/minishell
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is part of the 42 School curriculum and is subject to its licensing terms.

## Acknowledgements

- The 42 School for the project requirements and guidelines
- The bash shell documentation for reference


---------------------------------------

**Minishell**

> Bash, fish, zsh gibi kabukların basit bir implementasyonu. 

 - [ ] Yeni bir komut beklenirken prompt göstermeli.
 - [ ] Çalışan bir komut geçmişi (history) özelliğine sahip olmalı.
 - [ ] Alınan sinyalleri işlemek için, sadece sinyal numarasının saklandığı en fazla bir global değişken kullanmalı.
 - [ ] Özel karakterleri --(\, ;) gibi-- ve eksik tırnakları yorumlamamalı.
 - [ ] Tek tırnak ' içindeki meta karakterlerin yorumlanması engellenmeli.
 - [ ] Çift tırnak içindeki " metak karakterlerin -- $ işareti hariç -- yorumlanması engellenmeli.
 - [ ] >, >>, <<, <, | işaretleri desteklenmeli.
 - [ ] Çevresel değişkenleri $ ile görebilmeli.
 - [ ] Son çalıştırılan pipellinenın çıkış durumu $? ile gösterilmeli.
 - [ ] Ctrl-C, Ctrl-D Ctrl-\ bashtaki gibi işlemeli
	- Ctrl-C yeni satırda prompt gösterir.
	- Ctrl-D kabuktan çıkar.
	- Ctrl-\ hiçbir şey yapmaz.
 - [ ] Builtin komutları ayrı bir şekilde implemente etmeli.
	 - Echo / ekstra -n parametresi ile
	 - cd / relative, mutlak yol
	 - pwd
	 - export
	 - unset
	 - env
	 
------------------------------------

**Öğrenilmesi Gerekenler**

 - [ ] Readlines ile kullanıcıdan sürekli komut beklemek.
 - [ ] add_history / burada bence builtin history komutu implemente edersek daha hoş olur,
add_history ayrıca linkedlistli bir history olmalı, history komutu girince geçmişteki komutları görebilmeliyiz.
 - [ ] sigaction ile sinyal handllelama
 - [ ] tek tırnak, çift tırnak, gibi şeylerin bash üstünde tam olarak nasıl çalıştığını anlamak lazım
 - [ ] redirection ve pipe olayını tam olarak kavramak --> pipexi bitirince anlamış oluyoruz.
 - [ ] redirection ve pipeların birden çok olduğu komutlar için lexer/parser gerekiyor. yani her komutu bu şekilde
düşünüp bir işleyici oluşturmak gerekiyor.
 - [ ] $ işareti ve env handllelama

 -------------------------------

**Roadmap**

 - [x] Prompt
 - [x] Get Command
 - [ ] Lexer
 - [ ] Parser
 - [ ] Executer
 - [ ] Change functions with libft
 - [ ] Edge Cases
 - [ ] Error Check
 - [ ] Leak Check
 - [ ] Norm Check
 - [ ] Last Control
 - [ ] Push and Eval
