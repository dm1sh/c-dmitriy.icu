FROM alpine as builder

RUN apk add --no-cache build-base

WORKDIR /app

COPY . .

RUN make

FROM alpine

WORKDIR /srv

COPY --from=builder /app .

RUN apk add --no-cache gcompat

CMD ["build/server", "5000"]
