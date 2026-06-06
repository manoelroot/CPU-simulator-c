# UML e Diagramas

Os diagramas usam Mermaid para manter a documentação versionável no próprio repositório.

## Componentes

```mermaid
flowchart LR
    User[Usuario no terminal] --> Input[input.c]
    Main[main.c] --> Proc[proc.c]
    Main --> UI[ui.c]
    Main --> Input
    Proc --> Syscalls[syscalls.asm]
    UI --> Syscalls
    Input --> Syscalls
    Syscalls --> Kernel[Linux kernel]
    Kernel --> ProcFS[/proc]
    Kernel --> Terminal[stdout/stdin]
```

## Sequência de atualização

```mermaid
sequenceDiagram
    participant U as Usuario
    participant M as main.c
    participant P as proc.c
    participant UI as ui.c
    participant I as input.c
    participant K as Kernel Linux

    M->>P: read_cpuinfo()
    P->>K: open/read/close /proc/cpuinfo
    M->>P: read_meminfo()
    P->>K: open/read/close /proc/meminfo
    M->>P: read_uptime()
    P->>K: open/read/close /proc/uptime
    M->>UI: render_screen()
    UI->>K: write(stdout)
    M->>I: check_input()
    I->>K: poll(stdin)
    U-->>I: q + Enter
    I->>K: write(reset terminal)
    I->>K: exit(0)
```

## Estados do programa

```mermaid
stateDiagram-v2
    [*] --> Inicializando
    Inicializando --> Coletando
    Coletando --> Renderizando
    Renderizando --> VerificandoEntrada
    VerificandoEntrada --> Encerrando: q
    VerificandoEntrada --> AguardandoRefresh: sem q
    AguardandoRefresh --> Coletando
    Encerrando --> [*]
```

## Organização de módulos

```mermaid
classDiagram
    class Main {
        +_start()
    }

    class Proc {
        +read_cpuinfo()
        +read_meminfo()
        +read_uptime()
    }

    class UI {
        +render_screen()
        +write_matching_line()
        +write_data_line()
        +write_status_line()
    }

    class Input {
        +check_input()
    }

    class Syscalls {
        +sys_open()
        +sys_read()
        +sys_write()
        +sys_close()
        +sys_poll()
        +sys_nanosleep()
        +sys_exit()
    }

    Main --> Proc
    Main --> UI
    Main --> Input
    Proc --> Syscalls
    UI --> Syscalls
    Input --> Syscalls
```
