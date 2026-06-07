# Arquitetura

## Visão geral

ASM-SysMon segue uma arquitetura modular simples em camadas.

```text
Entrada do usuário -> input.c
                         ^
                         |
main.c -> proc.c -> syscalls.asm -> Linux kernel -> /proc
   |
   v
 ui.c -> syscalls.asm -> Linux kernel -> terminal
```

![Arquitetura C e Assembly](../diagrams/architecture.svg)

## Módulos

### `main.c`

Responsável pelo ciclo de execução:

- chama coleta de CPU, memória e uptime;
- chama renderização da UI;
- verifica entrada do usuário;
- aguarda o próximo refresh com `nanosleep`.

### `proc.c`

Responsável por abrir, ler e fechar arquivos virtuais de `/proc`.

Arquivos lidos:

- `/proc/cpuinfo`;
- `/proc/meminfo`;
- `/proc/uptime`.

### `ui.c`

Responsável por transformar buffers em saída de terminal.

Recursos atuais:

- limpeza de tela;
- moldura Unicode/ANSI;
- cores e alinhamento de colunas;
- indicador animado;
- seção de status e barra de memória.

### `input.c`

Responsável por entrada não bloqueante simplificada:

- usa `poll`;
- lê tecla quando disponível;
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

- Usar C freestanding para reduzir complexidade de fluxo, parsing e UI;
- não usar libc para manter foco didático em syscalls;
- manter Assembly nas chamadas críticas ao kernel;
- usar `/proc` como fonte de telemetria padrão UNIX/Linux;
- usar ANSI escape codes em vez de ncurses para reduzir dependências;
- manter buffers fixos para simplificar alocação e controle de memória.

## Diagramas SVG

Os artefatos visuais agora estão centralizados em `docs/diagrams/`:

- `docs/diagrams/architecture.svg`
- `docs/diagrams/sequence.svg`
- `docs/diagrams/state.svg`

## SDLC em cascata

Cada versão do projeto evolui em cinco fases:

1. Requisitos: definição das métricas, entradas e comportamento esperado;
2. Projeto: desenho da arquitetura, contratos e impacto visual;
3. Implementação: alteração dos módulos e preservação das camadas críticas;
4. Verificação: compilação com `make`, testes de execução e revisão da documentação;
5. Manutenção: atualização do histórico, dos diagramas e dos padrões de commit.

## Limitações atuais

- Parsing ainda é textual e simples.
- O layout assume largura aproximada de 80 colunas.
- O uptime ainda é exibido no formato bruto de `/proc/uptime`.
- A leitura de CPU e memória usa campos específicos, sem fallback por localidade.
