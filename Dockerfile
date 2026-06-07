FROM ubuntu:24.04

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
       build-essential \
       nasm \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN make

CMD ["/app/build/asm-sysmon"]
