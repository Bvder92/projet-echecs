FROM debian:bullseye

WORKDIR /projet

RUN apt-get update && apt-get install -y gcc make git 

COPY . /projet

CMD echo "Hello World"
