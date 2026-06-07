# Histórico e Evolução

Este histórico segue o método cascata para cada versão do projeto:
requisitos, projeto, implementação, verificação e manutenção.

## Versão 0.1 — Base mínima do monitor

### Requisitos
- monitorar CPU, memória e uptime com syscalls Linux diretas;
- não depender de libc;
- apresentar os dados em terminal.

### Projeto
- definir módulos para fluxo, coleta, UI e syscalls;
- usar `/proc` como fonte de dados;
- manter o código simples e rastreável.

### Implementação
- criar `main.c`, `proc.c`, `ui.c`, `input.c`, `syscalls.asm`;
- implementar leitura de `/proc/cpuinfo`, `/proc/meminfo`, `/proc/uptime`;
- compilar com `Makefile` para gerar `build/asm-sysmon`.

### Verificação
- executar o binário e validar saída no terminal;
- confirmar uso correto de `open`, `read`, `write`, `poll`, `nanosleep`, `exit`.

### Manutenção
- documentar as responsabilidades dos arquivos;
- manter exemplo de execução no `README.md`.

## Versão 0.2 — Modularização e build reprodutível

### Requisitos
- suportar compilações locais consistentes;
- isolar a UI de apresentação da coleta de dados;
- melhorar a estrutura de commits e documentação.

### Projeto
- ajustar `Makefile` para build e limpeza;
- documentar arquitetura e casos de uso;
- criar mapas de dependência entre módulos.

### Implementação
- atualizar `Makefile` para produzir `build/asm-sysmon` e remover artefatos;
- separar parsing bruto de apresentação;
- criar documentação inicial em `docs/`.

### Verificação
- rodar `make` e `make clean`;
- executar o programa em ambiente limpo;
- revisar documentação existente.

### Manutenção
- acrescentar `docs/README.md` e índices de documentação;
- garantir que diagramas e histórico acompanhem alterações.

## Versão 0.3 — UI moderna em C com Assembly crítico

### Requisitos
- renderizar dashboard moderno no terminal;
- manter syscalls críticos em Assembly;
- preservar o modelo freestanding em C.

### Projeto
- definir layout ANSI com bordas, barra de memória e indicador de refresh;
- manter `src/syscalls/syscalls.asm` como único ponto de acesso direto ao kernel;
- documentar arquitetura e fluxo em SVG.

### Implementação
- atualizar `src/ui/ui.c` para novo dashboard;
- manter `src/core/proc.c` e `src/input/input.c` responsáveis por coleta e teclado;
- incluir `docs/diagrams/` com diagramas visuais.

### Verificação
- validar o dashboard localmente em terminal ANSI;
- testar a saída `timeout 2 ./build/asm-sysmon | head -40`;
- confirmar que `q + Enter` encerra corretamente.

### Manutenção
- criar documentação de testes em `docs/tests.md`;
- atualizar `README.md` para referenciar diagramas e validação.

## Versão 0.4 — Documentação formal e testes

### Requisitos
- documentar todos os testes e a estratégia de validação;
- mapear cada versão segundo o método cascata;
- incluir diagramas SVG versionáveis.

### Projeto
- especificar fluxo de requisitos, projeto, implementação, verificação e manutenção;
- criar arquivos de documentação para arquitetura, testes, histórico e casos de uso;
- centralizar artefatos visuais em `docs/diagrams/`.

### Implementação
- adicionar `docs/tests.md`;
- gerar `docs/diagrams/architecture.svg`, `docs/diagrams/sequence.svg` e `docs/diagrams/state.svg`;
- atualizar `docs/README.md` e `docs/architecture/arquitetura.md`.

### Verificação
- revisar documentação e linkagem entre arquivos;
- confirmar que os diagramas SVG abrem corretamente;
- validar que a documentação corresponde ao código existente.

### Manutenção
- manter o histórico em cascata atualizado a cada versão;
- documentar novas métricas, mudanças de UI e ajustes de build.
