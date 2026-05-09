# simulador_assembly_CPU

Monitor de sistema em Assembly x86_64 para Linux, usando NASM e syscalls diretas.

O projeto lê dados reais de `/proc` e renderiza uma interface de terminal no estilo DOS/UNIX, sem libc e sem dependências de UI externas.

## Objetivo

- Exibir nome do processador.
- Exibir memória total e memória disponível.
- Exibir uptime do sistema.
- Manter uma base modular para evoluir parsing, interface, logs e coleta de métricas.

## Execução

```bash
make
./build/asm-sysmon
```

Para sair:

```text
q + Enter
```

## Estrutura

```text
include/constants.inc  Constantes de syscall, buffers e refresh
src/main.asm           Loop principal
src/proc.asm           Leitura de /proc
src/ui.asm             Renderização ANSI no terminal
src/input.asm          Entrada de teclado e saída limpa
src/syscalls.asm       Wrappers de syscalls Linux
docs/                  Documentação técnica e planejamento
```

## Documentação

- [Índice documental](docs/README.md)
- [Histórico e evolução](docs/history/linha-do-tempo.md)
- [Arquitetura](docs/architecture/arquitetura.md)
- [UML](docs/uml/diagramas.md)
- [Casos de uso](docs/use-cases/casos-de-uso.md)
- [Padrões modernos aplicados](docs/padroes-modernos.md)

## Importância

ASM-SysMon é útil como estudo prático de baixo nível: mostra como um programa Linux pode coletar dados do sistema, controlar terminal e organizar módulos sem depender de runtime externo.
