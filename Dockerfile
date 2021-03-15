FROM ubuntu:18.04

WORKDIR /app

COPY . /app

RUN apt update -y && apt install make clang -y

RUN make 

CMD ["./build/server", "$PORT"]