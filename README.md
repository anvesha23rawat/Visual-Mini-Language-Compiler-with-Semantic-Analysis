# Compiler Simulator

A small educational compiler-pipeline simulator written in C++ with a browser-based interface served by Node.js/Express. It demonstrates lexical analysis, a simplified AST parser, basic semantic checks, three-address-code (TAC) generation, a rudimentary constant-folding pass, and control-flow-graph (CFG) output.

> **Scope:** This is a learning prototype, not a standards-compliant C/C++ compiler. The stages are partly independent; the current parser does not consume lexer tokens, and some UI actions show demonstrations rather than running the corresponding C++ stage. See [Current limitations](#current-limitations) before relying on results.

## Features

- **Lexical analysis** — recognizes a small set of keywords, identifiers, integer/decimal-looking numbers, quoted strings, comments, operators, punctuation, and invalid characters. Results are written to `tokens.txt`.
- **Parsing and AST display** — builds a simplified AST from the source file and writes a tree to `parse.txt`.
- **Semantic checks** — builds a simple symbol table, reports redeclarations and undeclared variables, and writes results to `semantic.txt`.
- **TAC generation** — translates the supported AST assignments and `if` conditions into a basic three-address-code listing in `tac.txt`.
- **Constant-folding demonstration** — `optimizer.exe` processes simple integer TAC arithmetic and writes `optimized.txt`.
- **CFG output** — `cfg.exe` reads TAC lines and emits a Graphviz DOT file in `cfg.dot`. A pre-generated `cfg.png` is also present for the web page’s CFG button.
- **Browser UI** — offers Tokenize, Parse, Semantic, Run All, AST, CFG, Optimization, and Sample Code actions.

## Compiler pipeline and project layout

The intended learning flow is:

```text
Source text (Code.txt)
        ├── lexer.cpp ───────────────> tokens.txt
        └── parser.cpp ──> AST ──────> parse.txt
                              ├──────> semantic.cpp ──> semantic.txt
                              └──────> tac.cpp ────────> tac.txt
                                                         ├── optimizer.cpp ──> optimized.txt
                                                         └── cfg.cpp ────────> cfg.dot
```

| File | Purpose |
| --- | --- |
| `lexer.cpp`, `token.h` | Standalone lexer and a token type definition. The current lexer defines its own token categories and writes text output rather than using `token.h`. |
| `ast.h` | Shared AST node structure and a console tree printer. |
| `parser.cpp`, `parser.h` | Line-oriented source-to-AST parser. |
| `parser_main.cpp` | Standalone parser executable; writes `parse.txt`. |
| `semantic.cpp`, `semantic.h`, `semantic_main.cpp` | Symbol-table and basic semantic checks; standalone executable writes `semantic.txt`. |
| `tac.cpp` | Standalone TAC generator; writes `tac.txt`. |
| `optimizer.cpp` | Standalone integer constant-folding pass over `tac.txt`; writes `optimized.txt`. |
| `cfg.cpp`, `cfg.h` | Standalone CFG DOT writer based on lines in `tac.txt`. |
| `server.js`, `frontend.html` | Express API and static browser UI. |
| `Code.txt` | Default source input used by the standalone C++ programs. |
| `grammar.txt` | Intended grammar reference; it is broader than the parser currently implemented. |

## Requirements

- **Windows** (the web server launches executables using Windows-style paths such as `.\lexer.exe`).
- **Node.js 18 or newer** and npm. The project uses Express 5, whose dependencies require Node 18+.
- A C++ compiler available as `g++` on `PATH`, such as MinGW-w64. The commands below use the tested MinGW-style `g++` interface and C++17.
- A browser to use the UI.

## Setup and run

Run these commands from the project root in PowerShell.

### 1. Install the Node.js dependencies

```powershell
npm install
```

### 2. Build the browser-required C++ executables

The browser server expects `lexer.exe`, `parser.exe`, and `semantic.exe` in the project root. Build them with:

```powershell
g++ -std=c++17 lexer.cpp -o lexer.exe
g++ -std=c++17 parser_main.cpp parser.cpp -o parser.exe
g++ -std=c++17 semantic_main.cpp semantic.cpp parser.cpp -o semantic.exe
```

### 3. Start the web server

```powershell
npm start
```

Open **http://localhost:3000/frontend.html** in a browser. The server listens on port `3000`. Keep the terminal running while using the page.

### 4. Run optional stages from the command line

Build and run TAC generation:

```powershell
g++ -std=c++17 tac.cpp parser.cpp -o tac.exe
.\tac.exe
```

Build and run constant folding after TAC has been generated:

```powershell
g++ -std=c++17 optimizer.cpp -o optimizer.exe
.\optimizer.exe
```

Build and run CFG generation after TAC has been generated:

```powershell
g++ -std=c++17 cfg.cpp -o cfg.exe
.\cfg.exe
```

All commands use `Code.txt` and read/write output files in the current working directory. Run them from the repository root. The CFG generator creates `cfg.dot`; to render a fresh PNG, install Graphviz separately and run `dot -Tpng cfg.dot -o cfg.png`. The UI's CFG button displays the existing `cfg.png`; it does not invoke the CFG generator.

## Using the browser interface

1. Enter source code in the editor, or click **Sample Code**.
2. Choose **Tokenize**, **Parse**, or **Semantic** to run an individual server-backed stage.
3. Choose **Run All** to request tokenization, parsing, and semantic analysis and display their outputs together.
4. **AST** requests the parser output and formats it for display.
5. **Show CFG** displays the current `cfg.png` file.
6. **Optimization** currently displays a fixed before/after example; it does not execute `optimizer.exe`.

The server exposes `POST /tokenize`, `POST /parse`, `POST /semantic`, and `POST /all`. Requests send JSON in the form `{ "code": "...source..." }`. The server saves submitted text to `Code.txt`, then launches the corresponding Windows executable. Ensure the required executables exist before using those routes.

## Supported input (current implementation)

The parser currently handles a narrow subset of source text:

- Integer declarations, such as `int count;`.
- Integer declarations with an initializer, such as `int count = 5;`.
- Assignment statements, such as `count = value + 2;`.
- A simple `if` condition written on one line, such as `if (count < 10)`.
- Arithmetic expression tokens separated by whitespace (for example, `value + 2`).

The sample program is:

```cpp
int a;
a = 5 + 3;

if (a < 10)
a = a + 1;
```

The lexer recognizes more token spellings than the parser supports. For example, keywords include `float`, `char`, `bool`, `void`, `double`, `while`, `for`, `else`, and `return`; operators include arithmetic, assignment, and relational forms. Recognition by the lexer does **not** imply that those constructs are parsed or semantically supported.

## Generated files

| Output | Generated by | Description |
| --- | --- | --- |
| `tokens.txt` | `lexer.exe` | One textual token per line. |
| `parse.txt` | `parser.exe` | Printed simplified AST. |
| `semantic.txt` | `semantic.exe` | Semantic diagnostics and symbol table. |
| `tac.txt` | `tac.exe` | Three-address-code listing. |
| `optimized.txt` | `optimizer.exe` | TAC with supported constant arithmetic folded. |
| `cfg.dot` | `cfg.exe` | Graphviz DOT representation of sequential TAC lines. |
| `cfg.png` | Supplied/generated separately | Image currently loaded by the UI's CFG action. |

The lexer, parser, and semantic programs overwrite their respective output files on each run. These generated `.txt` files and executables are ignored by Git.

## Current limitations

- The parser reads source lines directly; it does not consume `tokens.txt` or the lexer’s token stream. It is a collection of simple line-based cases, not a complete grammar parser.
- Parsing is limited mainly to integer declarations, assignments, and a simple one-line `if` condition. The `if` body is not attached to the AST, and blocks, nested statements, `else`, loops, function declarations, and general expression precedence are not fully implemented.
- Semantic analysis checks declarations, redeclarations, assignment targets, and identifiers used in assignment expressions using a single basic symbol table. It does not perform type checking, scope handling, or complete control-flow analysis; for example, identifiers in an `if` condition are not checked.
- TAC generation and optimization are separate executables and are not part of the web server's current full-pipeline route. TAC generation of `if` statements emits branch labels but does not generate the conditional body from the current AST.
- The CFG generator connects each non-empty TAC line sequentially; it does not infer branch edges or basic blocks. DOT labels are not escaped for arbitrary instruction text.
- Constant folding handles only simple integer arithmetic patterns. It is not a general optimizer and does not implement floating-point folding or robust divide-by-zero diagnostics.
- The UI's Optimization action is a hard-coded illustration. The Run All button invokes only tokenization, parsing, and semantic analysis; its optimization/CFG messages are informational, not generated results.
- `main.cpp` is an additional driver, but it cannot currently be linked together with `cfg.cpp` as-is because both files define `main()`. The browser setup above uses the separate `parser_main.cpp` and `semantic_main.cpp` entry points instead.
- `npm test` is a placeholder and exits with an error because no automated test suite is configured.

## Troubleshooting

- **“Lexer/Parser/Semantic Error” in the page:** build the three required `.exe` files in the project root, then restart the Node server.
- **Executable not found:** use Windows with MinGW-built `.exe` files, and start `npm start` from the repository root.
- **Page does not load at `/`:** open `/frontend.html` explicitly; that is the UI filename served by Express.
- **CFG image does not change:** the UI reads the existing `cfg.png`; regenerate the DOT and image manually if needed.
- **Port 3000 is occupied:** stop the other service using that port. The port is currently fixed in `server.js`.
- **Unexpected parse results:** check the supported subset above. The parser expects simple, line-oriented syntax and is not equivalent to a general C/C++ parser.
