# Padrões Modernos Aplicados

Mesmo sendo um projeto de baixo nível em C + Assembly, ASM-SysMon pode seguir padrões modernos de engenharia.

## Separação de responsabilidades

Cada módulo tem uma responsabilidade clara:

- `main.c`: coordenação;
- `main.c`: coordenação;
- `proc.c`: coleta;
- `ui.c`: apresentação;
- `input.c`: interação;
- `syscalls.asm`: acesso ao kernel.

Importância:

- reduz acoplamento;
- facilita manutenção;
- permite evolução incremental.

## Camada de infraestrutura

`syscalls.asm` funciona como uma camada de infraestrutura crítica. O restante do projeto usa o contrato de `include/sysmon.h` e não precisa repetir números de syscalls diretamente.

Importância:

- centraliza mudanças;
- melhora legibilidade;
- reduz duplicação.

## Observabilidade simples

A interface mostra um indicador animado de atualização.

Importância:

- informa que o programa está vivo;
- ajuda a perceber travamentos;
- melhora experiência sem dependências externas.

## Documentação viva

Diagramas, casos de uso e SVGs ficam no repositório junto com o código.

Importância:

- facilita revisão;
- ajuda novos colaboradores;
- mantém decisões técnicas rastreáveis.

## Build reproduzível

O `Makefile` define o caminho esperado para compilar C freestanding, montar Assembly NASM, linkar sem libc e limpar.

Importância:

- reduz passos manuais;
- facilita teste local;
- prepara base para CI futura.

## Evolução sugerida

Próximos padrões que podem ser adicionados:

- parser dedicado para transformar strings em valores numéricos;
- camada de métricas separada da camada de leitura bruta;
- testes automatizados de parsing com fixtures;
- documentação de ADRs para decisões importantes;
- integração CI com `make` e `git diff --check`.

## SDLC/cascata aplicado

Cada alteração significativa deve avançar por fases explícitas:

- requisito documentado;
- desenho de camadas e impacto visual;
- implementação em C ou Assembly conforme criticidade;
- verificação local com `make`;
- atualização de histórico, diagramas e padrões.
