# Zh Virtual Machine
Custom virtual machine / interpreter

# Index

* [Zh Virtual Machine](#zh-virtual-machine)
  * [Characteristics](#characteristics)
* [Instruction composition](#instruction-composition)
  * [With 2 arguments](#with-2-arguments)
  * [With 1 arguments](#with-1-arguments)
  * [With 0 arguments](#with-0-arguments)
* [ISA by pages](#isa-by-pages)
  * [Page #0](#page-0)
  * [Page #1](#page-1)
* [Installation](#how-to-build-it)

# Characteristics
- 16 bit instruction
- 64 KiB of memory per page
- 65536 memory pages
- 64 KiB of memory per stack
- 65536 stacks
- 64 registers of 16 bits each
- 32 flags of 1 bit each

# Instruction composition

### With 2 arguments
| ID     | OP0    | OP1    |
|--------|--------|--------|
| 4 bits | 6 bits | 6 bits |

### With 1 arguments
| ID     | OP0     |
|--------|---------|
| 4 bits | 12 bits |

### With 0 arguments
| ID     | NULL    |
|--------|---------|
| 4 bits | 12 bits |

# ISA by pages

### Page #0

| ID     | ARGV | NAME  | ARGS               |
|--------|------|-------|--------------------|
| 0x0000 | 0    | NOP   |                    |
| 0x0001 | 2    | MOVR  | destination, value |
| 0x0010 | 2    | MOVI  | destination, value |
| 0x0011 | 2    | ADDR  | destination, value |
| 0x0100 | 2    | ADDI  | destination, value |
| 0x0101 | 2    | SUBR  | destination, value |
| 0x0110 | 2    | SUBI  | destination, value |
| 0x0111 | 1    | JMP   | memory address     |
| 0x1000 | 2    | CMP   | destination, value |
| 0x1001 | 1    | JA    | memory address     |
| 0x1010 | 1    | JB    | memory address     |
| 0x1011 | 1    | JE    | memory address     |
| 0x1100 | 1    | WR    | memory address     |
| 0x1101 | 1    | RE    | memory address     |
| 0x1110 | 0    | EXIT  |                    |
| 0x1111 | 1    | ISA   | page number        |

### Page #1

| ID     | ARGV | NAME  | ARGS               |
|--------|------|-------|--------------------|
| 0x0000 | 2    | PUSH  | destination, value |
| 0x0001 | 2    | POP   | destination, value |
| 0x0010 | 1    | MUL   | destination, value |
| 0x0011 | 1    | DIV   | destination, value |
| 0x0100 | 0    | NULL  |                    |
| 0x0101 | 0    | NULL  |                    |
| 0x0110 | 0    | NULL  |                    |
| 0x0111 | 0    | NULL  |                    |
| 0x1000 | 0    | NULL  |                    |
| 0x1001 | 0    | NULL  |                    |
| 0x1010 | 0    | NULL  |                    |
| 0x1011 | 0    | NULL  |                    |
| 0x1100 | 0    | NULL  |                    |
| 0x1101 | 0    | NULL  |                    |
| 0x1110 | 0    | NULL  |                    |
| 0x1111 | 1    | ISA   | page number        |

### How to build it

* Linux with cmake
```
$ git clone https://github.com/solanav/zh_vm
$ cd zh_vm
$ mkdir build
$ cmake ..
$ make
```

* Windows with Visual Studio
```
1. Go to File > Open > Cmake
2. Select CMakeLists.txt
3. ?
4. Profit
```
