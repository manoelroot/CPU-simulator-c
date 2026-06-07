# Documentação do ASM-SysMon

Este diretório organiza a documentação técnica do projeto desde a base inicial até as próximas evoluções.

## Mapa documental

- [Histórico e evolução](history/linha-do-tempo.md): origem, mudanças realizadas e próximos marcos.
- [Arquitetura](architecture/arquitetura.md): módulos, responsabilidades e fluxo de execução.
- [UML](uml/diagramas.md): diagramas em Mermaid para arquitetura e fluxo.
- [Casos de uso](use-cases/casos-de-uso.md): interações principais do usuário.
- [Padrões modernos aplicados](padroes-modernos.md): práticas de organização, manutenção e evolução.
- [Testes e validação](tests.md): roteiro de compilação, execução local e Docker.
- [Diagramas SVG](diagrams/architecture.svg): artefatos visuais de arquitetura e fluxo.

## Escopo atual

O ASM-SysMon é um monitor de sistema em C freestanding + Assembly x86_64 para Linux. A versão atual:

- compila C com `gcc -ffreestanding` e Assembly com NASM;
- usa syscalls Linux diretas;
- lê `/proc/cpuinfo`, `/proc/meminfo` e `/proc/uptime`;
- renderiza uma UI ANSI com moldura, cores e indicador animado;
- permite saída limpa com `q + Enter`.
- mantém documentação de testes em `docs/tests.md` e diagramas em `docs/diagrams/`;

## Critérios de evolução

Toda mudança deve preservar:

- simplicidade do código C e Assembly;
- separação clara entre coleta, UI, entrada e syscalls;
- execução sem libc;
- documentação atualizada junto com a implementação.

## Estrutura de commit

Mudanças significativas devem seguir commits pequenos e rastreáveis:

- `feat:` novas capacidades de execução, coleta ou UI;
- `docs:` atualização de documentação e diagramas;
- `build:` ajustes de Makefile, flags e artefatos gerados;
- `refactor:` reorganização sem mudança de comportamento esperado;
- `chore:` manutenção sem impacto funcional.
