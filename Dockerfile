FROM heroku/heroku:20-build as builder

WORKDIR /app

COPY . .

RUN make

FROM heroku/heroku:20

WORKDIR /srv

COPY --from=builder /app .

RUN echo 5000

CMD ["./build/server", "5000"]