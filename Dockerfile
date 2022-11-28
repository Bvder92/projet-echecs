FROM debian:bullseye

WORKDIR /projet

RUN apt-get update && apt-get install -y neofetch gcc make git 

COPY . /projet

CMD echo "Hello World"
