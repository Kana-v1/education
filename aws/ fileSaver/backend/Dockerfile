FROM golang:1.18.0-alpine3.15 as builder

ADD . /app/
WORKDIR /app

RUN go build main.go

FROM alpine:3.12 as runtime
WORKDIR /app
COPY --from=builder /app/main .
CMD ["./main"]

