FROM ubuntu

WORKDIR /app

COPY . /app

EXPOSE $PORT

RUN apt update -y && apt install make clang -y

RUN make 
