#Crée un conteneur à partir de l'image Debian avec dossier /projet dans lequel tout le contenu du dossier du projet est copié

FROM debian:bullseye

WORKDIR /projet

RUN apt-get update && apt-get install -y gcc make git 

COPY . /projet

CMD echo "Hello World"
