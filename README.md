# Compiler Project

This project is a multi-stage compiler developed as part of a compilers course. Each stage builds upon the previous one, adding more functionality to the compiler.

## Stages

The project is divided into the following stages:

*   **Lexical Scanner (`lexical_scanner`):** The first stage of the compiler, responsible for lexical analysis. It takes a source file as input and breaks it down into a series of tokens.
*   **Syntactic Analysis (`etapa2`):** This stage introduces a parser that performs syntactic analysis on the token stream from the lexical scanner.
*   **Abstract Syntax Tree (`etapa3`):** This stage builds an Abstract Syntax Tree (AST) representation of the source code.
*   **Semantic Analysis (`etapa4`):** This stage performs semantic checks on the AST to ensure that the code is semantically correct.
*   **Three-Address Code Generation (`etapa5`):** This stage generates intermediate code in the form of Three-Address Code (TAC).
*   **Assembly Code Generation (`etapa6`):** The final stage of the compiler, which generates assembly code from the TAC.

## Building and Running

Each stage has its own Makefile and can be built and run independently. To build a specific stage, navigate to its directory and run the `make` command.

For example, to build the lexical scanner:

```bash
cd lexical_scanner
make
```

To run the lexical scanner, provide an input file:

```bash
./lexical_scanner test.txt
```

Similarly, to build and run other stages, navigate to their respective directories and use the `make` command. The executable for each stage is named after the directory (e.g., `etapa2`, `etapa3`).

### Cleaning up

To clean up the build files in any stage, run:

```bash
make clean
```
