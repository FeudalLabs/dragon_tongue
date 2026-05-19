# DragonTongue Language Support for VS Code

[![Version](https://img.shields.io/badge/version-0.0.1-blue.svg)](https://marketplace.visualstudio.com/items?itemName=dragontongue.dragontongue)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)

Syntax highlighting and language support for the **DragonTongue** programming language.

## Features

- ✅ Syntax highlighting for `.dt` files
- ✅ Support for all DragonTongue keywords
- ✅ Type highlighting (`Int`, `Bool`, `String`, `Option`)
- ✅ Built-in function highlighting (`println`, `print`, `len`, `to_string`, etc.)
- ✅ Concurrency keywords (`spawn`, `await`, `parallel`)
- ✅ String and number highlighting
- ✅ Operator highlighting
- ✅ Comment highlighting
- ✅ Auto-closing brackets and quotes

## DragonTongue Keywords

| Category | Keywords |
|----------|----------|
| **Control** | `let`, `var`, `fn`, `struct`, `if`, `else`, `for`, `while`, `return`, `match`, `type` |
| **Concurrency** | `spawn`, `await`, `parallel` |
| **Types** | `Int`, `Bool`, `String`, `Void`, `Option`, `Some`, `None` |
| **Built-ins** | `println`, `print`, `len`, `to_string`, `to_int`, `read_file`, `write_file` |
| **Constants** | `true`, `false` |

## Example

Here's what DragonTongue code looks like with syntax highlighting:

```dragontongue
// DragonTongue example program
let greeting = "Hello, DragonTongue!"
println(greeting)

fn add(a: Int, b: Int) -> Int {
    return a + b
}

let result = add(5, 3)
println("Result: " + to_string(result))

if result > 5 {
    println("Greater than 5")
} else {
    println("Less or equal to 5")
}

struct User {
    name: String
    age: Int
}

let user = User{name: "Alice", age: 30}
println(user.name)

spawn background_task()
await handle

parallel {
    task1 = fetch_data()
    task2 = process_data()
}
```

## Installation

### Method 1: VSIX Installation

Download the dragontongue-0.0.1.vsix file

Open Visual Studio Code

Press Ctrl+Shift+P

Type "Extensions: Install from VSIX"

Select the .vsix file

Reload VS Code

### Method 2: Copy to Extensions Folder

```
cp -r dragontongue ~/.vscode/extensions/
```

### Method 3: Symlink (for development)
```
ln -sf /path/to/dragontongue ~/.vscode/extensions/dragontongue
```

## Requirements

Visual Studio Code version 1.80.0 or higher

## Extension Settings

This extension contributes the following settings:

| Setting             | Description                                                  |
|---------------------|--------------------------------------------------------------|
| files.associations  | Automatically associates .dt files with DragonTongue language |

## Known Issues

No IntelliSense support yet (planned for future release)

No debugging support yet (planned for future release)

## Release Notes

### 0.0.1

Initial release

Basic syntax highlighting for all DragonTongue keywords, types, and built-ins

Language configuration (comments, brackets, auto-closing pairs)

## Contributing

Contributions are welcome! Please visit the DragonTongue GitHub repository for more information.

## License

MIT

Enjoy coding with DragonTongue! 🐉

