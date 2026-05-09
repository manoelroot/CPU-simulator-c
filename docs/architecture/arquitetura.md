# Arquitetura

## Visão geral

ASM-SysMon segue uma arquitetura modular simples:

```text
Entrada do usuário -> main.asm -> proc.asm -> ui.asm -> terminal
                           |
                           v
                      syscalls.asm
```

## Módulos

### `main.asm`

Responsável pelo ciclo de execução:

- chama coleta de CPU, memória e uptime;
- chama renderização da UI;
- verifica entrada do usuário;
- aguarda o próximo refresh com `nanosleep`.

### `proc.asm`

Responsável por abrir, ler e fechar arquivos virtuais de `/proc`.

Arquivos lidos:

- `/proc/cpuinfo`;
- `/proc/meminfo`;
- `/proc/uptime`.

### `ui.asm`

Responsável por transformar buffers em saída de terminal.

Recursos atuais:

- limpeza de tela;
- moldura ASCII;
- cores ANSI;
- indicador animado;
- filtragem simples de linhas relevantes.

### `input.asm`

Responsável por entrada não bloqueante simplificada:

- usa `poll`;
- lê uma tecla quando disponível;
- encerra com `q`;
- restaura cursor e cores antes do `exit`.

### `syscalls.asm`

Centraliza wrappers de syscalls:

- `read`;
- `write`;
- `open`;
- `close`;
- `poll`;
- `nanosleep`;
- `exit`.

## Decisões técnicas

- Não usar libc para manter foco didático em syscalls.
- Usar `/proc` como fonte de telemetria por ser padrão UNIX/Linux.
- Usar ANSI escape codes em vez de ncurses para reduzir dependências.
- Manter buffers fixos para simplificar alocação e controle de memória.

## Limitações atuais

- Parsing ainda é textual e simples.
- O layout assume largura aproximada de 80 colunas.
- O uptime ainda é exibido no formato bruto de `/proc/uptime`.
- A leitura de CPU e memória usa campos específicos, sem fallback por localidade.
