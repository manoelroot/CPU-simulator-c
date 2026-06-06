# Histórico e Evolução

## Marco 0 - Ideia inicial

O projeto nasceu como uma base simples de monitoramento em Assembly para Linux Debian em arquitetura AMD/Intel x86_64.

Objetivo inicial:

- coletar nome da CPU;
- coletar memória total e disponível;
- coletar uptime;
- fazer tudo com syscalls diretas, sem libc.

Importância:

- estudar chamadas de sistema Linux;
- entender leitura de arquivos virtuais em `/proc`;
- exercitar modularização em Assembly.

## Marco 1 - Base modular

Foram definidos módulos principais na base Assembly inicial:

- `main.asm`: loop principal;
- `proc.asm`: leitura dos arquivos em `/proc`;
- `ui.asm`: renderização no terminal;
- `syscalls.asm`: camada única para syscalls;
- `input.asm`: leitura de teclado.

Importância:

- evita concentrar toda lógica em um único arquivo;
- facilita teste incremental;
- abre espaço para novas métricas.

## Marco 2 - Build funcional

O `Makefile` foi ajustado para montar objetos NASM e linkar o binário em `build/asm-sysmon`.

Importância:

- padroniza execução local;
- reduz erro manual de montagem;
- permite limpar artefatos com `make clean`.

## Marco 3 - Coleta real de dados

O programa passou a ler:

- `/proc/cpuinfo`;
- `/proc/meminfo`;
- `/proc/uptime`.

Importância:

- troca dados simulados por dados reais do sistema;
- mantém compatibilidade com o modelo UNIX de arquivos virtuais;
- cria base para parsing mais avançado.

## Marco 4 - UI de terminal

A visualização evoluiu para uma interface ANSI com:

- moldura ASCII;
- cores;
- seções separadas;
- cursor oculto durante a atualização;
- restauração do terminal ao sair.

Importância:

- melhora leitura;
- aproxima o projeto de ferramentas clássicas de terminal;
- mantém compatibilidade com ambientes sem UI gráfica.

## Marco 5 - Otimização de refresh

O loop de atraso ocupado foi substituído por `nanosleep`.

Importância:

- reduz desperdício de CPU;
- deixa o programa mais próximo de um daemon/monitor real;
- separa atualização visual de consumo agressivo de processamento.

## Marco 6 - Animação

Foi adicionado um indicador animado de atualização no rodapé da interface.

Importância:

- mostra que o loop está vivo;
- melhora feedback visual;
- mantém implementação simples e compatível com terminal ANSI.

## Marco 7 - Estrutura C + Assembly

A base foi reorganizada para usar C freestanding nas camadas de fluxo, coleta, input e UI, mantendo `syscalls.asm` como ponto crítico de acesso ao kernel.

Importância:

- reduz complexidade de manutenção fora da fronteira crítica;
- preserva o estudo de syscalls Linux em Assembly;
- prepara evolução de parsing e métricas com menor atrito.

## Marco 8 - Documentação SDLC e artefato visual

Foi adicionado o SVG `src/assets/architecture.svg` e a documentação passou a explicitar o fluxo SDLC/cascata para mudanças significativas.

Importância:

- registra a arquitetura de camadas de forma visual;
- padroniza requisitos, projeto, implementação, verificação e manutenção;
- orienta commits pequenos com prefixos claros.
