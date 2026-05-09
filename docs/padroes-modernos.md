# Padrões Modernos Aplicados

Mesmo sendo um projeto em Assembly, ASM-SysMon pode seguir padrões modernos de engenharia.

## Separação de responsabilidades

Cada módulo tem uma responsabilidade clara:

- `main.asm`: coordenação;
- `proc.asm`: coleta;
- `ui.asm`: apresentação;
- `input.asm`: interação;
- `syscalls.asm`: acesso ao kernel.

Importância:

- reduz acoplamento;
- facilita manutenção;
- permite evolução incremental.

## Camada de infraestrutura

`syscalls.asm` funciona como uma camada de infraestrutura. O restante do projeto não precisa repetir números de syscalls diretamente.

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

Diagramas e casos de uso ficam no repositório junto com o código.

Importância:

- facilita revisão;
- ajuda novos colaboradores;
- mantém decisões técnicas rastreáveis.

## Build reproduzível

O `Makefile` define o caminho esperado para compilar e limpar.

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
