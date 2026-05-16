# DragonTongue Glossary

## A

**AST (Abstract Syntax Tree)** - Tree representation of source code after parsing.

**Arena Allocator** - Memory pool that allocates sequentially and frees all at once.

**Await** - Keyword to wait for a spawned task to complete.

## B

**Binary Operator** - Operator that takes two operands (e.g., `+`, `-`, `*`).

**Bool** - Boolean type, values `true` or `false`.

## C

**Channel** - Communication primitive for passing values between tasks.

**Code Generation** - Phase that produces output code (C, machine code, etc.).

**Concurrency** - Multiple tasks making progress simultaneously.

## D

**DragonTongue** - The programming language documented here.

## E

**Error Handling** - Mechanism for dealing with failures (`catch/else`).

**Expression** - Code that produces a value (e.g., `5 + 3`).

## F

**Float** - Floating-point number type.

**Function** - Reusable block of code that may return a value.

## G

**GC (Garbage Collection)** - Automatic memory management (DragonTongue doesn't use this).

## H

**Heap** - Memory region for dynamic allocation (explicit in DragonTongue).

## I

**Identifier** - Name given to variable, function, or struct.

**Immutable** - Cannot be changed after initialization (default in DragonTongue).

**Int** - Integer type (64-bit signed).

## L

**Let** - Keyword for immutable variable declaration.

**Lexer** - Phase that converts source text into tokens.

**Line** - Source code line number (used in error messages).

## M

**Match** - Pattern matching construct.

**Method** - Function attached to a struct.

**Mutable** - Can be changed (use `var` keyword).

## N

**Node** - Element in an Abstract Syntax Tree.

**Null** - Absence of value (DragonTongue does NOT have null).

## O

**Option** - Type representing optional value (`Some` or `None`).

## P

**Parallel Block** - `parallel { ... }` runs multiple tasks concurrently.

**Parser** - Phase that converts tokens to AST.

**Pattern Matching** - Deconstructing values with `match`.

## R

**Recursion** - Function that calls itself.

**Runtime** - Library that provides language features during execution.

## S

**Scanner** - Another name for lexer.

**Semantic Analysis** - Phase that checks types and validates rules.

**Spawn** - Keyword to create a new concurrent task.

**Stack** - Memory region for local variables (default in DragonTongue).

**Statement** - Code that performs an action (no return value).

**String** - Text type.

**Struct** - Composite type grouping multiple values.

**Symbol Table** - Data structure mapping names to their definitions.

## T

**Task** - Lightweight concurrent unit of execution.

**Token** - Smallest meaningful unit of source code.

**Transpiler** - Compiler that converts one language to another.

**Type** - Classification of a value (Int, String, etc.).

**Type Inference** - Compiler determining types automatically.

## V

**Var** - Keyword for mutable variable declaration.

**Variable** - Named storage for a value.

---

## Acronyms

| Acronym | Meaning |
|---------|---------|
| AST | Abstract Syntax Tree |
| API | Application Programming Interface |
| CLI | Command Line Interface |
| FFI | Foreign Function Interface |
| GC | Garbage Collection |
| HTTP | Hypertext Transfer Protocol |
| IR | Intermediate Representation |
| JSON | JavaScript Object Notation |
| M:N | M user threads on N OS threads |
| WASM | WebAssembly |