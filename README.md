# Zh Virtual Machine
Custom virtual machine / interpreter

# Characteristics

# ISA by pages

### > Pages #0 and #1

| ID     | ARGV | NAME  |   | ID     | ARGV | NAME  |
|--------|------|-------|---|--------|------|-------|
| 0X0000 | 0    | NOP   |   | 0X0000 | 2    | push  |
| 0X0001 | 2    | MOVR  |   | 0X0001 | 2    | pop   |
| 0X0010 | 2    | MOVI  |   | 0X0010 | 1    | mul   |
| 0X0011 | 2    | ADDR  |   | 0X0011 | 1    | div   |
| 0X0100 | 2    | ADDI  |   | 0X0100 | 0    | null  |
| 0X0101 | 2    | SUBR  |   | 0X0101 | 0    | null  |
| 0X0110 | 2    | SUBI  |   | 0X0110 | 0    | null  |
| 0X0111 | 1    | JMP   |   | 0X0111 | 0    | null  |
| 0X1000 | 2    | CMP   |   | 0X1000 | 0    | null  |
| 0X1001 | 1    | JA    |   | 0X1001 | 0    | null  |
| 0X1010 | 1    | JB    |   | 0X1010 | 0    | null  |
| 0X1011 | 1    | JE    |   | 0X1011 | 0    | null  |
| 0X1100 | 1    | WR    |   | 0X1100 | 0    | null  |
| 0X1101 | 1    | RE    |   | 0X1101 | 0    | null  |
| 0X1110 | 0    | EXIT  |   | 0X1110 | 0    | null  |
| 0X1111 | 1    | ISA   |   | 0X1111 | 1    | isa   |
