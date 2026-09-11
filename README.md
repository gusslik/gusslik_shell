
# Gusslik Shell

My own implementation of a simple Linux shell, which allows to run built-in commands or custom execute files.
## Deployment

### With CMake

To build this project with CMake run this from the root directory of the repository:

```bash
cmake -S . -B build
cmake --build build
```

Then run the executable with this command:

```bash
./build/gusslik_shell
```

### With GCC

To build this project with GCC run this from the root directory of the repository:

```bash
gcc main.c -o gusslik_shell
```

Then run the executable with this command:

```bash
./gusslik_shell
```