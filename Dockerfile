FROM ubuntu

WORKDIR /app

COPY . /app

EXPOSE 8080

RUN apt update -y && apt install make clang -y

RUN make 
