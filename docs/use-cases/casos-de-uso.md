# Casos de Uso

## UC01 - Visualizar status do sistema

Ator: usuário no terminal.

Fluxo principal:

1. Usuário executa `./build/asm-sysmon`.
2. Sistema limpa a tela.
3. Sistema lê dados de `/proc`.
4. Sistema renderiza CPU, memória e uptime.
5. Sistema atualiza periodicamente a visualização.

Resultado esperado:

- usuário acompanha dados básicos do sistema em tempo real.

## UC02 - Encerrar o monitor

Ator: usuário no terminal.

Fluxo principal:

1. Usuário pressiona `q + Enter`.
2. Sistema detecta entrada com `poll`.
3. Sistema restaura cores e cursor.
4. Sistema encerra com status `0`.

Resultado esperado:

- terminal volta a um estado utilizável.

## UC03 - Evoluir novas métricas

Ator: desenvolvedor.

Fluxo principal:

1. Desenvolvedor escolhe nova fonte em `/proc` ou `/sys`.
2. Implementa leitura em módulo específico.
3. Expõe buffer ou parser.
4. Adiciona seção visual na UI.
5. Atualiza documentação e testes manuais.

Resultado esperado:

- nova métrica é adicionada sem quebrar os módulos existentes.

## UC04 - Estudar syscalls Linux

Ator: estudante ou desenvolvedor.

Fluxo principal:

1. Usuário lê `syscalls.asm`.
2. Compara wrappers com chamadas feitas em `proc.asm`, `ui.asm` e `input.asm`.
3. Executa o programa e observa o comportamento.

Resultado esperado:

- entendimento prático de `open`, `read`, `write`, `poll`, `nanosleep` e `exit`.
