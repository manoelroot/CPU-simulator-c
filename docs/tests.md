# Testes e Validação

## Objetivo

Documentar os testes aplicados à versão atual do ASM-SysMon, garantindo que a compilação, execução e comportamento da UI sejam validados de forma reproduzível.

## Ambiente de teste

- Linux x86_64
- `gcc`, `nasm` e `make`
- Terminal ANSI com suporte a UTF-8
- Opcional: Docker para validação de ambiente isolado

## Passos de teste

1. Compilar o projeto:

```bash
make
```

2. Verificar artefatos gerados:

- `build/asm-sysmon`

3. Executar o monitor localmente:

```bash
./build/asm-sysmon
```

4. Confirmar o comportamento principal:

- a interface limpa e redesenha em loop;
- o nome do processador aparece;
- a barra de memória e uptime são exibidos;
- o indicador de refresh muda (`|`, `/`, `-`, `\`);
- `q + Enter` encerra o programa e restaura o terminal.

5. Teste de regressão com saída limitada:

```bash
timeout 2 ./build/asm-sysmon | head -40
```

6. Teste em contêiner Docker:

```bash
docker build -t cpu-sim-modern .
docker run --rm cpu-sim-modern timeout 2 ./build/asm-sysmon | head -40
```

## Cobertura dos testes

- compilação do binário C/Assembly
- leitura de `/proc/cpuinfo`, `/proc/meminfo` e `/proc/uptime`
- renderização ANSI da UI
- entrada de teclado com `poll`
- uso de `nanosleep` para refresh
- saída limpa via `exit`

## Critérios de aceitação

- `make` termina sem erros;
- `./build/asm-sysmon` produz dashboard legível;
- pressionar `q + Enter` retorna ao prompt;
- o contêiner Docker executa e mostra o mesmo dashboard básico.

## Observações

- Em terminais sem suporte UTF-8, os caracteres de borda podem apresentar artefatos.
- A validação Docker serve para garantir que a cadeia de build e execução é reproduzível em ambientes limpos.
