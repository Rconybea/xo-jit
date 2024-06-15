# xo-jit library

A library for representing abstract syntax trees for EGAD (a small jit-based language).

## Links

- [new pass manager (26mar2021)](https://blog.llvm.org/posts/2021-03-26-the-new-pass-manager)
- [life of an llvm instruction (24nov2012)](https://eli.thegreenplace.net/2012/11/24/life-of-an-instruction-in-llvm)

## Getting Started

### build + install `xo-cmake` dependency

- [github/Rconybea/xo-cmake](https://github.com/Rconybea/xo-cmake)

Installs a few cmake ingredients,  along with a build assistant `xo-build` for XO projects such as this one.

### build + install other necessary XO dependencies
```
$ xo-build --clone --configure --build --install xo-indentlog
$ xo-build --clone --configure --build --install xo-refnct
$ xo-build --clone --configure --build --install xo-subsys
$ xo-build --clone --configure --build --install xo-reflect
$ xo-build --clone --configure --build --install xo-expression
```

Note: can use `xo-build -n` to dry-run here

### copy `xo-jit` repository locally
```
$ xo-build --clone xo-jit
```

or equivalently
```
$ git clone git@github.com:Rconybea/xo-jit.git
```

### build + install xo-jit
```
$ xo-build --configure --build --install xo-jit
```

or equivalently:
```
$ PREFIX=/usr/local  # or wherever you prefer
$ cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} -S xo-jit -B xo-jit/.build
$ cmake --build xo-jit/.build
$ cmake --install xo-jit/.build
```

### build for unit test coverage
```
$ cmake -DCMAKE_BUILD_TYPE=coverage -DCMAKE_INSTALL_PREFIX=$PREFIX xo-jit/.build-ccov
$ cmake --build xo-jit/.build-ccov
```

### LSP support
```
$ cd xo-jit
$ ln -s build/compile_commands.json  # lsp will look for compile_commands.json in the root of the source tree
```
